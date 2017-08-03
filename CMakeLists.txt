project(Ruqola)
cmake_minimum_required(VERSION 3.0)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

cmake_policy(SET CMP0063 NEW)

include(KDECompilerSettings)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(ECMInstallIcons)
include(ECMQtDeclareLoggingCategory)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUXX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")
endif()

set(REQUIRED_QT_VERSION "5.7.0")

find_package(Qt5 ${REQUIRED_QT_VERSION} REQUIRED COMPONENTS Core Gui Widgets Qml Quick WebSockets Network NetworkAuth)

find_package(KF5 REQUIRED COMPONENTS
    Kirigami2
    CoreAddons
)

include(ECMPoQmTools)
# 
include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

add_definitions("-DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII")
add_definitions(-DQT_NO_NARROWING_CONVERSIONS_IN_CONNECT)
add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x060000)
add_definitions(-DQT_NO_URL_CAST_FROM_STRING)

# TODO customize it.
set(RUQOLA_VERSION "0.1")

configure_file(config-ruqola.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-ruqola.h)

add_subdirectory(src)

if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/po")
    ecm_install_po_files_as_qm(po)
endif()
install(FILES ruqola.categories DESTINATION ${KDE_INSTALL_CONFDIR})

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
