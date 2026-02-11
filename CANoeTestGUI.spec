# -*- mode: python ; coding: utf-8 -*-
# PyInstaller spec file for CANoe Automation Test GUI

import sys
import os

block_cipher = None

a = Analysis(
    ['CANoeTestGUI.py'],
    pathex=[],
    binaries=[],
    datas=[
        ('YuRa.png', '.'),   # Bundle logo into root of the package
    ],
    hiddenimports=[
        'pythoncom',
        'pywintypes',
        'win32com',
        'win32com.client',
        'win32com.server',
        'win32api',
        'win32con',
        'openpyxl',
        'openpyxl.styles',
        'PyQt6',
        'PyQt6.QtWidgets',
        'PyQt6.QtCore',
        'PyQt6.QtGui',
        'PyQt6.sip',
    ],
    hookspath=[],
    hooksconfig={},
    runtime_hooks=[],
    excludes=[],
    noarchive=False,
    optimize=0,
)

pyz = PYZ(a.pure, a.zipped_data, cipher=block_cipher)

exe = EXE(
    pyz,
    a.scripts,
    a.binaries,
    a.datas,
    [],
    name='CANoeTestGUI',
    debug=False,
    bootloader_ignore_signals=False,
    strip=False,
    upx=True,
    upx_exclude=[],
    runtime_tmpdir=None,
    console=False,           # No console window (GUI app)
    disable_windowed_traceback=False,
    argv_emulation=False,
    target_arch=None,
    codesign_identity=None,
    entitlements_file=None,
    icon='YuRa.ico',         # Executable icon
)
