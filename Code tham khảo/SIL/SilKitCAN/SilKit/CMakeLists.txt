# Copyright (c) Vector Informatik GmbH. All rights reserved.
cmake_minimum_required(VERSION 3.5)

#######################################################################################################################
# Project definition
#######################################################################################################################
project("SilKit-CANoe-Demo-CAN_easy" LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 14)

#assuming we are standalone but still within the original distribution directory layout
#so the source release should be located in ../${INSTALL_SOURCE_DIR}
#NB: as of VIB 3.0.2 FMT is optional
if(NOT SILKIT_DIRECTORY)
    set(SILKIT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/SilKit/)
    if(NOT SILKIT_DIRECTORY)
        message(FATAL_ERROR "Please set the PATH to the SilKit in SILKIT_DIRECTORY: e.g."
            " cmake -D SILKIT_DIRECTORY=../SilKit-x.y.z-Platform/")
    endif()
endif()
message(STATUS "Searching SilKit package in ${SILKIT_DIRECTORY}")
find_package(SilKit
    REQUIRED
    CONFIG
    NO_CMAKE_PACKAGE_REGISTRY
    NO_DEFAULT_PATH
    PATHS "${SILKIT_DIRECTORY}"
)

set(_DEMO_OUTPUT_DIR "${CMAKE_CURRENT_LIST_DIR}/bin")

get_target_property(impLocRelease SilKit::SilKit IMPORTED_LOCATION_RELEASE)
get_target_property(impLocDebug SilKit::SilKit IMPORTED_LOCATION_DEBUG)
file(COPY ${impLocRelease} ${impLocDebug} DESTINATION ${_DEMO_OUTPUT_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${_DEMO_OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${_DEMO_OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${_DEMO_OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${_DEMO_OUTPUT_DIR})

if(NOT TARGET SilKit::SilKit)
    message(FATAL_ERROR "Could not find SilKit package in ${SILKIT_DIRECTORY}")
endif()

#######################################################################################################################
# Add the actual demo projects
#######################################################################################################################
add_subdirectory(SUT/Engine)
add_subdirectory(SUT/Lights)

file(COPY ${impLocRelease} ${impLocDebug} DESTINATION ${CMAKE_BINARY_DIR})
