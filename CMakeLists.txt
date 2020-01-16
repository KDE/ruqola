set(RUQOLA_VERSION "1.0.85")
cmake_minimum_required(VERSION 3.5)
project(Ruqola VERSION ${RUQOLA_VERSION})
set (CMAKE_CXX_STANDARD 11)

set(KF5_MIN_VERSION "5.62.0")
set(REQUIRED_QT_VERSION "5.13.0")

# Do NOT add quote
set(RUQOLA_DEV_VERSION beta1)

# add an extra space
if(DEFINED RUQOLA_DEV_VERSION)
	set(RUQOLA_DEV_VERSION " ${RUQOLA_DEV_VERSION}")
endif()

set(RUQOLA_VERSION_STRING "${RUQOLA_VERSION}${RUQOLA_DEV_VERSION}")


find_package(ECM ${KF5_MIN_VERSION} CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${Ruqola_SOURCE_DIR}/cmake/modules ${ECM_MODULE_PATH})

if (POLICY CMP0063)
    cmake_policy(SET CMP0063 NEW)
endif()

if (POLICY CMP0071)
    cmake_policy(SET CMP0071 NEW)
endif()

include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(ECMInstallIcons)
include(ECMQtDeclareLoggingCategory)
include(ECMAddTests)
include(GenerateExportHeader)
include(FeatureSummary)

option(DISABLE_QTQUICK_COMPILER "Disable using qtquick compiler (true)" TRUE)
option(PLUGINS_AUTHENTICATION_BASED_ON_O2 "Build authentication based on o2 lib (experimental)" FALSE)
if (NOT WIN32)
    option(UNITY_SUPPORT "Build unity support" TRUE)
endif()

if (UNITY_SUPPORT)
    add_definitions(-DUNITY_SUPPORT)
endif()

find_package(Qt5 ${REQUIRED_QT_VERSION} REQUIRED COMPONENTS Core Gui Widgets Qml Quick WebSockets Network NetworkAuth Test)
find_package(Qt5 CONFIG QUIET OPTIONAL_COMPONENTS QuickCompiler)

find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    Kirigami2
    CoreAddons
    I18n
    Crash
    Notifications
    SyntaxHighlighting
    DocTools
)

if (WIN32 OR APPLE)
    find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
        IconThemes
    )
endif()

find_package(KF5DBusAddons ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5XmlGui ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5Config ${KF5_MIN_VERSION} CONFIG REQUIRED)

find_package(Qt5Keychain)
set_package_properties(Qt5Keychain PROPERTIES
                                   DESCRIPTION "Provides support for secure credentials storage"
                                   URL "https://github.com/frankosterfeld/qtkeychain"
                                   TYPE OPTIONAL)
if (Qt5Keychain_FOUND)
    set(HAVE_QT5KEYCHAIN 1)
else()
    set(HAVE_QT5KEYCHAIN 0)
endif()

include_directories(${CMAKE_CURRENT_BINARY_DIR})
if (EXISTS "${CMAKE_SOURCE_DIR}/.git")
   add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x060000)
   add_definitions(-DKF_DISABLE_DEPRECATED_BEFORE_AND_AT=0x060000)
endif()
add_definitions(-DQT_NO_FOREACH)


set(RUQOLA_LIB_VERSION "${RUQOLA_VERSION}")
set(RUQOLA_LIB_SOVERSION "0")


configure_file(config-ruqola.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-ruqola.h)

if(BUILD_TESTING)
   add_definitions(-DBUILD_TESTING)
endif(BUILD_TESTING)

if (PLUGINS_AUTHENTICATION_BASED_ON_O2)
    find_package(o2)
endif()


add_subdirectory(src)

if (BUILD_TESTING)
    add_subdirectory(tests)
endif()

add_subdirectory(doc)
install(FILES ruqola.categories DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR})

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
