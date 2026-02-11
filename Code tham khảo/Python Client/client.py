# This script requires pythonnet (can be installed via "pip install pythonnet")
# Tested with Python 3.9.13 and pythonnet 3.0.1

# Add assembly references (if XIL API has been installed to a different location, these need to be adapted)
import clr
clr.AddReference("C:\\Program Files (x86)\\ASAM e.V\\ASAM AE XIL API Standard Assemblies 2.1.0\\bin\\ASAM.XIL.Implementation.Testbench.dll")
clr.AddReference("C:\\Program Files (x86)\\ASAM e.V\\ASAM AE XIL API Standard Assemblies 2.1.0\\bin\\ASAM.XIL.Implementation.TestbenchFactory.dll")
clr.AddReference("C:\\Program Files (x86)\\ASAM e.V\\ASAM AE XIL API Standard Assemblies 2.1.0\\bin\\ASAM.XIL.Interfaces.dll")

# Import required types
from ASAM.XIL.Implementation.TestbenchFactory.Testbench import TestbenchFactory
from ASAM.XIL.Implementation.Testbench.Common.ValueContainer import FloatValue
from ASAM.XIL.Implementation.Testbench.Common.ValueContainer import IntValue
from ASAM.XIL.Interfaces.Testbench.Common.ValueContainer import IFloatValue

# Additional Python includes
import time


# Instantiate Vector CANoe test bench for XIL API version 2.1.0
# Note: if using 32 bit Python, change "CANoe64" to "CANoe32" (this has nothing
# to do with the CANoe version but the python interpreter version you use!)
factory = TestbenchFactory()
testBench = factory.CreateVendorSpecificTestbench("Vector", "CANoe64", "2.1.0")

# Instantiate a model access port and configure it
print("Creating the MA port")
maPort = testBench.MAPortFactory.CreateMAPort("Python MA Port")
maPort.Configure(maPort.LoadConfiguration("VectorMAPortConfig.xml"), True)
maPort.StartSimulation()

# Read a variable
print("Reading variable 'Test::Variable1'")
for i in range(0, 3):
  floatValue = IFloatValue(maPort.Read("Test::Variable1"))
  print("Value of the variable is %d" % (floatValue.Value))
  time.sleep(.25)
  
# Write a variable
print("Writing variable 'Test::Variable2'")
for i in range(0, 100):
  maPort.Write("Test::Variable2", IntValue(i))
  time.sleep(.1)

# Shut the port down
print("Shutting down the MA port")
maPort.StopSimulation()
maPort.Disconnect()
maPort.Dispose()