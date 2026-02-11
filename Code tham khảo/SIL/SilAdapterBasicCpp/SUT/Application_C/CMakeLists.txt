# Define Executable
add_executable(Application_C
    src/Logic.c
    src/Logic.h
    src/IO.h
    src/IO.c
    src/main.c
)

target_sources(Application_C
    PRIVATE SilAdapterWrapper/SilAdapterWrapper.h
            SilAdapterWrapper/SilAdapterWrapper.cpp
)

target_include_directories(Application_C PRIVATE SilAdapterWrapper)

# Link SilAdapter with Executable
target_link_libraries(Application_C PRIVATE SilAdapter)
