# SIL Adapter Type Library

This folder contains a SIL Adapter Type Library for Python which has been
generated based on a CANoe/CANoe4SW configuration. The Type Library represents
Distributed Objects with a SIL Adapter binding. It may be used in a Python
application to access those Distributed Objects.

To run the Type Library there is a runtime needed that is dependent on the
version and the architecture of the used Python interpreter.

# Installation of SIL Adapter runtime

The SIL Adapter runtime is currently shipped as a Python package with CANoe/CANoe4SW.
Multiple versions of this Python package are located below the subfolder
"Installer Additional Components\SilAdapter\Python Runtime" of a
CANoe/CANoe4SW installation folder. 

Currently there are packages available for Windows and Linux, 32-bit and
64-bit as well as for multiple versions of the Python interpreter.
The right version must be installed to the Python interpreter that shall 
be used to run the Python application that shall able to access the 
Distributed Objects. You may copy the packages to a machine with a supported 
architecture and operating system.

In order to install the right version for the current machine and for the 
current Python interpreter there exists an installer script. You may open a 
command line and type

c:\> python "<path to CANoe/CANoe4SW installation>\Installer Additional Components\SilAdapter\Python Runtime\install_runtime.py"

# Notes

The installed runtime version must have the same major version and must be
newer or equal than the runtime version shipped with the CANoe/CANoe4SW
that has been used to generate the Type Library.

The Type Library must be re-generated in the CANoe/CANoe4SW configuration if
the definition of the Distributed Object have been changed.

The Type Library may only be used to run a Python application and not for
Python models that were added to a CANoe/CANoe4SW configuration.
