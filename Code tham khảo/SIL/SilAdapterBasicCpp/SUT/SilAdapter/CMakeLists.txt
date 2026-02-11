cmake_minimum_required(VERSION 3.13)

################################################################################
# Project Definition
################################################################################

project("SilAdapter" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

################################################################################
# Include TypeLib Dependencies
################################################################################

set(CMAKE_FOLDER "SilAdapter/Dependencies")
add_subdirectory(Dependencies)

################################################################################
# Include TypeLib
################################################################################

option(SIL_ADAPTER_INCLUDE_SIL_KIT_API "Allows access to the SIL Kit target and passing an existing SIL Kit instance into the SIL Adapter." OFF)

add_subdirectory(TypeLib)
set_target_properties(TypeLib PROPERTIES FOLDER "SilAdapter")

add_library(SilAdapter ALIAS TypeLib)
