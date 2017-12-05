set(RUQOLA_VERSION "0.4.7")
cmake_minimum_required(VERSION 3.1)
project(Ruqola VERSION ${RUQOLA_VERSION})
set (CMAKE_CXX_STANDARD 11)

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

set(KF5_VERSION "5.39.0")

find_package(ECM ${KF5_VERSION} CONFIG REQUIRED)

set(REQUIRED_QT_VERSION "5.9.0")

find_package(Qt5 ${REQUIRED_QT_VERSION} REQUIRED COMPONENTS Core Gui Widgets Qml Quick WebSockets Network NetworkAuth Test)

find_package(KF5 ${KF5_VERSION} REQUIRED COMPONENTS
    Kirigami2
    CoreAddons
    I18n
    Crash
    KIO
    Notifications
)

if (WIN32 OR APPLE)
    find_package(KF5 ${KF5_VERSION} REQUIRED COMPONENTS
        IconThemes
    )
endif()

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_definitions("-DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII")
add_definitions(-DQT_NO_NARROWING_CONVERSIONS_IN_CONNECT)
add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x060000)
add_definitions(-DQT_NO_URL_CAST_FROM_STRING)



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

install(FILES ruqola.categories DESTINATION ${KDE_INSTALL_CONFDIR})

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
