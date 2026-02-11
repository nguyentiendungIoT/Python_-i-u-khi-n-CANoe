###########################################################
# Properties / Options / Definitions for all CLA Components
###########################################################

option(CLA_ENABLE_DEBUG_LOGGING "Enables the logging for tracing / debug messages of the SAB CLA." OFF)

#################################################################
# Sources
#################################################################

set(INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)

set(CLA_PUBLIC_HEADERS
    ${INCLUDE_DIR}/cla/DataTypes.hpp
    ${INCLUDE_DIR}/cla/IDo.hpp
    ${INCLUDE_DIR}/cla/IDoMember.hpp
    ${INCLUDE_DIR}/cla/IDoData.hpp
    ${INCLUDE_DIR}/cla/IDoEvent.hpp
    ${INCLUDE_DIR}/cla/IDoField.hpp
    ${INCLUDE_DIR}/cla/IDoMethod.hpp
    ${INCLUDE_DIR}/cla/IDoConsumedData.hpp
    ${INCLUDE_DIR}/cla/IDoConsumedEvent.hpp
    ${INCLUDE_DIR}/cla/IDoConsumedField.hpp
    ${INCLUDE_DIR}/cla/IDoConsumedMethod.hpp
    ${INCLUDE_DIR}/cla/IDoProvidedData.hpp
    ${INCLUDE_DIR}/cla/IDoProvidedEvent.hpp
    ${INCLUDE_DIR}/cla/IDoProvidedField.hpp
    ${INCLUDE_DIR}/cla/IDoProvidedMethod.hpp
    ${INCLUDE_DIR}/cla/IValueEntity.hpp
    ${INCLUDE_DIR}/cla/ISerializer.hpp
    ${INCLUDE_DIR}/cla/IDeserializer.hpp
    ${INCLUDE_DIR}/cla/ICallContext.hpp
    ${INCLUDE_DIR}/cla/IConsumerCallContext.hpp
    ${INCLUDE_DIR}/cla/IProviderCallContext.hpp
    ${INCLUDE_DIR}/cla/ClaMacros.hpp
    ${INCLUDE_DIR}/cla/Version.hpp

    # SIL Use Case only

    ${INCLUDE_DIR}/cla/ClaSil.hpp
    ${INCLUDE_DIR}/cla/IClaService.hpp
    ${INCLUDE_DIR}/cla/IClaServiceSil.hpp
    ${INCLUDE_DIR}/cla/ClaError.hpp
)

set(CLA_INTERNAL_HEADERS
    ${SOURCE_DIR}/ClaService.hpp
    ${SOURCE_DIR}/WorkerThreadService.hpp
    ${SOURCE_DIR}/DoRegistry.hpp
    ${SOURCE_DIR}/Do.hpp
    ${SOURCE_DIR}/DoMember.hpp
    ${SOURCE_DIR}/DoData.hpp
    ${SOURCE_DIR}/DoEvent.hpp
    ${SOURCE_DIR}/DoMethod.hpp
    ${SOURCE_DIR}/DoField.hpp
    ${SOURCE_DIR}/DoConsumedData.hpp
    ${SOURCE_DIR}/DoConsumedEvent.hpp
    ${SOURCE_DIR}/DoConsumedMethod.hpp
    ${SOURCE_DIR}/DoConsumedField.hpp
    ${SOURCE_DIR}/DoInstContainer.hpp
    ${SOURCE_DIR}/DoInstContainerElement.hpp
    ${SOURCE_DIR}/DoProvidedData.hpp
    ${SOURCE_DIR}/DoProvidedEvent.hpp
    ${SOURCE_DIR}/DoProvidedMethod.hpp
    ${SOURCE_DIR}/DoProvidedField.hpp
    ${SOURCE_DIR}/CallContext.hpp
    ${SOURCE_DIR}/ConsumerCallContext.hpp
    ${SOURCE_DIR}/ProviderCallContext.hpp
    ${SOURCE_DIR}/ValueEntity.hpp
    ${SOURCE_DIR}/Serializer.hpp
    ${SOURCE_DIR}/Deserializer.hpp
    ${SOURCE_DIR}/SpdlogLibrary.hpp
    ${SOURCE_DIR}/SilKitCommunicationInfo.hpp
    ${SOURCE_DIR}/ISilKitFactory.hpp
    ${SOURCE_DIR}/SilKitFactory.hpp
)

set(CLA_SOURCES
    ${SOURCE_DIR}/ClaSil.cpp
    ${SOURCE_DIR}/WorkerThreadService.cpp
    ${SOURCE_DIR}/ClaService.cpp
    ${SOURCE_DIR}/DoRegistry.cpp
    ${SOURCE_DIR}/Do.cpp
    ${SOURCE_DIR}/DoMember.cpp
    ${SOURCE_DIR}/DoData.cpp
    ${SOURCE_DIR}/DoEvent.cpp
    ${SOURCE_DIR}/DoMethod.cpp
    ${SOURCE_DIR}/DoField.cpp
    ${SOURCE_DIR}/DoConsumedData.cpp
    ${SOURCE_DIR}/DoConsumedEvent.cpp
    ${SOURCE_DIR}/DoConsumedMethod.cpp
    ${SOURCE_DIR}/DoConsumedField.cpp
    ${SOURCE_DIR}/DoInstContainer.cpp
    ${SOURCE_DIR}/DoInstContainerElement.cpp
    ${SOURCE_DIR}/DoProvidedData.cpp
    ${SOURCE_DIR}/DoProvidedEvent.cpp
    ${SOURCE_DIR}/DoProvidedMethod.cpp
    ${SOURCE_DIR}/DoProvidedField.cpp
    ${SOURCE_DIR}/ValueEntity.cpp
    ${SOURCE_DIR}/CallContext.cpp
    ${SOURCE_DIR}/ConsumerCallContext.cpp
    ${SOURCE_DIR}/ProviderCallContext.cpp
    ${SOURCE_DIR}/Serializer.cpp
    ${SOURCE_DIR}/Deserializer.cpp
    ${SOURCE_DIR}/Version.cpp
    ${SOURCE_DIR}/SilKitCommunicationInfo.cpp
    ${SOURCE_DIR}/SilKitFactory.cpp
)

#################################################################
# Library Properties
#################################################################

add_library(Cla STATIC
    ${CLA_PUBLIC_HEADERS}
    ${CLA_INTERNAL_HEADERS}
    ${CLA_SOURCES}
)

if (CLA_ENABLE_DEBUG_LOGGING)
    target_compile_definitions(Cla PRIVATE "CLA_ENABLE_DEBUG_LOGGING")
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    target_compile_options(Cla PRIVATE "/MP")
    target_compile_definitions(Cla PRIVATE "_SCL_SECURE_NO_WARNINGS")
    target_compile_definitions(Cla PRIVATE "_CRT_SECURE_NO_WARNINGS")
    
    if(${MSVC_TOOLSET_VERSION} GREATER_EQUAL 141)
        # VS2017 or higher
        target_compile_options(Cla PRIVATE "/permissive-")
    endif()
    if (${MSVC_TOOLSET_VERSION} LESS_EQUAL 141)
        # VS2017 or lower
        target_compile_options(Cla PRIVATE
            /wd4307 # deliberate 'integral constant overflow' in fmt, https://github.com/fmtlib/fmt/issues/3163
        )
    endif()
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    target_compile_options(Cla PRIVATE "-pedantic")
endif()

target_include_directories(Cla
    PUBLIC $<BUILD_INTERFACE:$<$<BOOL:$<TARGET_PROPERTY:CLA_INTERNALS>>:${SOURCE_DIR}>>
           $<BUILD_INTERFACE:${INCLUDE_DIR}>
           $<INSTALL_INTERFACE:include>
)

target_link_libraries(Cla
    PUBLIC $<BUILD_INTERFACE:$<$<BOOL:$<TARGET_PROPERTY:CLA_INTERNALS>>:spdlog>>
    PRIVATE SilKit
            $<BUILD_INTERFACE:spdlog>
)

#################################################################
# IDE Properties
#################################################################

source_group(TREE ${INCLUDE_DIR} PREFIX "public" FILES ${CLA_PUBLIC_HEADERS})
source_group(TREE ${SOURCE_DIR} PREFIX "internal" FILES ${CLA_INTERNAL_HEADERS})
source_group(TREE ${SOURCE_DIR} PREFIX "src" FILES ${CLA_SOURCES})
