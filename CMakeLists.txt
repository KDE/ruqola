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
include(ECMAddTests)
include(GenerateExportHeader)
include(FeatureSummary)
# Use i18n ?
include(ECMPoQmTools)

set(KF5_VERSION "5.37.0")
option(RUN_UNINSTALL "test run uninstalled apps (experimental)" FALSE)

set(ECM_VERSION ${KF5_VERSION})
# ECM setup
if (RUN_UNINSTALL)
   set(ECM_VERSION "5.38.0")
endif()

find_package(ECM ${ECM_VERSION} CONFIG REQUIRED)

set(REQUIRED_QT_VERSION "5.7.0")

find_package(Qt5 ${REQUIRED_QT_VERSION} REQUIRED COMPONENTS Core Gui Widgets Qml Quick WebSockets Network NetworkAuth Test)

find_package(KF5 ${KF5_VERSION} REQUIRED COMPONENTS
    Kirigami2
    CoreAddons
    I18n
)


include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_definitions("-DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII")
add_definitions(-DQT_NO_NARROWING_CONVERSIONS_IN_CONNECT)
add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x060000)
add_definitions(-DQT_NO_URL_CAST_FROM_STRING)


# TODO customize it.
set(RUQOLA_VERSION "0.3.3")

set(RUQOLA_LIB_VERSION "${RUQOLA_VERSION}")
set(RUQOLA_LIB_SOVERSION "0")


configure_file(config-ruqola.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-ruqola.h)

if(BUILD_TESTING)
   add_definitions(-DBUILD_TESTING)
endif(BUILD_TESTING)


add_subdirectory(src)

if (BUILD_TESTING)
    add_subdirectory(autotests)
    add_subdirectory(tests)
endif()

if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/po")
    ecm_install_po_files_as_qm(po)
endif()

install(FILES ruqola.categories DESTINATION ${KDE_INSTALL_CONFDIR})

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
