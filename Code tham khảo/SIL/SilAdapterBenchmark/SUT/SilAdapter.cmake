function(select_canoe_install_dir)
    set(relevantEnvVars "TestUnitRunner_InstallDir64;CANoe4SWSE_InstallDir64;CANoe4SWSE_InstallDir;CANoe4SW_InstallDir64;CANoe4SW_InstallDir;CANoe_InstallDir64;CANoe_InstallDir")
    foreach(relevantEnvVar ${relevantEnvVars})
        if(DEFINED ENV{${relevantEnvVar}})
            set(SIL_ADAPTER_SIL_ADAPTER_BUILDER_DIR $ENV{${relevantEnvVar}} PARENT_SCOPE)
            return()
        endif()
    endforeach()
    string(REPLACE ";"  " " relevantEnvVarsWithSpaces "${relevantEnvVars}")
    message(FATAL_ERROR "One of the following environment variables needs to be set: ${relevantEnvVarsWithSpaces}" )
endfunction()

function(get_dependent_file_list)
    cmake_parse_arguments("SIL_ADAPTER" "" "NAME;OUTPUT_DIR" "" ${ARGN} )
    
    set(dependentFileListPathFullVersion "${CMAKE_CURRENT_BINARY_DIR}/.${SIL_ADAPTER_NAME}.build/${SIL_ADAPTER_NAME}.venvironment/dependent_file_lists.json")
    set(dependentFileListPathBasic "${CMAKE_CURRENT_BINARY_DIR}/.${SIL_ADAPTER_NAME}.build/${SIL_ADAPTER_NAME}.venvironment-basic/dependent_file_lists.json")
    
    if(EXISTS ${dependentFileListPathFullVersion})
        set(dependentFileListPath ${dependentFileListPathFullVersion})
    elseif(EXISTS ${dependentFileListPathBasic})
        set(dependentFileListPath ${dependentFileListPathBasic})
    else()
        message(FATAL_ERROR "dependent file list does not exist: Searched for ${dependentFileListPathFullVersion} and ${dependentFileListPathBasic}.")
    endif()
    
    file(READ ${dependentFileListPath} dependentFileList)
    string(REGEX MATCHALL "\"path\":.?\"[^\"]*" result ${dependentFileList})
    string(REPLACE "\"path\": " "" result "${result}")
    string(REPLACE "\"" "" result "${result}")
    set(DEPENDENT_FILE_LIST "${result}" PARENT_SCOPE)
endfunction()

function(add_siladapter)     
    cmake_parse_arguments("SIL_ADAPTER" "" "NAME;OUTPUT_DIR;SIL_ADAPTER_BUILDER_DIR;VERBOSITY_LEVEL" "INPUT_VCDLS" ${ARGN} )
    
    if(NOT DEFINED SIL_ADAPTER_INPUT_VCDLS)
        message(FATAL_ERROR "You must specify at least one .vcdl file.")
        return()
    endif()

    if(NOT DEFINED SIL_ADAPTER_NAME)
        set(SIL_ADAPTER_NAME SilAdapter)
    endif()
    
    if(NOT DEFINED SIL_ADAPTER_OUTPUT_DIR)
        set(SIL_ADAPTER_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
    else()
        if(IS_ABSOLUTE ${SIL_ADAPTER_OUTPUT_DIR})
            set(SIL_ADAPTER_OUTPUT_DIR ${SIL_ADAPTER_OUTPUT_DIR})
        else()
            set(SIL_ADAPTER_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${SIL_ADAPTER_OUTPUT_DIR})
        endif()
    endif()

    # On Unix the SIL Adapter Generation is not available. Therefore try adding an already existing SIL Adapter.
    if(UNIX)
        add_subdirectory(${SIL_ADAPTER_OUTPUT_DIR}/${SIL_ADAPTER_NAME})
        return()
    endif(UNIX)
    
    if(NOT DEFINED SIL_ADAPTER_VERBOSITY_LEVEL)
        set(SIL_ADAPTER_VERBOSITY_LEVEL info)
    endif()

    if(NOT DEFINED SIL_ADAPTER_SIL_ADAPTER_BUILDER_DIR)
        select_canoe_install_dir()
    endif()
    
    if(NOT EXISTS ${SIL_ADAPTER_SIL_ADAPTER_BUILDER_DIR})
        message(FATAL_ERROR "SIL Adapter Generation failed. sil-adapter-builder.exe directory does not exist: ${SIL_ADAPTER_SIL_ADAPTER_BUILDER_DIR}")
        return()
    endif()
    
    message("SIL Adapter Builder will use the following installation: ${SIL_ADAPTER_SIL_ADAPTER_BUILDER_DIR}")

    set(RESOLVED_INPUT_SOURCES "")
    foreach(input_item ${SIL_ADAPTER_INPUT_VCDLS})
        set(resolved_input_item ${input_item})
        if(NOT IS_ABSOLUTE ${input_item})
            string(REPLACE ${input_item} "${CMAKE_CURRENT_SOURCE_DIR}/${input_item}" resolved_input_item ${input_item})
        endif()
        list(APPEND RESOLVED_INPUT_SOURCES ${resolved_input_item})
    endforeach()

    execute_process(COMMAND "${SIL_ADAPTER_SIL_ADAPTER_BUILDER_DIR}/sil-adapter-builder.exe" ${RESOLVED_INPUT_SOURCES} -l cpp -o ${SIL_ADAPTER_OUTPUT_DIR} -n ${SIL_ADAPTER_NAME} -t ${CMAKE_CURRENT_BINARY_DIR} -v ${SIL_ADAPTER_VERBOSITY_LEVEL} RESULT_VARIABLE SAB_RESULT) 
    if(NOT ${SAB_RESULT} EQUAL 0)
        message(FATAL_ERROR "Failed to generate SIL Adapter: sil-adapter-builder.exe failed with error: ${SAB_RESULT}")
    endif()

    # Set File Watcher that will trigger CMake generation on change
    set(FILE_WATCHER_OUT_DIR "sil_adapter_file_watcher")
    file(REMOVE_RECURSE "${CMAKE_CURRENT_BINARY_DIR}/${FILE_WATCHER_OUT_DIR}")

    if(DEFINED SIL_ADAPTER_INPUT_VCDLS)
        get_dependent_file_list(
            OUTPUT_DIR "${SIL_ADAPTER_OUTPUT_DIR}"
            NAME "${SIL_ADAPTER_NAME}")
        
        foreach(INPUT_VCDL ${DEPENDENT_FILE_LIST})
            string(RANDOM randomId)
            get_filename_component(INPUT_VCDL_FILENAME "${INPUT_VCDL}" NAME)
            string(REGEX REPLACE "\.[vV][cC][dD][lL]" "_${randomId}.vcdl" INPUT_VCDL_FILENAME "${INPUT_VCDL_FILENAME}")

            # Enforces re-run of CMake generation if one or more dependent files have changed.
            configure_file("${INPUT_VCDL}" "${FILE_WATCHER_OUT_DIR}/${INPUT_VCDL_FILENAME}")
        endforeach()
    endif()
         
    add_subdirectory(${SIL_ADAPTER_OUTPUT_DIR}/${SIL_ADAPTER_NAME} ${CMAKE_CURRENT_BINARY_DIR}/${SIL_ADAPTER_NAME})
endfunction()