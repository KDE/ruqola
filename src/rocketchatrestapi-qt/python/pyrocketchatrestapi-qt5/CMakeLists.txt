# SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
# Auto-Genereate files every class will have his cpp/h files
set(pyrocketchatrestapi-qt5_SRC
    # individual classes
    ${CMAKE_CURRENT_BINARY_DIR}/RocketChatRestapiQt5/rocketchatrestapiqt5_module_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/RocketChatRestapiQt5/rocketchatrestapiqt5_module_wrapper.h
    ${CMAKE_CURRENT_BINARY_DIR}/RocketChatRestapiQt5/RocketChatRestapiQt5_python.h
    )

# includes necessary to parse and build the classes specified on typesystem
set(pyrocketchatrestapi-qt5_include_paths
    $<JOIN:$<TARGET_PROPERTY:librocketchatrestapi-qt5,INTERFACE_INCLUDE_DIRECTORIES>,${PATH_SEP}>
    )

# A list of paths where shiboken should look for typesystem
set(pyrocketchatrestapi-qt5_typesystem_paths
    # PySide path, this variable was exposed by FindPySide2.cmake
    ${PYSIDE_TYPESYSTEMS}
    )

# Include flags/path that will be set in 'target_include_directories'
set(pyrocketchatrestapi-qt5_target_include_directories
    ${CMAKE_SOURCE_DIR}
    )

# Libraries that will be necessary to link the target, this will used in the command 'target_link_libraries'
set(pyrocketchatrestapi-qt5_target_link_libraries
    Qt5::Core
    Qt5::Network
    Qt5::NetworkAuth
    Qt5::Gui
    Qt5::Widgets
    KF${KF_MAJOR_VERSION}::I18n
    ${Python3_LIBRARIES}
    )

# changes on these files should trigger a new generation
set(pyrocketchatrestapi-qt5_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/rocketchatrestapi-qt5_global.h
    ${CMAKE_SOURCE_DIR}/src/rocketchatrestapi-qt5/restapimethod.h
    )

create_python_bindings(
    "RocketChatRestApiQt5"
    "${pyrocketchatrestapi-qt5_typesystem_paths}"
    "${pyrocketchatrestapi-qt5_include_paths}"
    "${pyrocketchatrestapi-qt5_SRC}"
    "${pyrocketchatrestapi-qt5_target_include_directories}"
    "${pyrocketchatrestapi-qt5_target_link_libraries}"
    ${CMAKE_CURRENT_SOURCE_DIR}/rocketchatrestapi-qt5_global.h
    ${CMAKE_CURRENT_SOURCE_DIR}/typesystem_rocketchatrestapi-qt5.xml
    "${pyrocketchatrestapi-qt5_DEPENDS}"
    ${CMAKE_CURRENT_BINARY_DIR}
    )

# Make module import from build dir works
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/__init__.py ${CMAKE_CURRENT_BINARY_DIR}/__init__.py)

#TODO define PYTHON_BINDINGS_INSTALL_PREFIX
# install
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py DESTINATION ${PYTHON_BINDINGS_INSTALL_PREFIX}/pyrocketchatrestapi-qt5)
