# SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
set(RUQOLA_VERSION "1.9.64")
project(Ruqola VERSION ${RUQOLA_VERSION})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(KF_MIN_VERSION "5.100.0")

# Do NOT add quote
set(RUQOLA_DEV_VERSION alpha)

# add an extra space
if(DEFINED RUQOLA_DEV_VERSION)
    set(RUQOLA_DEV_VERSION " ${RUQOLA_DEV_VERSION}")
endif()

set(RUQOLA_VERSION_STRING "${RUQOLA_VERSION}${RUQOLA_DEV_VERSION}")


find_package(ECM ${KF_MIN_VERSION} CONFIG REQUIRED)
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
include(ECMGenerateHeaders)
include(ECMSetupVersion)

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

option(EXPERIMENTAL_DATABASE_SUPPORT "Experimental database support (experimental)" FALSE)
if (EXPERIMENTAL_DATABASE_SUPPORT)
	set(HAVE_DATABASE_SUPPORT TRUE)
endif()

if (NOT WIN32 AND NOT APPLE)
    option(UNITY_SUPPORT "Build unity support" TRUE)
else()
    set(UNITY_SUPPORT OFF)
endif()

if (UNITY_SUPPORT)
    add_definitions(-DUNITY_SUPPORT)
endif()
set(KTEXTADDONS_MIN_VERSION "1.2.1")
set(REQUIRED_QT_VERSION "5.15.2")

if (QT_MAJOR_VERSION STREQUAL "6")
    set(QT_REQUIRED_VERSION "6.4.0")
    set(KF_MIN_VERSION "5.240.0")
    set(KF_MAJOR_VERSION "6")
    set(KTEXTADDONS_MIN_VERSION "1.3.0")
else()
    set(KF_MAJOR_VERSION "5")
endif()

find_package(Qt${QT_MAJOR_VERSION} ${REQUIRED_QT_VERSION} REQUIRED COMPONENTS Core Gui Widgets WebSockets Network NetworkAuth MultimediaWidgets Sql)

find_package(KF${KF_MAJOR_VERSION} ${KF_MIN_VERSION} REQUIRED COMPONENTS
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
    Archive
)


find_package(KF${KF_MAJOR_VERSION}TextTranslator ${KTEXTADDONS_MIN_VERSION} CONFIG)
set_package_properties(KF${KF_MAJOR_VERSION}TextTranslator PROPERTIES DESCRIPTION
    "Add support for text translation (ktextaddons)"
    TYPE OPTIONAL
)
if (TARGET KF${KF_MAJOR_VERSION}::TextTranslator)
    set(HAVE_TEXT_TRANSLATOR TRUE)
endif()

find_package(KF${KF_MAJOR_VERSION}TextAutoCorrectionWidgets ${KTEXTADDONS_MIN_VERSION} CONFIG)
set_package_properties(KF${KF_MAJOR_VERSION}TextAutoCorrectionWidgets PROPERTIES DESCRIPTION
    "Add support for text auto correction (ktextaddons)"
    TYPE OPTIONAL
)
if (TARGET KF${KF_MAJOR_VERSION}::TextAutoCorrectionWidgets)
    set(HAVE_TEXT_AUTOCORRECTION_WIDGETS TRUE)
else()
    find_package(KF${KF_MAJOR_VERSION}TextAutoCorrection ${KTEXTADDONS_MIN_VERSION} CONFIG)
    set_package_properties(KF${KF_MAJOR_VERSION}TextAutoCorrection PROPERTIES DESCRIPTION
        "Add support for text auto correction (ktextaddons)"
        TYPE OPTIONAL
    )
    if (TARGET KF${KF_MAJOR_VERSION}::TextAutoCorrection)
        set(HAVE_TEXT_AUTOCORRECTION TRUE)
    endif()
endif()

find_package(KF${KF_MAJOR_VERSION}TextEditTextToSpeech ${KTEXTADDONS_MIN_VERSION} CONFIG)
set_package_properties(KF${KF_MAJOR_VERSION}TextEditTextToSpeech PROPERTIES DESCRIPTION
    "Add support for text to speech (ktextaddons)"
    TYPE OPTIONAL
)
if (TARGET KF${KF_MAJOR_VERSION}::TextEditTextToSpeech)
    set(HAVE_TEXT_TO_SPEECH TRUE)
endif()

find_package(KF${KF_MAJOR_VERSION}TextEmoticonsWidgets ${KTEXTADDONS_MIN_VERSION} CONFIG)
set_package_properties(KF${KF_MAJOR_VERSION}TextEmoticonsWidgets PROPERTIES DESCRIPTION
    "Add text emoticons support (ktextaddons)"
    TYPE OPTIONAL
)

if (NOT WIN32 AND NOT APPLE)
    find_package(KF${KF_MAJOR_VERSION}DBusAddons ${KF_MIN_VERSION} CONFIG REQUIRED)
endif()


find_package(KF${KF_MAJOR_VERSION}XmlGui ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF${KF_MAJOR_VERSION}Config ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF${KF_MAJOR_VERSION}KIO ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF${KF_MAJOR_VERSION}Sonnet ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF${KF_MAJOR_VERSION}TextWidgets ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF${KF_MAJOR_VERSION}Purpose ${KF_MIN_VERSION} CONFIG)

find_package(KF${KF_MAJOR_VERSION}DocTools ${KF_MIN_VERSION})
set_package_properties(KF${KF_MAJOR_VERSION}DocTools PROPERTIES DESCRIPTION
    "Tools to generate documentation"
    TYPE OPTIONAL
)


find_package(Qt${QT_MAJOR_VERSION}Keychain CONFIG)
set_package_properties(Qt${QT_MAJOR_VERSION}Keychain PROPERTIES
                                   DESCRIPTION "Provides support for secure credentials storage"
                                   URL "https://github.com/frankosterfeld/qtkeychain"
                                   TYPE REQUIRED)


set(KUSERFEEDBACK_SUFFIX)
if (QT_MAJOR_VERSION STREQUAL "6")
    set(KUSERFEEDBACK_SUFFIX "Qt6")
    find_package(KUserFeedback${KUSERFEEDBACK_SUFFIX} 1.2.1 CONFIG)
    add_definitions(-DUSE_KUSERFEEDBACK_QT6)
else()
    find_package(KUserFeedback${KUSERFEEDBACK_SUFFIX} 1.2.0 CONFIG)
endif()
set(HAVE_KUSERFEEDBACK ${KUserFeedback${KUSERFEEDBACK_SUFFIX}_FOUND})
set_package_properties(KUserFeedback${KUSERFEEDBACK_SUFFIX} PROPERTIES DESCRIPTION "User Feedback lib" TYPE OPTIONAL PURPOSE "Allow to send Telemetry Information (optional).")

find_package(KF${KF_MAJOR_VERSION}Solid ${KF_MIN_VERSION} CONFIG)
if (KF${KF_MAJOR_VERSION}Solid_FOUND)
# SolidPower is not built by default, and the only way to find is to
# check for the header and that it builds
    get_target_property(SOLID_INCLUDE_DIRS KF${KF_MAJOR_VERSION}::Solid INTERFACE_INCLUDE_DIRECTORIES)
    find_file(SOLID_POWER_FOUND Solid/Power PATHS ${SOLID_INCLUDE_DIRS} NO_DEFAULT_PATH)
    if (SOLID_POWER_FOUND)
        message(STATUS "Found Solid Power header at ${SOLID_POWER_FOUND}")
        set(HAVE_SOLID 1)
    else()
        message(WARNING "Solid Power header was not found, suspend/resume detection is disabled. Reconfigure Solid build with `cmake -DWITH_NEW_SOLID_JOB=ON -DWITH_NEW_POWER_ASYNC_API=ON -DWITH_NEW_POWER_ASYNC_FREEDESKTOP=ON .`")
    endif()
endif()
set_package_properties(KF${KF_MAJOR_VERSION}Solid PROPERTIES DESCRIPTION "Device integration framework" TYPE OPTIONAL PURPOSE "Allows detecting suspend and resume.")

find_package(KF${KF_MAJOR_VERSION}NetworkManagerQt ${KF_MIN_VERSION} CONFIG)
set(HAVE_NETWORKMANAGER ${KF${KF_MAJOR_VERSION}NetworkManagerQt_FOUND})
set_package_properties(KF${KF_MAJOR_VERSION}NetworkManagerQt PROPERTIES DESCRIPTION "Network management lib" TYPE OPTIONAL PURPOSE "Allows detecting network state changes.")


include_directories(${CMAKE_CURRENT_BINARY_DIR})
ecm_set_disabled_deprecation_versions(QT 6.5
    KF 5.105.0
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
if (KF${KF_MAJOR_VERSION}DocTools_FOUND)
    kdoctools_install(po)
    add_subdirectory(doc)
endif()
feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
