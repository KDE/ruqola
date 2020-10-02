#macro (PYSIDE2_UIC_GENERATOR _sources )
    #find_program(PYSIDE2-UIC NAMES pyside2-uic)
    #if (NOT PYSIDE2-UIC)
        #MESSAGE(ERROR "Impossible to find pyside2-uic.")
    #endif()
    #foreach (_current_FILE ${ARGN})
        #get_filename_component(_tmp_FILE ${_current_FILE} ABSOLUTE)
        #get_filename_component(_basename ${_tmp_FILE} NAME_WE)

    #MESSAGE(STATUS "ssssssssssss ${_current_FILE} ${PYSIDE2-UIC} ${_tmp_FILE}")
        #set(generated_pythonfile ${CMAKE_CURRENT_BINARY_DIR}/${_basename}.py)

        #add_custom_command(OUTPUT ${generated_pythonfile}
         #COMMAND ${PYSIDE2-UIC} ${_current_FILE} -o ${generated_pythonfile}
         #MAIN_DEPENDENCY ${_tmp_FILE}
      #)
      #list(APPEND ${_sources} ${generated_pythonfile} )
    #endforeach()
#endmacro ()


    #MACRO(PYSIDE_WRAP_UI outfiles)
    #set(UICOPTIONS "--generator=python")
        #find_program(PYSIDE2-UIC NAMES pyside2-uic)
        #if (NOT PYSIDE2-UIC)
            #MESSAGE(ERROR "Impossible to find pyside2-uic.")
        #endif()
    #FOREACH(it ${ARGN})
        #GET_FILENAME_COMPONENT(outfile ${it} NAME_WE)
        #GET_FILENAME_COMPONENT(infile ${it} ABSOLUTE)
        #SET(outfile ${CMAKE_CURRENT_BINARY_DIR}/ui_${outfile}.py)
        #MESSAGE(STATUS "${outfile}")
        #ADD_CUSTOM_TARGET(${it} ALL
         #DEPENDS ${outfile}
        #)
        #if(WIN32 OR APPLE)
            #ADD_CUSTOM_COMMAND(OUTPUT ${outfile}
            #COMMAND ${PYSIDE2UICBINARY} ${UICOPTIONS} ${infile} -o ${outfile}
            #MAIN_DEPENDENCY ${infile}
            #)
        #else()
            #Especially on Open Build Service we don't want changing date like
            #pyside2-uic generates in comments at beginning., which is why
            #we follow the tool command with in-place sed.
            #ADD_CUSTOM_COMMAND(OUTPUT ${outfile}
            #COMMAND "${PYSIDE2-UIC}" ${UICOPTIONS} "${infile}" -o "${outfile}"
            #COMMAND sed -i "/^# /d" "${outfile}"
            #MAIN_DEPENDENCY "${infile}"
            #)
        #endif()
        #list(APPEND ${outfiles} ${outfile})
        #MESSAGE(STATUS "${outfiles}")
    #ENDFOREACH(it)
    #ENDMACRO (PYSIDE_WRAP_UI)

include(CMakeParseArguments)


# qt5_wrap_ui(outfiles inputfile ... )

function(PYSIDE_WRAP_UI outfiles )
    set(options)
    set(oneValueArgs)
    set(multiValueArgs OPTIONS)

        set(UICOPTIONS "--generator=python")
        find_program(PYSIDE2-UIC NAMES pyside2-uic)
        if (NOT PYSIDE2-UIC)
            MESSAGE(ERROR "Impossible to find pyside2-uic.")
        endif()
    cmake_parse_arguments(_WRAP_UI "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(ui_files ${_WRAP_UI_UNPARSED_ARGUMENTS})
    set(ui_options ${_WRAP_UI_OPTIONS})

    foreach(it ${ui_files})
    MESSAGE(STATUS "FILE ${it}")
        get_filename_component(outfile ${it} NAME_WE)
        get_filename_component(infile ${it} ABSOLUTE)
        set(outfile ${CMAKE_CURRENT_BINARY_DIR}/ui_${outfile}.py)
        add_custom_command(OUTPUT ${outfile}
          COMMAND ${PYSIDE2-UIC}
          ARGS ${ui_options} -o ${outfile} ${infile}
          MAIN_DEPENDENCY ${infile} VERBATIM)
        list(APPEND ${outfiles} ${outfile})
    endforeach()
    set(${outfiles} ${${outfiles}} PARENT_SCOPE)
endfunction()
