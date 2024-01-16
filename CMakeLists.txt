# SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
set(RUQOLA_VERSION "2.0.80")
# TODO: update it in each release version
set(RUQOLA_RELEASE_VERSION "24.01.09")

project(Ruqola VERSION ${RUQOLA_VERSION})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(KF_MIN_VERSION "5.108.0")

# Do NOT add quote
set(RUQOLA_DEV_VERSION beta1)

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
option(OPTION_SELENIUMWEBDRIVER_SUPPORT "Add selenium driver test (experimental)" OFF)
option(OPTION_USE_SIZEHINT_CACHE_SUPPORT "Add sizehint cache" ON)

if (OPTION_USE_SIZEHINT_CACHE_SUPPORT)
    set(USE_SIZEHINT_CACHE_SUPPORT true)
endif()

if (OPTION_BUILD_PYTHON_BINDINGS)
   set(CMAKE_MODULE_PATH ${Ruqola_SOURCE_DIR}/cmake/Python ${ECM_MODULE_PATH})
endif()   

option(USE_UNITY_CMAKE_SUPPORT "Use UNITY cmake support (speedup compile time)" OFF)

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

option(PLUGINS_AUTHENTICATION_BASED_ON_O2 "Build authentication based on o2 lib (experimental)" OFF)
if (NOT WIN32 AND NOT APPLE)
    option(UNITY_SUPPORT "Build unity support" ON)
else()
    set(UNITY_SUPPORT OFF)
endif()

if (UNITY_SUPPORT)
    add_definitions(-DUNITY_SUPPORT)
endif()
set(KTEXTADDONS_MIN_VERSION "1.5.2")
set(REQUIRED_QT_VERSION "5.15.2")

set(KLLMWIDGETS_VERSION "0.1.0")

if (QT_MAJOR_VERSION STREQUAL "6")
    set(QT_REQUIRED_VERSION "6.6.0")
    set(KF_MIN_VERSION "5.246.0")
    set(KF_MAJOR_VERSION "6")
    set(KTEXTADDONS_MIN_VERSION "1.5.3")
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
    Codecs
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

if (${KF${KF_MAJOR_VERSION}TextEmoticonsWidgets_VERSION} VERSION_GREATER_EQUAL 1.5.44)
    set(HAVE_TEXTADDONS_TEXTEMOTICON_EXCLUDEEMOTICON_SUPPORT TRUE)
else()
    if (${KF${KF_MAJOR_VERSION}TextEmoticonsWidgets_VERSION} VERSION_GREATER_EQUAL 1.5.3)
        if (${KF${KF_MAJOR_VERSION}TextEmoticonsWidgets_VERSION} VERSION_LESS_EQUAL 1.5.30)
            set(HAVE_TEXTADDONS_TEXTEMOTICON_EXCLUDEEMOTICON_SUPPORT TRUE)
        endif()
    endif()
endif()

find_package(KF${KF_MAJOR_VERSION}TextUtils ${KTEXTADDONS_MIN_VERSION} CONFIG)
set_package_properties(KF${KF_MAJOR_VERSION}TextUtils PROPERTIES DESCRIPTION
    "Add utils text functions (ktextaddons)"
    TYPE OPTIONAL
)

if (TARGET KF${KF_MAJOR_VERSION}::TextUtils)
   set(HAVE_TEXT_UTILS TRUE)
endif()


find_package(KF${KF_MAJOR_VERSION}TextCustomEditor ${KTEXTADDONS_MIN_VERSION} CONFIG)
set_package_properties(KF${KF_MAJOR_VERSION}TextCustomEditor PROPERTIES DESCRIPTION
    "Add text custom editor (ktextaddons)"
    TYPE OPTIONAL
)

if (TARGET KF${KF_MAJOR_VERSION}::TextCustomEditor)
   set(HAVE_TEXT_CUSTOM_EDITOR TRUE)
endif()

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

find_package(KLLMWidgets ${KLLMWIDGETS_VERSION})
set_package_properties(KLLMWidgets PROPERTIES
    TYPE RECOMMENDED
    PURPOSE "Required for building IA plugins support"
    DESCRIPTION "Use local IA plugin"
    URL https://invent.kde.org/lorendb/kandalf/
)

if (QT_MAJOR_VERSION STREQUAL "6")
    find_package(KF6StatusNotifierItem ${KF_MIN_VERSION} REQUIRED)
endif()

find_package(Qt${QT_MAJOR_VERSION}Keychain CONFIG)
set_package_properties(Qt${QT_MAJOR_VERSION}Keychain PROPERTIES
                                   DESCRIPTION "Provides support for secure credentials storage"
                                   URL "https://github.com/frankosterfeld/qtkeychain"
                                   TYPE REQUIRED)

if (Qt${QT_MAJOR_VERSION}Keychain_VERSION VERSION_LESS 0.14.2)
    if (QT_MAJOR_VERSION STREQUAL "6")
        MESSAGE(STATUS "Qt${QT_MAJOR_VERSION}KeyChain version is less 0.14.2. It will not support KWallet 6... It will not able to load password. Please update it.")
    endif()
endif()

if (OPTION_SELENIUMWEBDRIVER_SUPPORT)
    find_package(SeleniumWebDriverATSPI)
    set_package_properties(SeleniumWebDriverATSPI PROPERTIES PURPOSE "Needed for GUI tests" URL "https://invent.kde.org/sdk/selenium-webdriver-at-spi" TYPE OPTIONAL)
endif()

if (QT_MAJOR_VERSION STREQUAL "6")
    find_package(KF6UserFeedback ${KF_MIN_VERSION} CONFIG)
    set(HAVE_KUSERFEEDBACK ${KF6UserFeedback_FOUND})
    set_package_properties(KF6UserFeedback PROPERTIES DESCRIPTION "User Feedback lib" TYPE OPTIONAL PURPOSE "Allow to send Telemetry Information (optional).")
else()
    find_package(KUserFeedback 1.2.0 CONFIG)
    set(HAVE_KUSERFEEDBACK ${KUserFeedback_FOUND})
    set_package_properties(KUserFeedback PROPERTIES DESCRIPTION "User Feedback lib" TYPE OPTIONAL PURPOSE "Allow to send Telemetry Information (optional).")
endif()

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
    KF 5.248.0
    )
if (OPTION_BUILD_PYTHON_BINDINGS)
   remove_definitions(-DQT_STRICT_ITERATORS)
endif()       	
add_definitions(-DQT_NO_FOREACH)
add_definitions(-DQT_NO_KEYWORDS)

set(RUQOLA_LIB_VERSION "${RUQOLA_VERSION}")
set(RUQOLA_LIB_SOVERSION "0")


configure_file(config-ruqola.h.in ${CMAKE_CURRENT_BINARY_DIR}/config-ruqola.h)

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
