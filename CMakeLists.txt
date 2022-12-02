# SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

set(RUQOLA_VERSION "1.8.52")
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(Ruqola VERSION ${RUQOLA_VERSION})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(KF5_MIN_VERSION "5.93.0")
set(REQUIRED_QT_VERSION "5.15.2")
if (QT_MAJOR_VERSION STREQUAL "6")
    set(QT_REQUIRED_VERSION "6.4.0")
endif()
# Do NOT add quote
set(RUQOLA_DEV_VERSION alpha)

# add an extra space
if(DEFINED RUQOLA_DEV_VERSION)
    set(RUQOLA_DEV_VERSION " ${RUQOLA_DEV_VERSION}")
endif()

set(RUQOLA_VERSION_STRING "${RUQOLA_VERSION}${RUQOLA_DEV_VERSION}")


find_package(ECM ${KF5_MIN_VERSION} CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${Ruqola_SOURCE_DIR}/cmake/modules ${ECM_MODULE_PATH})


include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(ECMInstallIcons)
include(ECMQtDeclareLoggingCategory)
include(ECMAddTests)
include(GenerateExportHeader)
include(FeatureSummary)
include(CheckIncludeFileCXX)
include(KDEGitCommitHooks)
include(ECMDeprecationSettings)
include(KDEClangFormat)
# remove when created a lib for libpimcommonautocorrection
include(ECMGenerateHeaders)

option(OPTION_BUILD_PYTHON_BINDINGS "Build python bindings (experimental)" OFF)

if (OPTION_BUILD_PYTHON_BINDINGS)
   set(CMAKE_MODULE_PATH ${Ruqola_SOURCE_DIR}/cmake/Python ${ECM_MODULE_PATH})
endif()   


option(USE_UNITY_CMAKE_SUPPORT "Use UNITY cmake support (speedup compile time)" FALSE)

set(COMPILE_WITH_UNITY_CMAKE_SUPPORT false)
if (USE_UNITY_CMAKE_SUPPORT)
    set(COMPILE_WITH_UNITY_CMAKE_SUPPORT true)
endif()
option(USE_PRECOMPILED_HEADERS "Use precompiled headers" OFF) # Set to OFF when using clazy and such
set(COMPILE_WITH_CMAKE_PCH_SUPPORT false)
macro(ruqola_target_precompile_headers)
    if (USE_PRECOMPILED_HEADERS)
        set(COMPILE_WITH_CMAKE_PCH_SUPPORT true)

        if (COMPILE_WITH_CMAKE_PCH_SUPPORT)
            target_precompile_headers(${ARGV})
        endif()
    endif()
endmacro()

option(PLUGINS_AUTHENTICATION_BASED_ON_O2 "Build authentication based on o2 lib (experimental)" FALSE)

if (NOT WIN32 AND NOT APPLE)
    option(UNITY_SUPPORT "Build unity support" TRUE)
else()
    set(UNITY_SUPPORT OFF)
endif()

if (UNITY_SUPPORT)
    add_definitions(-DUNITY_SUPPORT)
endif()

find_package(Qt${QT_MAJOR_VERSION} ${REQUIRED_QT_VERSION} REQUIRED COMPONENTS Core Gui Widgets WebSockets Network NetworkAuth MultimediaWidgets Sql)

find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    CoreAddons
    I18n
    Crash
    Notifications
    IconThemes
    SyntaxHighlighting
    NotifyConfig
    ItemViews
    IdleTime
    Prison
    # need by pimcommonautocorrection. Remove it after creating lib
    Archive
)

if (NOT WIN32 AND NOT APPLE)
    find_package(KF5DBusAddons ${KF5_MIN_VERSION} CONFIG REQUIRED)
endif()

find_package(KF5XmlGui ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5Config ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5KIO ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5Sonnet ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5TextWidgets ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5Purpose ${KF5_MIN_VERSION} CONFIG REQUIRED)

find_package(KF5DocTools ${KF5_MIN_VERSION})
set_package_properties(KF5DocTools PROPERTIES DESCRIPTION
    "Tools to generate documentation"
    TYPE OPTIONAL
)

find_package(Qt${QT_MAJOR_VERSION}TextToSpeech ${QT_REQUIRED_VERSION} CONFIG)
set_package_properties(Qt${QT_MAJOR_VERSION}TextToSpeech PROPERTIES DESCRIPTION
    "Add support for text to speech"
    TYPE OPTIONAL
)
# remove it when we will split lib
if(TARGET Qt${QT_MAJOR_VERSION}::TextToSpeech)
    add_definitions(-DHAVE_TEXT_TO_SPEECH_SUPPORT)
    set(KPIMTEXTEDIT_TEXT_TO_SPEECH TRUE)
endif()

find_package(Qt${QT_MAJOR_VERSION}Keychain CONFIG)
set_package_properties(Qt${QT_MAJOR_VERSION}Keychain PROPERTIES
                                   DESCRIPTION "Provides support for secure credentials storage"
                                   URL "https://github.com/frankosterfeld/qtkeychain"
                                   TYPE REQUIRED)

find_package(KUserFeedback 1.2.0 CONFIG)
set(HAVE_KUSERFEEDBACK ${KUserFeedback_FOUND})
set_package_properties(KUserFeedback PROPERTIES DESCRIPTION "User Feedback lib" TYPE OPTIONAL PURPOSE "Allow to send Telemetry Information (optional).")

find_package(KF5Solid ${KF5_MIN_VERSION} CONFIG)
if (KF5Solid_FOUND)
# SolidPower is not built by default, and the only way to find is to
# check for the header and that it builds
    get_target_property(SOLID_INCLUDE_DIRS KF5::Solid INTERFACE_INCLUDE_DIRECTORIES)
    find_file(SOLID_POWER_FOUND Solid/Power PATHS ${SOLID_INCLUDE_DIRS} NO_DEFAULT_PATH)
    if (SOLID_POWER_FOUND)
        message(STATUS "Found Solid Power header at ${SOLID_POWER_FOUND}")
        set(HAVE_SOLID 1)
    else()
        message(WARNING "Solid Power header was not found, suspend/resume detection is disabled. Reconfigure Solid build with `cmake -DWITH_NEW_SOLID_JOB=ON -DWITH_NEW_POWER_ASYNC_API=ON -DWITH_NEW_POWER_ASYNC_FREEDESKTOP=ON .`")
    endif()
endif()
set_package_properties(KF5Solid PROPERTIES DESCRIPTION "Device integration framework" TYPE OPTIONAL PURPOSE "Allows detecting suspend and resume.")

find_package(KF5NetworkManagerQt ${KF5_MIN_VERSION} CONFIG)
set(HAVE_NETWORKMANAGER ${KF5NetworkManagerQt_FOUND})
set_package_properties(KF5NetworkManagerQt PROPERTIES DESCRIPTION "Network management lib" TYPE OPTIONAL PURPOSE "Allows detecting network state changes.")


include_directories(${CMAKE_CURRENT_BINARY_DIR})
ecm_set_disabled_deprecation_versions(QT 5.15.2
    KF 6.0.0
    )
if (OPTION_BUILD_PYTHON_BINDINGS)
   remove_definitions(-DQT_STRICT_ITERATORS)
endif()       	
add_definitions(-DQT_NO_FOREACH)
add_definitions(-DQT_NO_KEYWORDS)

set(RUQOLA_LIB_VERSION "${RUQOLA_VERSION}")
set(RUQOLA_LIB_SOVERSION "0")


configure_file(config-ruqola.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-ruqola.h)

if(BUILD_TESTING)
    find_package(Qt${QT_MAJOR_VERSION} ${REQUIRED_QT_VERSION} REQUIRED COMPONENTS Test)
   add_definitions(-DBUILD_TESTING)
endif(BUILD_TESTING)

if (PLUGINS_AUTHENTICATION_BASED_ON_O2)
    find_package(o2)
endif()


add_subdirectory(src)

if (BUILD_TESTING)
    add_subdirectory(tests)
endif()


ecm_qt_install_logging_categories(
        EXPORT RUQOLA
        FILE ruqola.categories
        DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR}
        )
kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
ki18n_install(po)
if (KF5DocTools_FOUND)
    kdoctools_install(po)
    add_subdirectory(doc)
endif()
feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
