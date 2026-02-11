# Define Executable
add_executable(Application_Cpp
   src/main.cpp
)

# Link SilAdapter with Executable
target_link_libraries(Application_Cpp PRIVATE SilAdapter)
