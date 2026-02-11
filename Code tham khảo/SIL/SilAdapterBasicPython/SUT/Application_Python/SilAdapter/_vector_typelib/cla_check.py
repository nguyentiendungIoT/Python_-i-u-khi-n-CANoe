#
# Copyright (c) Vector Informatik GmbH. All rights reserved.
#
# This code was generated automatically. Changes to this file may
# cause incorrect behavior and will be lost if the code is regenerated.

canoe_cla_installed = True

try:
    import vector.canoe
except ModuleNotFoundError:
    canoe_cla_installed = False

if canoe_cla_installed is True:
    raise RuntimeError("Der SIL Adapter kann nicht von CANoe geladen werden. Bitte benutzen Sie den SIL Adapter in einem Skript, welches außerhalb von CANoe läuft.")

try:
    import canoe_sil_adapter_runtime.cla
except ModuleNotFoundError:
    raise ModuleNotFoundError("Das Paket canoe-sil-adapter-runtime fehlt. Es kann mit <CANoe/CANoe4SW install folder>/Installer Additional Components/SilAdapter/Python Runtime/install_runtime.py installiert werden.")

SAB_CLA = "SabCla"
CANOE_CLA = "CANoeCla"
REQUIRED_CLA_VERSION = "6.0.22"


def check_version():
    required_version = REQUIRED_CLA_VERSION.split('.')
    required_major = int(required_version[0])
    required_minor = int(required_version[1])
    required_patch = int(required_version[2])
    implementation_version = canoe_sil_adapter_runtime.cla.GetImplementationVersion()
    if(implementation_version.major != required_major or (required_minor, required_patch) > (implementation_version.minor, implementation_version.patch)):
        raise VersionException(str(implementation_version.major) + '.' + str(implementation_version.minor) + '.' + str(implementation_version.patch))


class VersionException(Exception):
    def __init__(self, *args):
        self.message = args[0]

    def __str__(self):
        return 'Die canoe-sil-adapter-runtime Paketversion ist inkompatibel: {0} geeignet: {1}. Bitte aktualisieren sie das canoe-sil-adapter-runtime Paket und generieren sie die TypeLib neu.'.format(self.message, REQUIRED_CLA_VERSION)
