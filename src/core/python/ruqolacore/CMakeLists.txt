# SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
# Auto-Genereate files every class will have his cpp/h files
set(pyruqolacore_SRC
    # individual classes
    ${CMAKE_CURRENT_BINARY_DIR}/RuqolaCore/ruqolacore_module_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/RuqolaCore/ruqolacore_module_wrapper.h
    ${CMAKE_CURRENT_BINARY_DIR}/RuqolaCore/PyRuqolaCore_python.h
    )

# includes necessary to parse and build the classes specified on typesystem
set(pyruqolacore_include_paths
    $<JOIN:$<TARGET_PROPERTY:libruqolacore,INTERFACE_INCLUDE_DIRECTORIES>,${PATH_SEP}>
    )

# A list of paths where shiboken should look for typesystem
set(pyruqolacore_typesystem_paths
    # PySide path, this variable was exposed by FindPySide2.cmake
    ${PYSIDE_TYPESYSTEMS}
    )

# Include flags/path that will be set in 'target_include_directories'
set(pyruqolacore_target_include_directories
    ${CMAKE_SOURCE_DIR}
    )

# Libraries that will be necessary to link the target, this will used in the command 'target_link_libraries'
set(pyruqolacore_target_link_libraries
    Qt5::Core
    Qt5::Network
    Qt5::NetworkAuth
    Qt5::Gui
    Qt5::Widgets
    KF${KF_MAJOR_VERSION}::I18n
    ${Python3_LIBRARIES}
    )

# changes on these files should trigger a new generation
set(pyruqolacore_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/ruqolacore_global.h
    ${CMAKE_SOURCE_DIR}/src/core/rocketchataccount.h
    )

create_python_bindings(
    "RuqolaCore"
    "${pyruqolacore_typesystem_paths}"
    "${pyruqolacore_include_paths}"
    "${pyruqolacore_SRC}"
    "${pyruqolacore_target_include_directories}"
    "${pyruqolacore_target_link_libraries}"
    ${CMAKE_CURRENT_SOURCE_DIR}/ruqolacore_global.h
    ${CMAKE_CURRENT_SOURCE_DIR}/typesystem_ruqolacore.xml
    "${pyruqolacore_DEPENDS}"
    ${CMAKE_CURRENT_BINARY_DIR}
    )

# Make module import from build dir works
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/__init__.py ${CMAKE_CURRENT_BINARY_DIR}/__init__.py)

#TODO define PYTHON_BINDINGS_INSTALL_PREFIX
# install
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py DESTINATION ${PYTHON_BINDINGS_INSTALL_PREFIX}/pyruqolacore)
