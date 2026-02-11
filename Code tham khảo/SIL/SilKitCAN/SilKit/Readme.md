
# CANoe Sample Configuration "SIL Kit CAN Demo" - SUTs based on SIL Kit

This directory contains the two SUTs required by the "SIL Kit CAN Demo" sample configuration for CANoe.
The SUTs and CANoe are using SIL Kit to exchange CAN frames.

For ease of use, precompiled executables are provided in the `bin` directory.

## Building the SUTs

You can compile the SUTs yourself, you will simply have to download a SIL Kit package from the available [releases](https://github.com/vectorgrp/sil-kit/releases).

Please download the appropriate package, e.g., `SilKit-...-Win-x86_64-VS2017.zip` for building with the Visual Studio 2017 (or later) for x64.
Directly unzip the package in this directory. This should result in a directory named exactly like the package, e.g., `SilKit-...-Win-x86_64-VS2017`.

The SUTs and SIL Kit use `cmake` as the build-system. When running `cmake` on the command line on Windows, make sure the relevant Visual Studio environment is loaded, see `vcvarsall.bat` for more information.

The provided `CMakeSettings.json` and `CMakePresets.json` are set to use version **4.0.5** of SIL Kit.
You can use other versions, but please make sure to change the `SILKIT_DIRECTORY` value in the `CMakeSettings.json` or `CMakePresets.json` files, otherwise the configuration step will fail, due to the build system not being able to find a compatible version of SIL Kit.

Please provide the path to the unzipped `SilKit-<version>-<platform>-<architecture>-<toolset>` package directory to cmake during the configuration step in the `SILKIT_DIRECTORY` variable.

The directory structure after unzipping should look like the following:

```
...
Engine/...
Lights/...
SilKit-<version>-<platform>-<architecture>-<toolset>/...
  SilKit/...
  SilKit-Demos/...
  SilKit-Documentation/...
  SilKit-Source/...
  ...
```

