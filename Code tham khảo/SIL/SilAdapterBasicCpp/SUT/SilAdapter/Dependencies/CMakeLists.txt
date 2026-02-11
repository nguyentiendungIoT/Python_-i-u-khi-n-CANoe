cmake_minimum_required(VERSION 3.13)

project(Cla DESCRIPTION "Common Language API (CLA) for the SIL Adapter use case." LANGUAGES CXX)

#####################################################
# Externals
#####################################################

set(SILKIT_BUILD_TESTS OFF CACHE BOOL "Enable unit and integration tests for the SIL Kit" FORCE)
set(SILKIT_BUILD_STATIC ON CACHE BOOL "build SIL Kit static library" FORCE)
set(SILKIT_BUILD_DEMOS OFF CACHE BOOL "Build SIL Kit demos" FORCE)
set(SILKIT_BUILD_UTILITIES OFF CACHE BOOL "Build SIL Kit utility tools" FORCE)
set(SILKIT_BUILD_DASHBOARD OFF CACHE BOOL "Build SIL Kit dashboard" FORCE)
set(SILKIT_PACKAGE_SYMBOLS OFF CACHE BOOL "Package the SIL Kit PDBs" FORCE)
if(${CMAKE_CURRENT_BINARY_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  add_subdirectory(sk)
else()
  add_subdirectory(sk ${CMAKE_CURRENT_BINARY_DIR}/../sk)
endif()

#####################################################
# CLA
#####################################################

add_subdirectory(Cla)
