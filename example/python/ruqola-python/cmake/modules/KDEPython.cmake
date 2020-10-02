# KDE_Python
#
# defines macros for installing python files in a KDE project:
#   * KDE_INSTALL_PYTHON_FILES
#   * KDE_ADD_PYTHON_EXECUTABLE
#
# By Wolfgang Rohdewald <wolfgang@rohdewald.de>
#
# copied and adapted from
#    FindPyKDE4.cmake by Simon Edwards <simon@simonzone.com>
#    PythonMacros.cmake by Simon Edwards and others
#
# removed support for windows and Python2


###########################################################################
# PYTHON_INSTALL (SOURCE_FILE DESTINATION_DIR)
#
#     Install the SOURCE_FILE, which is a Python .py file, into the
#     destination directory during install. The file will be byte compiled
#     and both the .py file and the file in __pycache__ will be installed.
#     Neither Windows nor Python2 is supported.

set(PYTHON_MACROS_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})

macro(PYTHON_INSTALL SOURCE_FILE DESTINATION_DIR)

  find_file(_python_compile_py PythonCompile.py PATHS ${CMAKE_MODULE_PATH})

  # Install the source file.
  install(FILES ${SOURCE_FILE} DESTINATION ${DESTINATION_DIR})

  # Byte compile and install the .pyc file, unless explicitly prevented by env..
  if("$ENV{PYTHONDONTWRITEBYTECODE}" STREQUAL "")
    get_filename_component(_absfilename ${SOURCE_FILE} ABSOLUTE)
    get_filename_component(_filename ${SOURCE_FILE} NAME)
    get_filename_component(_filenamebase ${SOURCE_FILE} NAME_WE)
    get_filename_component(_basepath ${SOURCE_FILE} PATH)
    file(RELATIVE_PATH _relativefilename ${CMAKE_SOURCE_DIR} ${_absfilename})

    set(_bin_py ${CMAKE_CURRENT_BINARY_DIR}/${_basepath}/${_filename})

    set(_bin_pyc "${CMAKE_CURRENT_BINARY_DIR}/${_basepath}/__pycache__/${_filenamebase}.cpython-${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR}.pyc")
    set(_py_install_dir "${DESTINATION_DIR}/__pycache__/")

    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${_basepath})

    # Setting because it will be displayed later, in compile_python_files
    set(_message "Byte-compiling ${_bin_py} to ${_bin_pyc}")

    string(REPLACE "/" "_" _rule_name "${_relativefilename}_rule")
    string(REPLACE " " "_" _rule_name "${_rule_name}")
    add_custom_target("${_rule_name}" ALL)

    get_filename_component(_abs_bin_py ${_bin_py} ABSOLUTE)
    if(_abs_bin_py STREQUAL _absfilename)    # Don't copy the file onto itself.
      add_custom_command(
        TARGET "${_rule_name}"
        COMMAND "${CMAKE_COMMAND}" -E echo "${_message}"
        COMMAND "${PYTHON_EXECUTABLE}" "${_python_compile_py}" "--destination-dir" "${DESTINATION_DIR}" "${_bin_py}"
        DEPENDS "${_absfilename}"
      )
    else()
      add_custom_command(
        TARGET "${_rule_name}"
        COMMAND "${CMAKE_COMMAND}" -E echo "${_message}"
        COMMAND "${CMAKE_COMMAND}" -E copy "${_absfilename}" "${_bin_py}"
        COMMAND "${PYTHON_EXECUTABLE}" "${_python_compile_py}" "--destination-dir" "${DESTINATION_DIR}" "${_bin_py}"
        DEPENDS "${_absfilename}"
      )
    endif()

    install(FILES ${_bin_pyc} DESTINATION "${_py_install_dir}")
    unset(_py_install_dir)
    unset(_message)

  endif("$ENV{PYTHONDONTWRITEBYTECODE}" STREQUAL "")

endmacro(PYTHON_INSTALL)


###########################################################################
# KDE_INSTALL_PYTHON_FILES(file_name...)
#
# Installs and bytes compiles Python files into the data directory for this
# project..
#

MACRO(KDE_INSTALL_PYTHON_FILES)
    FOREACH (_current_file ${ARGN})
        python_install(${_current_file} ${DATA_INSTALL_DIR}/${PROJECT_NAME})
    ENDFOREACH (_current_file)
ENDMACRO(KDE_INSTALL_PYTHON_FILES)


###########################################################################
# KDE_ADD_PYTHON_EXECUTABLE(py_name exe_name)
#
# Creates a smybolic link with name exe_name at install time from the
# install bin directory to the Python file. The Python file is also make
# executable.
#
MACRO(KDE_ADD_PYTHON_EXECUTABLE _pyname _exename)
    if(NOT PROJECT_NAME)
         MESSAGE(STATUS "Project name is necessary to create symlink against python program!!! It will fail.")
    endif(NOT PROJECT_NAME)

    set(TARGET ${CMAKE_INSTALL_PREFIX}/${DATA_INSTALL_DIR}/${PROJECT_NAME}/${_pyname})
    set(LINK_NAME ${CMAKE_INSTALL_PREFIX}/${BIN_INSTALL_DIR}/${_exename})

    GET_FILENAME_COMPONENT(abs_link_name ${LINK_NAME} ABSOLUTE)
    GET_FILENAME_COMPONENT(link_path ${LINK_NAME} PATH)
    GET_FILENAME_COMPONENT(abs_link_path ${link_path} ABSOLUTE)
    GET_FILENAME_COMPONENT(abs_target ${TARGET} ABSOLUTE)

    INSTALL(CODE "EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E make_directory \$ENV\{DESTDIR\}${abs_link_path})")
    INSTALL(CODE "EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E create_symlink ${abs_target} \$ENV\{DESTDIR\}${abs_link_name})")
    INSTALL(CODE "EXECUTE_PROCESS(COMMAND chmod a+x \$ENV\{DESTDIR\}/${abs_target})")

ENDMACRO(KDE_ADD_PYTHON_EXECUTABLE)
