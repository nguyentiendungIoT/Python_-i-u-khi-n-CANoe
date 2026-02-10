"""
CANoe Automation Test Tool - Professional GUI
==============================================
PyQt6-based GUI for CANoe Software-in-the-Loop (SIL) testing.
Wraps the CANoe COM API with proper threading architecture
to keep the UI responsive during long COM operations.

Architecture:
    UI Thread (MainWindow)  <--signals/slots-->  Worker Thread (CANoeWorker)
                                                    |
                                                    v
                                              CANoe COM Server
"""

import sys
import os
import time
import datetime
import traceback

# ---------------------------------------------------------------------------
# Dependency checks
# ---------------------------------------------------------------------------
try:
    from PyQt6.QtWidgets import (
        QApplication,
        QMainWindow,
        QWidget,
        QVBoxLayout,
        QHBoxLayout,
        QGroupBox,
        QLabel,
        QLineEdit,
        QPushButton,
        QTextEdit,
        QTreeWidget,
        QTreeWidgetItem,
        QProgressBar,
        QSplitter,
        QFileDialog,
        QMessageBox,
        QHeaderView,
        QSizePolicy,
    )
    from PyQt6.QtCore import (
        Qt,
        QThread,
        QObject,
        pyqtSignal,
        pyqtSlot,
        QTimer,
        QSettings,
    )
    from PyQt6.QtGui import (
        QFont,
        QColor,
        QTextCursor,
        QAction,
        QPixmap,
        QIcon,
    )
except ImportError:
    print("ERROR: PyQt6 is required.  pip install PyQt6")
    sys.exit(1)

try:
    import pythoncom
    from win32com.client import DispatchEx, CastTo, WithEvents, DispatchWithEvents

    HAS_COM = True
except ImportError:
    HAS_COM = False
    print("WARNING: pywin32 not found. COM features will be disabled.")

# ---------------------------------------------------------------------------
# Constants
# ---------------------------------------------------------------------------
APP_TITLE = "BLTN Team - YuRa HaNoi "
VERSION = "1.0.0"

VERDICT_NA = 0
VERDICT_PASSED = 1
VERDICT_FAILED = 2

LED_COLORS = {
    "idle": "#6c6c6c",
    "running": "#569cd6",
    "pass": "#4caf50",
    "fail": "#f44336",
    "error": "#ff9800",
}

# Resolve logo path relative to this script
_SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
LOGO_PATH = os.path.join(_SCRIPT_DIR, "YuRa.png")


# ---------------------------------------------------------------------------
# Dark professional stylesheet (automotive-tool look)
# ---------------------------------------------------------------------------
DARK_STYLE = """
QMainWindow, QWidget {
    background-color: #1e1e1e;
    color: #d4d4d4;
    font-family: "Segoe UI", sans-serif;
    font-size: 10pt;
}
QGroupBox {
    border: 1px solid #3c3c3c;
    border-radius: 4px;
    margin-top: 14px;
    padding-top: 14px;
    font-weight: bold;
    color: #569cd6;
}
QGroupBox::title {
    subcontrol-origin: margin;
    left: 10px;
    padding: 0 6px;
}
QLineEdit {
    background-color: #2d2d2d;
    border: 1px solid #3c3c3c;
    border-radius: 3px;
    padding: 5px 8px;
    color: #d4d4d4;
    selection-background-color: #264f78;
}
QLineEdit:focus { border-color: #569cd6; }

QPushButton {
    background-color: #0e639c;
    color: #ffffff;
    border: none;
    border-radius: 3px;
    padding: 6px 16px;
    font-weight: bold;
    min-height: 24px;
}
QPushButton:hover   { background-color: #1177bb; }
QPushButton:pressed { background-color: #0d5689; }
QPushButton:disabled {
    background-color: #3c3c3c;
    color: #6c6c6c;
}
QPushButton#btn_start       { background-color: #388e3c; }
QPushButton#btn_start:hover { background-color: #43a047; }
QPushButton#btn_stop        { background-color: #d32f2f; }
QPushButton#btn_stop:hover  { background-color: #e53935; }
QPushButton#btn_run         { background-color: #f57c00; }
QPushButton#btn_run:hover   { background-color: #fb8c00; }

QTextEdit {
    background-color: #1e1e1e;
    border: 1px solid #3c3c3c;
    border-radius: 3px;
    color: #d4d4d4;
    font-family: "Consolas", "Courier New", monospace;
    font-size: 9pt;
}
QTreeWidget {
    background-color: #252526;
    border: 1px solid #3c3c3c;
    border-radius: 3px;
    color: #d4d4d4;
    alternate-background-color: #2a2d2e;
}
QTreeWidget::item:hover    { background-color: #2a2d2e; }
QTreeWidget::item:selected { background-color: #094771; }
QTreeWidget::indicator:unchecked {
    border: 1px solid #555;
    background-color: #2d2d2d;
}
QTreeWidget::indicator:checked {
    border: 1px solid #569cd6;
    background-color: #569cd6;
}
QHeaderView::section {
    background-color: #252526;
    color: #d4d4d4;
    padding: 4px;
    border: 1px solid #3c3c3c;
}
QProgressBar {
    border: 1px solid #3c3c3c;
    border-radius: 3px;
    background-color: #2d2d2d;
    text-align: center;
    color: #d4d4d4;
    height: 22px;
}
QProgressBar::chunk {
    background-color: #569cd6;
    border-radius: 2px;
}
QStatusBar {
    background-color: #007acc;
    color: #ffffff;
    font-size: 9pt;
}
QStatusBar::item { border: none; }
QMenuBar {
    background-color: #2d2d2d;
    color: #d4d4d4;
    border-bottom: 1px solid #3c3c3c;
}
QMenuBar::item:selected { background-color: #094771; }
QMenu {
    background-color: #252526;
    color: #d4d4d4;
    border: 1px solid #3c3c3c;
}
QMenu::item:selected { background-color: #094771; }
QSplitter::handle {
    background-color: #3c3c3c;
    width: 2px;
}
"""


# ==========================================================================
# COM Event Handlers  (instantiated in the worker thread)
# ==========================================================================


class AppEventHandler:
    """Handles CANoe.Application events."""

    def __init__(self):
        self.worker = None

    def OnOpen(self, fullname):
        if self.worker:
            self.worker.log_message.emit(f"Configuration opened: {fullname}", "info")

    def OnQuit(self):
        if self.worker:
            self.worker.log_message.emit("CANoe application quit", "warning")
            self.worker.canoe_quit.emit()


class MeasEventHandler:
    """Handles CANoe.Measurement events."""

    def __init__(self):
        self.worker = None
        self._started = False
        self._stopped = False

    @property
    def is_started(self):
        return self._started

    @property
    def is_stopped(self):
        return self._stopped

    def OnInit(self):
        if self.worker:
            self.worker.log_message.emit("Measurement initializing...", "info")

    def OnStart(self):
        self._started = True
        self._stopped = False
        if self.worker:
            self.worker.log_message.emit("Measurement started", "info")
            self.worker.measurement_state.emit(True)

    def OnStop(self):
        self._started = False
        self._stopped = True
        if self.worker:
            self.worker.log_message.emit("Measurement stopped", "info")
            self.worker.measurement_state.emit(False)

    def OnExit(self):
        if self.worker:
            self.worker.log_message.emit("Measurement exited", "info")


class TestEventHandler:
    """Handles CANoe TestModule / TestConfiguration events."""

    def __init__(self):
        self.worker = None
        self.started = False
        self.stopped = False
        self.verdict = VERDICT_NA

    def OnStart(self):
        self.started = True
        self.stopped = False
        self.verdict = VERDICT_NA
        if self.worker:
            self.worker.log_message.emit(f"Test '{self.Name}' started", "info")
            self.worker.test_item_started.emit(self.Name)

    def OnStop(self, reason):
        self.started = False
        self.stopped = True
        self.verdict = reason
        if self.worker:
            tag = {0: "N/A", 1: "PASSED", 2: "FAILED"}.get(reason, f"Unknown({reason})")
            lvl = "info" if reason == 1 else ("error" if reason == 2 else "warning")
            self.worker.log_message.emit(
                f"Test '{self.Name}' stopped  -  Verdict: {tag}", lvl
            )
            self.worker.test_item_finished.emit(self.Name, reason)


# ==========================================================================
# CANoe Worker  (lives in a dedicated QThread)
# ==========================================================================


class CANoeWorker(QObject):
    """All CANoe COM operations happen here, off the UI thread."""

    # --- signals to UI ---
    log_message = pyqtSignal(str, str)  # (msg, level)
    state_changed = pyqtSignal(str)  # state name
    canoe_connected = pyqtSignal(str)  # version string
    canoe_quit = pyqtSignal()
    measurement_state = pyqtSignal(bool)  # running?
    test_modules_found = pyqtSignal(list)  # [(name, enabled), ...]
    test_configs_found = pyqtSignal(list)
    test_item_started = pyqtSignal(str)  # name
    test_item_finished = pyqtSignal(str, int)  # name, verdict
    all_tests_complete = pyqtSignal(int, int)  # passed, failed
    progress = pyqtSignal(int, int)  # done, total
    verdict_update = pyqtSignal(str)  # led state key
    report_available = pyqtSignal(str)  # path (global)
    test_report_found = pyqtSignal(str, str)  # test_name, report_path
    write_window_text = pyqtSignal(str)  # new Write Window content

    def __init__(self):
        super().__init__()
        self._app_com = None
        self._meas_com = None
        self._meas_ev = None
        self._app_ev = None
        self._test_modules = []
        self._test_configs = []
        self._config_dir = ""
        self._pump_timer = None
        self._write_poll_timer = None
        self._last_write_text = ""
        self._connected = False

    # ---- thread bootstrap ------------------------------------------------

    @pyqtSlot()
    def initialize(self):
        """Called once when the thread starts."""
        if not HAS_COM:
            self.log_message.emit("pywin32 not installed - COM disabled", "error")
            return
        try:
            pythoncom.CoInitialize()
            self._pump_timer = QTimer()
            self._pump_timer.timeout.connect(self._pump)
            self._pump_timer.start(50)
            self._write_poll_timer = QTimer()
            self._write_poll_timer.timeout.connect(self._poll_write_window)
            self.log_message.emit("COM initialised. Ready.", "info")
        except Exception as e:
            self.log_message.emit(f"COM init failed: {e}", "error")

    @pyqtSlot()
    def _pump(self):
        try:
            pythoncom.PumpWaitingMessages()
        except Exception:
            pass

    @pyqtSlot()
    def _poll_write_window(self):
        """Read CANoe Write Window and emit new content."""
        if not self._connected or not self._app_com:
            return
        try:
            text = self._app_com.UI.Write.Text
            if text is None:
                text = ""
            if text != self._last_write_text:
                if text.startswith(self._last_write_text):
                    new_part = text[len(self._last_write_text) :]
                else:
                    new_part = text
                if new_part.strip():
                    self.write_window_text.emit(new_part)
                self._last_write_text = text
        except Exception:
            pass

    @pyqtSlot()
    def reset_write_cache(self):
        """Reset cached text so next poll re-syncs from current state."""
        self._last_write_text = ""

    # ---- helpers ---------------------------------------------------------

    def _do_events(self):
        pythoncom.PumpWaitingMessages()
        time.sleep(0.1)

    def _wait_for(self, cond, timeout=30):
        t0 = time.time()
        while not cond():
            if time.time() - t0 > timeout:
                return False
            self._do_events()
        return True

    # ---- public slots (called via signals from UI) -----------------------

    @pyqtSlot(str)
    def connect_and_load(self, cfg_path):
        if not HAS_COM:
            self.log_message.emit("COM unavailable", "error")
            return
        try:
            self.state_changed.emit("connecting")
            self.log_message.emit("Connecting to CANoe COM Server...", "info")

            self._app_com = DispatchEx("CANoe.Application")
            ver = self._app_com.Version
            ver_str = f"{ver.major}.{ver.minor}.{ver.Build}"
            self.log_message.emit(f"CANoe {ver_str} loaded", "info")

            # event handlers
            self._app_ev = WithEvents(self._app_com, AppEventHandler)
            self._app_ev.worker = self
            self._meas_com = self._app_com.Measurement
            self._meas_ev = WithEvents(self._meas_com, MeasEventHandler)
            self._meas_ev.worker = self

            # open config
            self._app_com.Configuration.Modified = False
            cfg_abs = os.path.abspath(cfg_path)
            self._config_dir = os.path.dirname(cfg_abs)
            self.log_message.emit(f"Opening: {cfg_abs}", "info")
            self._app_com.Open(cfg_abs)

            self._connected = True
            self._test_modules.clear()
            self._test_configs.clear()
            self._last_write_text = ""
            # Start Write Window polling (500ms interval)
            if self._write_poll_timer and not self._write_poll_timer.isActive():
                self._write_poll_timer.start(500)
            self.canoe_connected.emit(ver_str)
            self.state_changed.emit("connected")
            self.log_message.emit("Configuration loaded successfully", "info")

            # Auto-scan test environments already loaded in CANoe
            self._scan_existing_test_envs()

        except Exception as e:
            self.log_message.emit(f"Connection failed: {e}", "error")
            self.state_changed.emit("error")
            self._connected = False

    def _scan_existing_test_envs(self):
        """Scan CANoe for already-loaded test environments and populate tree."""
        try:
            ts = self._app_com.Configuration.TestSetup
            envs = ts.TestEnvironments
            if envs.Count == 0:
                return
            self._test_modules.clear()
            for i in range(1, envs.Count + 1):
                try:
                    tenv = envs.Item(i)
                    tenv = CastTo(tenv, "ITestEnvironment2")
                    self._collect_modules(tenv)
                except Exception:
                    pass

            # Pump and refresh names
            for _ in range(30):
                self._do_events()
            self._refresh_module_names()

            if self._test_modules:
                info = [(m["name"], m["enabled"]) for m in self._test_modules]
                self.test_modules_found.emit(info)
                self.log_message.emit(
                    f"Found {len(self._test_modules)} existing test module(s) - all enabled",
                    "info",
                )
                # Auto-scan for existing reports
                self._check_reports()
        except Exception as e:
            self.log_message.emit(f"Auto-scan test envs: {e}", "warning")

    @pyqtSlot(str)
    def load_test_setup(self, tse_path):
        if not self._connected:
            self.log_message.emit("Not connected to CANoe", "error")
            return
        try:
            self.log_message.emit(f"Loading test setup: {tse_path}", "info")
            ts = self._app_com.Configuration.TestSetup
            tse_abs = os.path.abspath(tse_path)

            # Try to add the TSE; if already loaded, scan existing instead
            tenv = None
            try:
                tenv = ts.TestEnvironments.Add(tse_abs)
                tenv = CastTo(tenv, "ITestEnvironment2")
            except Exception as add_err:
                self.log_message.emit(
                    f"TSE may already be loaded, scanning existing environments",
                    "warning",
                )

            self._test_modules.clear()
            if tenv is not None:
                self._collect_modules(tenv)
            else:
                # TSE already loaded - scan all existing environments
                envs = ts.TestEnvironments
                for i in range(1, envs.Count + 1):
                    try:
                        existing = envs.Item(i)
                        existing = CastTo(existing, "ITestEnvironment2")
                        self._collect_modules(existing)
                    except Exception:
                        pass

            # Pump COM messages extensively so CANoe finishes processing
            for _ in range(50):
                self._do_events()

            # Refresh names from COM objects
            self._refresh_module_names()

            info = [(m["name"], m["enabled"]) for m in self._test_modules]
            self.test_modules_found.emit(info)
            self.log_message.emit(
                f"Found {len(self._test_modules)} test module(s) - all enabled in CANoe",
                "info",
            )
            # Auto-scan for existing reports
            self._check_reports()
        except Exception as e:
            self.log_message.emit(f"Load test setup failed: {e}", "error")

    def _collect_modules(self, parent):
        for tm in parent.TestModules:
            # Enable the test module in CANoe (so it gets compiled/built)
            try:
                tm.Enabled = True
            except Exception:
                pass
            ev = DispatchWithEvents(tm, TestEventHandler)
            ev.worker = self
            self._test_modules.append(
                {
                    "name": tm.Name,
                    "enabled": True,
                    "obj": tm,
                    "events": ev,
                    "parent": parent,  # Store parent for re-enumeration
                }
            )
        for folder in parent.Folders:
            self._collect_modules(folder)

    def _refresh_module_names(self):
        """Re-read module names from COM after pumping messages.

        Note: The display name in CANoe may be the CAPL file name,
        but this is not directly accessible via COM API.
        If needed, names can be manually corrected via the UI.
        """
        # Currently just uses the Name property
        # User can manually update test case names if needed
        pass

    @pyqtSlot()
    def start_measurement(self):
        if not self._connected:
            self.log_message.emit("Not connected", "error")
            return
        try:
            if self._meas_com.Running:
                self.log_message.emit("Measurement already running", "warning")
                self.state_changed.emit("running")
                self.measurement_state.emit(True)
                return

            self.state_changed.emit("starting")
            self.log_message.emit("Starting measurement...", "info")
            self.verdict_update.emit("idle")

            self._meas_ev._started = False
            self._meas_com.Start()

            if self._wait_for(lambda: self._meas_ev.is_started, 30):
                self.state_changed.emit("running")
            else:
                self.log_message.emit("Timeout: measurement did not start", "error")
                self.state_changed.emit("connected")
        except Exception as e:
            self.log_message.emit(f"Start failed: {e}", "error")
            self.state_changed.emit("error")

    @pyqtSlot()
    def stop_measurement(self):
        if not self._connected:
            return
        try:
            if not self._meas_com.Running:
                self.log_message.emit("Measurement not running", "warning")
                self.state_changed.emit("connected")
                self.measurement_state.emit(False)
                return

            self.state_changed.emit("stopping")
            self.log_message.emit("Stopping measurement...", "info")

            self._meas_ev._stopped = False
            self._meas_com.Stop()

            if self._wait_for(lambda: self._meas_ev.is_stopped, 30):
                self.state_changed.emit("connected")
            else:
                self.log_message.emit("Timeout: measurement did not stop", "error")
        except Exception as e:
            self.log_message.emit(f"Stop failed: {e}", "error")
            self.state_changed.emit("error")

    @pyqtSlot(list)
    def run_selected_tests(self, selected_names):
        try:
            self.state_changed.emit("testing")
            self.verdict_update.emit("running")

            items = [m for m in self._test_modules if m["name"] in selected_names]
            items += [c for c in self._test_configs if c["name"] in selected_names]

            if not items:
                self.log_message.emit(
                    "No test items found matching selection", "warning"
                )
                self.state_changed.emit("running")
                return

            total = len(items)
            self.progress.emit(0, total)
            self.log_message.emit(f"Running {total} test(s)...", "info")

            for it in items:
                it["events"].started = False
                it["events"].stopped = False
                it["events"].verdict = VERDICT_NA

            for it in items:
                try:
                    self.log_message.emit(f"Starting test: {it['name']}", "info")
                    it["obj"].Start()
                    if not self._wait_for(lambda i=it: i["events"].started, 30):
                        self.log_message.emit(
                            f"Timeout waiting for '{it['name']}' to start", "warning"
                        )
                except Exception as e:
                    self.log_message.emit(
                        f"Failed to start '{it['name']}': {e}", "error"
                    )
                    it["events"].stopped = True
                    it["events"].verdict = VERDICT_NA

            # Wait for all items to finish
            while True:
                done = sum(1 for i in items if i["events"].stopped)
                self.progress.emit(done, total)
                if done >= total:
                    break
                self._do_events()

            passed = sum(1 for i in items if i["events"].verdict == VERDICT_PASSED)
            failed = sum(1 for i in items if i["events"].verdict == VERDICT_FAILED)

            self.progress.emit(total, total)
            self.all_tests_complete.emit(passed, failed)

            if failed > 0:
                self.verdict_update.emit("fail")
                self.log_message.emit(
                    f"Done: {passed} passed, {failed} failed", "error"
                )
            else:
                self.verdict_update.emit("pass")
                self.log_message.emit(f"Done: all {total} test(s) passed", "info")

            self.state_changed.emit("running")
            self._check_reports()

        except Exception as e:
            self.log_message.emit(f"Test run error: {e}", "error")
            self.verdict_update.emit("error")
            self.state_changed.emit("running")

    def _check_reports(self):
        """Search for per-test reports and a global latest report."""
        if not self._config_dir:
            return

        # Collect candidate report directories
        report_dirs = []
        for sub in (
            "TestReports",
            "Reports",
            "TestModules_Report",
            "TestConfiguration_Report",
            "Report",
        ):
            d = os.path.join(self._config_dir, sub)
            if os.path.isdir(d):
                report_dirs.append(d)
                self.log_message.emit(f"Scanning report directory: {d}", "info")

        if not report_dirs:
            self.log_message.emit("No report directories found", "warning")
            return

        # Collect ALL report files first
        all_report_files = []
        for d in report_dirs:
            try:
                files = os.listdir(d)
                for f in files:
                    fname_no_ext = os.path.splitext(f)[0]
                    ext_lower = f.lower()

                    # Find all files with valid extension (vtestreport, html, xml)
                    # and containing "report" keyword (flexible: _report, -report, report, etc.)
                    if ("report" in fname_no_ext.lower()) and (
                        ext_lower.endswith(".vtestreport")
                        or ext_lower.endswith(".html")
                        or ext_lower.endswith(".htm")
                        or ext_lower.endswith(".xml")
                    ):
                        full_path = os.path.join(d, f)
                        mtime = os.path.getmtime(full_path)
                        all_report_files.append((fname_no_ext, full_path, mtime))
                        self.log_message.emit(f"Found report file: {f}", "info")
            except Exception as e:
                self.log_message.emit(f"Error scanning {d}: {e}", "warning")

        # Per-test report search - try to match with test names
        all_items = self._test_modules + self._test_configs
        for it in all_items:
            name = it["name"]
            candidates = []

            # Match strategy: Find reports that match the test module name exactly
            for fname_no_ext, full_path, mtime in all_report_files:
                # Remove "_report" or " report" suffix from filename to get base name
                base_name = fname_no_ext
                if base_name.endswith("_report"):
                    base_name = base_name[:-7]  # Remove "_report"
                elif base_name.endswith(" report"):
                    base_name = base_name[:-7]  # Remove " report"
                elif base_name.endswith("-report"):
                    base_name = base_name[:-7]  # Remove "-report"

                # Exact match: base name must equal test name
                if base_name == name:
                    candidates.append((mtime, full_path))

            # Use most recent report if multiple found
            if candidates:
                candidates.sort(reverse=True)  # Sort by mtime, newest first
                report_path = candidates[0][1]
                self.test_report_found.emit(name, report_path)
                self.log_message.emit(
                    f"Mapped report for '{name}': {os.path.basename(report_path)}",
                    "info",
                )
            else:
                self.log_message.emit(
                    f"No matching report found for '{name}'", "warning"
                )

        # Global: emit the most recent report file
        all_reports = []
        for d in report_dirs:
            try:
                for f in os.listdir(d):
                    if f.lower().endswith((".vtestreport", ".html", ".htm", ".xml")):
                        full_path = os.path.join(d, f)
                        mtime = os.path.getmtime(full_path)
                        all_reports.append((mtime, full_path))
            except Exception:
                pass

        if all_reports:
            all_reports.sort(reverse=True)
            self.report_available.emit(all_reports[0][1])

    @pyqtSlot()
    def cleanup(self):
        try:
            if self._write_poll_timer:
                self._write_poll_timer.stop()
            if self._pump_timer:
                self._pump_timer.stop()
            if self._connected and self._meas_com:
                try:
                    if self._meas_com.Running:
                        self._meas_com.Stop()
                        time.sleep(2)
                except Exception:
                    pass
            if HAS_COM:
                try:
                    pythoncom.CoUninitialize()
                except Exception:
                    pass
            self._connected = False
        except Exception:
            pass


# ==========================================================================
# Main Window
# ==========================================================================


class MainWindow(QMainWindow):
    """Professional GUI for CANoe Automation Testing."""

    # signals to worker
    sig_connect = pyqtSignal(str)
    sig_load_tse = pyqtSignal(str)
    sig_start = pyqtSignal()
    sig_stop = pyqtSignal()
    sig_run_tests = pyqtSignal(list)
    sig_cleanup = pyqtSignal()
    sig_clear_write = pyqtSignal()

    def __init__(self):
        super().__init__()
        # Set window icon from logo
        if os.path.isfile(LOGO_PATH):
            icon = QIcon(LOGO_PATH)
            self.setWindowIcon(icon)

        self.setWindowTitle(f"{APP_TITLE}  v{VERSION}")
        self.setMinimumSize(950, 700)
        self.resize(1100, 820)

        self._state = "disconnected"
        self._canoe_ver = ""
        self._report_path = ""

        # Persistent settings
        self._settings = QSettings("BLTN", "CANoeTestTool")

        self._build_ui()
        self._build_menu()
        self._build_statusbar()
        self._setup_worker()
        self._update_buttons("disconnected")

        # Restore saved paths
        saved_cfg = self._settings.value("last_cfg_path", "")
        saved_tse = self._settings.value("last_tse_path", "")
        if saved_cfg:
            self.ed_cfg.setText(saved_cfg)
        if saved_tse:
            self.ed_tse.setText(saved_tse)

    # ------------------------------------------------------------------
    # UI construction
    # ------------------------------------------------------------------

    def _build_ui(self):
        central = QWidget()
        self.setCentralWidget(central)
        root = QVBoxLayout(central)
        root.setSpacing(6)
        root.setContentsMargins(8, 8, 8, 8)

        # ---- CONNECTION section ----
        grp_conn = QGroupBox("CONNECTION")
        lo_conn = QVBoxLayout(grp_conn)

        row_cfg = QHBoxLayout()
        row_cfg.addWidget(QLabel("Config (.cfg):"))
        self.ed_cfg = QLineEdit()
        self.ed_cfg.setPlaceholderText("Select CANoe configuration file...")
        row_cfg.addWidget(self.ed_cfg, 1)
        self.btn_browse_cfg = QPushButton("Browse...")
        self.btn_browse_cfg.clicked.connect(self._browse_cfg)
        row_cfg.addWidget(self.btn_browse_cfg)
        self.btn_connect = QPushButton("Connect && Load")
        self.btn_connect.clicked.connect(self._do_connect)
        row_cfg.addWidget(self.btn_connect)
        lo_conn.addLayout(row_cfg)

        row_tse = QHBoxLayout()
        row_tse.addWidget(QLabel("Test Setup (.tse):"))
        self.ed_tse = QLineEdit()
        self.ed_tse.setPlaceholderText("Select test environment file (optional)...")
        row_tse.addWidget(self.ed_tse, 1)
        self.btn_browse_tse = QPushButton("Browse...")
        self.btn_browse_tse.clicked.connect(self._browse_tse)
        row_tse.addWidget(self.btn_browse_tse)
        self.btn_load_tse = QPushButton("Load TSE")
        self.btn_load_tse.clicked.connect(self._do_load_tse)
        row_tse.addWidget(self.btn_load_tse)
        lo_conn.addLayout(row_tse)

        self.lbl_conn = QLabel("Status: Disconnected")
        self.lbl_conn.setStyleSheet("color:#6c6c6c; font-weight:bold;")
        lo_conn.addWidget(self.lbl_conn)

        root.addWidget(grp_conn)

        # ---- middle: vertical splitter (tree+ctrl | console) ----
        v_split = QSplitter(Qt.Orientation.Vertical)

        # -- horizontal splitter: test tree | control panel --
        h_split = QSplitter(Qt.Orientation.Horizontal)

        # left: test tree
        grp_tree = QGroupBox("TEST SELECTION")
        lo_tree = QVBoxLayout(grp_tree)
        self.tree = QTreeWidget()
        self.tree.setHeaderLabels(["Test Item", "Status", "Report"])
        self.tree.setColumnCount(3)
        hdr = self.tree.header()
        hdr.setStretchLastSection(False)
        hdr.setSectionResizeMode(0, QHeaderView.ResizeMode.Stretch)
        hdr.setSectionResizeMode(1, QHeaderView.ResizeMode.ResizeToContents)
        hdr.setSectionResizeMode(
            2, QHeaderView.ResizeMode.Interactive
        )  # Allow manual resize
        self.tree.setColumnWidth(2, 150)  # Set initial width for Report column
        self.tree.setAlternatingRowColors(True)
        self.tree.itemDoubleClicked.connect(
            self._on_tree_double_clicked
        )  # Double-click to open
        lo_tree.addWidget(self.tree)

        row_sel = QHBoxLayout()
        b_all = QPushButton("Select All")
        b_all.clicked.connect(self._select_all)
        b_none = QPushButton("Deselect All")
        b_none.clicked.connect(self._deselect_all)
        row_sel.addWidget(b_all)
        row_sel.addWidget(b_none)
        row_sel.addStretch()
        lo_tree.addLayout(row_sel)

        h_split.addWidget(grp_tree)

        # right: control & status
        grp_ctrl = QGroupBox("CONTROL && STATUS")
        lo_ctrl = QVBoxLayout(grp_ctrl)
        lo_ctrl.setSpacing(12)

        self.btn_start = QPushButton("START MEASUREMENT")
        self.btn_start.setObjectName("btn_start")
        self.btn_start.setMinimumHeight(44)
        self.btn_start.clicked.connect(self._do_start)
        lo_ctrl.addWidget(self.btn_start)

        self.btn_stop = QPushButton("STOP MEASUREMENT")
        self.btn_stop.setObjectName("btn_stop")
        self.btn_stop.setMinimumHeight(44)
        self.btn_stop.clicked.connect(self._do_stop)
        lo_ctrl.addWidget(self.btn_stop)

        lo_ctrl.addSpacing(8)

        self.btn_run = QPushButton("RUN SELECTED TESTS")
        self.btn_run.setObjectName("btn_run")
        self.btn_run.setMinimumHeight(44)
        self.btn_run.clicked.connect(self._do_run_tests)
        lo_ctrl.addWidget(self.btn_run)

        lo_ctrl.addSpacing(16)

        # verdict LED
        row_led = QHBoxLayout()
        row_led.addWidget(QLabel("Verdict:"))
        self.led = QLabel()
        self.led.setFixedSize(24, 24)
        self._set_led("idle")
        row_led.addWidget(self.led)
        self.lbl_verdict = QLabel("IDLE")
        self.lbl_verdict.setStyleSheet(
            "font-weight:bold; font-size:13pt; color:#6c6c6c;"
        )
        row_led.addWidget(self.lbl_verdict)
        row_led.addStretch()
        lo_ctrl.addLayout(row_led)

        self.lbl_meas = QLabel("Measurement: Stopped")
        self.lbl_meas.setStyleSheet("font-size:10pt; color:#6c6c6c;")
        lo_ctrl.addWidget(self.lbl_meas)

        lo_ctrl.addStretch()
        h_split.addWidget(grp_ctrl)
        h_split.setSizes([550, 300])

        v_split.addWidget(h_split)

        # ---- console + write window side by side ----
        console_split = QSplitter(Qt.Orientation.Horizontal)

        # left: console log
        grp_con = QGroupBox("CONSOLE LOG")
        lo_con = QVBoxLayout(grp_con)
        self.console = QTextEdit()
        self.console.setReadOnly(True)
        lo_con.addWidget(self.console)

        row_con = QHBoxLayout()
        b_clear = QPushButton("Clear")
        b_clear.clicked.connect(self.console.clear)
        row_con.addWidget(b_clear)
        row_con.addStretch()
        lo_con.addLayout(row_con)

        console_split.addWidget(grp_con)

        # right: write window (mirrors CANoe Write Window output)
        grp_write = QGroupBox("WRITE WINDOW")
        lo_write = QVBoxLayout(grp_write)
        self.write_win = QTextEdit()
        self.write_win.setReadOnly(True)
        lo_write.addWidget(self.write_win)

        row_wr = QHBoxLayout()
        b_clear_wr = QPushButton("Clear")
        b_clear_wr.clicked.connect(self._clear_write_window)
        row_wr.addWidget(b_clear_wr)
        row_wr.addStretch()
        lo_write.addLayout(row_wr)

        console_split.addWidget(grp_write)
        console_split.setSizes([500, 500])

        v_split.addWidget(console_split)
        v_split.setSizes([420, 250])

        root.addWidget(v_split, 1)

        # ---- bottom: progress + report ----
        row_bot = QHBoxLayout()
        self.progress = QProgressBar()
        self.progress.setRange(0, 100)
        self.progress.setValue(0)
        self.progress.setFormat("Idle")
        row_bot.addWidget(self.progress, 1)

        self.btn_report = QPushButton("Open Report")
        self.btn_report.setEnabled(False)
        self.btn_report.clicked.connect(self._open_report)
        row_bot.addWidget(self.btn_report)

        root.addLayout(row_bot)

    def _build_menu(self):
        mb = self.menuBar()

        m_file = mb.addMenu("File")
        a = QAction("Open Config...", self)
        a.setShortcut("Ctrl+O")
        a.triggered.connect(self._browse_cfg)
        m_file.addAction(a)
        m_file.addSeparator()
        a = QAction("Exit", self)
        a.setShortcut("Ctrl+Q")
        a.triggered.connect(self.close)
        m_file.addAction(a)

        m_tools = mb.addMenu("Tools")
        a = QAction("Clear Console", self)
        a.triggered.connect(self.console.clear)
        m_tools.addAction(a)
        a = QAction("Clear Write Window", self)
        a.triggered.connect(self._clear_write_window)
        m_tools.addAction(a)

        m_help = mb.addMenu("Help")
        a = QAction("About", self)
        a.triggered.connect(self._show_about)
        m_help.addAction(a)

    def _build_statusbar(self):
        sb = self.statusBar()
        self.sb_ver = QLabel("  CANoe: N/A")
        self.sb_state = QLabel("Disconnected  ")
        sb.addWidget(self.sb_ver, 1)
        sb.addPermanentWidget(self.sb_state)

    # ------------------------------------------------------------------
    # Worker thread setup
    # ------------------------------------------------------------------

    def _setup_worker(self):
        self._thread = QThread()
        self._worker = CANoeWorker()
        self._worker.moveToThread(self._thread)

        # thread start -> worker init
        self._thread.started.connect(self._worker.initialize)

        # UI -> Worker
        self.sig_connect.connect(self._worker.connect_and_load)
        self.sig_load_tse.connect(self._worker.load_test_setup)
        self.sig_start.connect(self._worker.start_measurement)
        self.sig_stop.connect(self._worker.stop_measurement)
        self.sig_run_tests.connect(self._worker.run_selected_tests)
        self.sig_cleanup.connect(self._worker.cleanup)
        self.sig_clear_write.connect(self._worker.reset_write_cache)

        # Worker -> UI
        self._worker.log_message.connect(self._on_log)
        self._worker.state_changed.connect(self._update_buttons)
        self._worker.canoe_connected.connect(self._on_connected)
        self._worker.canoe_quit.connect(lambda: self._update_buttons("disconnected"))
        self._worker.measurement_state.connect(self._on_meas_state)
        self._worker.test_modules_found.connect(self._populate_modules)
        self._worker.test_configs_found.connect(self._populate_configs)
        self._worker.test_item_started.connect(self._tree_set_running)
        self._worker.test_item_finished.connect(self._tree_set_verdict)
        self._worker.all_tests_complete.connect(self._on_tests_done)
        self._worker.progress.connect(self._on_progress)
        self._worker.verdict_update.connect(self._on_verdict)
        self._worker.report_available.connect(self._on_report)
        self._worker.test_report_found.connect(self._on_test_report)
        self._worker.write_window_text.connect(self._on_write_window)

        self._thread.start()

    # ------------------------------------------------------------------
    # UI event handlers (user clicks)
    # ------------------------------------------------------------------

    def _browse_cfg(self):
        p, _ = QFileDialog.getOpenFileName(
            self,
            "Select CANoe Config",
            self.ed_cfg.text() or "",
            "CANoe Config (*.cfg);;All Files (*)",
        )
        if p:
            self.ed_cfg.setText(p)

    def _browse_tse(self):
        start = self.ed_tse.text() or os.path.dirname(self.ed_cfg.text()) or ""
        p, _ = QFileDialog.getOpenFileName(
            self, "Select Test Setup", start, "Test Setup (*.tse);;All Files (*)"
        )
        if p:
            self.ed_tse.setText(p)

    def _do_connect(self):
        p = self.ed_cfg.text().strip()
        if not p:
            QMessageBox.warning(self, "Warning", "Please select a .cfg file first.")
            return
        if not os.path.isfile(p):
            QMessageBox.warning(self, "Warning", f"File not found:\n{p}")
            return
        self._save_paths()
        self.sig_connect.emit(p)

    def _do_load_tse(self):
        p = self.ed_tse.text().strip()
        if not p:
            QMessageBox.warning(self, "Warning", "Please select a .tse file first.")
            return
        self._save_paths()
        self.sig_load_tse.emit(p)

    def _do_start(self):
        self.sig_start.emit()

    def _do_stop(self):
        self.sig_stop.emit()

    def _do_run_tests(self):
        sel = self._get_checked()
        if not sel:
            QMessageBox.information(
                self, "Info", "No tests selected. Check items in the tree first."
            )
            return
        self.sig_run_tests.emit(sel)

    def _get_checked(self):
        names = []
        root = self.tree.invisibleRootItem()
        for i in range(root.childCount()):
            grp = root.child(i)
            for j in range(grp.childCount()):
                item = grp.child(j)
                if item.checkState(0) == Qt.CheckState.Checked:
                    names.append(item.text(0))
        return names

    def _select_all(self):
        root = self.tree.invisibleRootItem()
        for i in range(root.childCount()):
            g = root.child(i)
            for j in range(g.childCount()):
                g.child(j).setCheckState(0, Qt.CheckState.Checked)

    def _deselect_all(self):
        root = self.tree.invisibleRootItem()
        for i in range(root.childCount()):
            g = root.child(i)
            for j in range(g.childCount()):
                g.child(j).setCheckState(0, Qt.CheckState.Unchecked)

    def _on_tree_double_clicked(self, item, column):
        """Handle tree item double-click - open report if Report column double-clicked."""
        if column == 2:
            path = item.data(2, Qt.ItemDataRole.UserRole)
            if path and os.path.isfile(path):
                try:
                    os.startfile(path)
                    self._on_log(f"Opening report: {os.path.basename(path)}", "info")
                except Exception as e:
                    QMessageBox.warning(
                        self, "Error", f"Cannot open report file:\n{path}\n\nError: {e}"
                    )

    def _open_report(self):
        if self._report_path and os.path.isfile(self._report_path):
            os.startfile(self._report_path)
        else:
            QMessageBox.information(self, "Info", "No report file available yet.")

    def _clear_write_window(self):
        """Clear the Write Window display and reset worker's cache."""
        self.write_win.clear()
        self.sig_clear_write.emit()

    def _save_paths(self):
        """Persist current file paths to QSettings."""
        self._settings.setValue("last_cfg_path", self.ed_cfg.text().strip())
        self._settings.setValue("last_tse_path", self.ed_tse.text().strip())

    def _show_about(self):
        QMessageBox.about(
            self,
            "About",
            f"<b>{APP_TITLE}</b><br>Version {VERSION}<br><br>"
            "Professional CANoe Automation Test GUI<br>"
            "Built with PyQt6 + CANoe COM API<br><br>"
            "Architecture: UI Thread  &harr;  Worker Thread  &harr;  "
            "CANoe COM Server",
        )

    # ------------------------------------------------------------------
    # Worker signal handlers
    # ------------------------------------------------------------------

    def _on_log(self, msg, level):
        ts = datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]
        c = {"info": "#d4d4d4", "warning": "#dcdcaa", "error": "#f44747"}.get(
            level, "#d4d4d4"
        )
        pfx = {"warning": "WARNING: ", "error": "ERROR: "}.get(level, "")
        html = (
            f'<span style="color:#6a9955">[{ts}]</span> '
            f'<span style="color:{c}">{pfx}{msg}</span>'
        )
        self.console.append(html)
        cur = self.console.textCursor()
        cur.movePosition(QTextCursor.MoveOperation.End)
        self.console.setTextCursor(cur)

    def _on_connected(self, ver):
        self._canoe_ver = ver
        self.lbl_conn.setText(f"Status: Connected  (CANoe {ver})")
        self.lbl_conn.setStyleSheet("color:#4caf50; font-weight:bold;")
        self.sb_ver.setText(f"  CANoe: {ver}")

    def _on_meas_state(self, running):
        """Sync UI with measurement state (handles external CANoe changes)."""
        if running:
            self.lbl_meas.setText("Measurement: RUNNING")
            self.lbl_meas.setStyleSheet(
                "font-size:10pt; color:#4caf50; font-weight:bold;"
            )
            # Update buttons unless the tool itself is managing the transition
            if self._state not in ("starting", "testing"):
                self._update_buttons("running")
        else:
            self.lbl_meas.setText("Measurement: STOPPED")
            self.lbl_meas.setStyleSheet(
                "font-size:10pt; color:#6c6c6c; font-weight:bold;"
            )
            if self._state not in ("stopping", "testing", "connecting"):
                self._update_buttons("connected")

    def _populate_modules(self, modules):
        grp = self._get_or_create_group("Test Modules")
        while grp.childCount():
            grp.removeChild(grp.child(0))
        for name, enabled in modules:
            ch = QTreeWidgetItem(grp, [name, "Idle", ""])
            # Don't auto-check: UI checkbox = select to run (different from CANoe Enabled)
            ch.setCheckState(0, Qt.CheckState.Unchecked)
            ch.setForeground(1, QColor("#6c6c6c"))

    def _populate_configs(self, configs):
        grp = self._get_or_create_group("Test Configurations")
        while grp.childCount():
            grp.removeChild(grp.child(0))
        for name, enabled in configs:
            ch = QTreeWidgetItem(grp, [name, "Idle", ""])
            ch.setCheckState(
                0, Qt.CheckState.Checked if enabled else Qt.CheckState.Unchecked
            )
            ch.setForeground(1, QColor("#6c6c6c"))

    def _get_or_create_group(self, label):
        for i in range(self.tree.topLevelItemCount()):
            if self.tree.topLevelItem(i).text(0) == label:
                return self.tree.topLevelItem(i)
        item = QTreeWidgetItem(self.tree, [label, "", ""])
        item.setExpanded(True)
        f = item.font(0)
        f.setBold(True)
        item.setFont(0, f)
        return item

    def _tree_set_running(self, name):
        self._tree_status(name, "Running", "#569cd6")

    def _tree_set_verdict(self, name, verdict):
        m = {0: ("N/A", "#6c6c6c"), 1: ("PASSED", "#4caf50"), 2: ("FAILED", "#f44336")}
        txt, col = m.get(verdict, ("?", "#ff9800"))
        self._tree_status(name, txt, col)

    def _tree_status(self, name, status, color):
        root = self.tree.invisibleRootItem()
        for i in range(root.childCount()):
            g = root.child(i)
            for j in range(g.childCount()):
                it = g.child(j)
                if it.text(0) == name:
                    it.setText(1, status)
                    it.setForeground(1, QColor(color))
                    return

    def _on_tests_done(self, passed, failed):
        self._on_log(
            f"Summary: {passed} Passed, {failed} Failed",
            "info" if failed == 0 else "error",
        )

    def _on_progress(self, done, total):
        if total > 0:
            pct = int(done / total * 100)
            self.progress.setMaximum(100)
            self.progress.setValue(pct)
            self.progress.setFormat(f"{done}/{total}  ({pct}%)")
        else:
            self.progress.setValue(0)
            self.progress.setFormat("Idle")

    def _on_verdict(self, key):
        self._set_led(key)
        m = {
            "idle": ("IDLE", "#6c6c6c"),
            "running": ("RUNNING", "#569cd6"),
            "pass": ("PASSED", "#4caf50"),
            "fail": ("FAILED", "#f44336"),
            "error": ("ERROR", "#ff9800"),
        }
        txt, col = m.get(key, ("?", "#6c6c6c"))
        self.lbl_verdict.setText(txt)
        self.lbl_verdict.setStyleSheet(
            f"font-weight:bold; font-size:13pt; color:{col};"
        )

    def _on_report(self, path):
        self._report_path = path
        self.btn_report.setEnabled(True)
        self._on_log(f"Report: {path}", "info")

    def _on_test_report(self, name, path):
        """Set the Report column for a specific test item."""
        root = self.tree.invisibleRootItem()
        for i in range(root.childCount()):
            g = root.child(i)
            for j in range(g.childCount()):
                it = g.child(j)
                if it.text(0) == name:
                    # Display the actual report filename
                    report_filename = os.path.basename(path)
                    it.setText(2, f"📄 {report_filename}")
                    it.setForeground(2, QColor("#4caf50"))
                    it.setData(2, Qt.ItemDataRole.UserRole, path)
                    # Make it bold to stand out
                    font = it.font(2)
                    font.setBold(True)
                    it.setFont(2, font)
                    # Add tooltip
                    it.setToolTip(2, f"Double-click to open: {report_filename}")
                    return

    def _on_write_window(self, text):
        """Display new Write Window content from CANoe."""
        for line in text.splitlines():
            stripped = line.rstrip()
            if stripped:
                self.write_win.append(f'<span style="color:#9cdcfe">{stripped}</span>')
        cur = self.write_win.textCursor()
        cur.movePosition(QTextCursor.MoveOperation.End)
        self.write_win.setTextCursor(cur)

    # ------------------------------------------------------------------
    # LED indicator helper
    # ------------------------------------------------------------------

    def _set_led(self, state):
        c = LED_COLORS.get(state, LED_COLORS["idle"])
        self.led.setStyleSheet(
            f"background-color:{c}; border-radius:12px; "
            f"min-width:24px; min-height:24px; "
            f"max-width:24px; max-height:24px; "
            f"border:2px solid {c};"
        )

    # ------------------------------------------------------------------
    # State-driven button enable / disable
    # ------------------------------------------------------------------

    def _update_buttons(self, state):
        self._state = state
        conn = state == "connected"
        run = state == "running"
        busy = state in ("connecting", "starting", "stopping", "testing")

        # connection area
        self.btn_browse_cfg.setEnabled(not busy and not run)
        self.btn_connect.setEnabled(not busy and not run)
        self.btn_browse_tse.setEnabled(conn or run)
        self.btn_load_tse.setEnabled(conn or run)

        # control area
        self.btn_start.setEnabled(conn)
        self.btn_stop.setEnabled(run)
        self.btn_run.setEnabled(run)

        labels = {
            "disconnected": "Disconnected",
            "connecting": "Connecting...",
            "connected": "Connected",
            "starting": "Starting...",
            "running": "Measurement Running",
            "stopping": "Stopping...",
            "testing": "Running Tests...",
            "error": "Error",
        }
        self.sb_state.setText(f"  {labels.get(state, state)}  ")

    # ------------------------------------------------------------------
    # Cleanup
    # ------------------------------------------------------------------

    def closeEvent(self, event):
        # Save paths before exit
        self._save_paths()
        self.sig_cleanup.emit()
        self._thread.quit()
        if not self._thread.wait(5000):
            self._thread.terminate()
        event.accept()


# ==========================================================================
# Entry point
# ==========================================================================


def main():
    app = QApplication(sys.argv)
    app.setStyleSheet(DARK_STYLE)
    win = MainWindow()
    win.show()
    sys.exit(app.exec())


if __name__ == "__main__":
    main()
