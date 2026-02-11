cmake_minimum_required(VERSION 3.10)

project(SilAdapterBasic DESCRIPTION "Basic SiL-Adapter Demo for Cpp" LANGUAGES C CXX)

# Add SilAdapter Dependency
include(SilAdapter.cmake)
add_siladapter(
        INPUT_VCDLS "${CMAKE_CURRENT_SOURCE_DIR}/../CANoe/DataSources/RoomTemperatureControl.vcdl"
        #OUTPUT_DIR <SIL Adapter output folder>
        #NAME <SIL Adapter name>
        #SIL_ADAPTER_BUILDER_DIR <sil-adapter-builder.exe directory (e.g., the Exec64 directory of Test Unit Runner or CANoe installation)>
        )

# Add Applications
add_subdirectory(Application_C)
add_subdirectory(Application_Cpp)
