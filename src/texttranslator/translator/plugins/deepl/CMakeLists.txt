# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none
include_directories(${CMAKE_CURRENT_BINARY_DIR}/)
add_library(translator_deepl MODULE)
ecm_qt_declare_logging_category(translator_debug_deepl_SRCS
    HEADER deepltranslator_debug.h
    IDENTIFIER TRANSLATOR_DEEPL_LOG
    CATEGORY_NAME org.kde.kf5.texttranslator.deepl
    DESCRIPTION "deepl translator"
    EXPORT KTEXTADDONS
)
target_sources(translator_deepl PRIVATE
    ${translator_debug_deepl_SRCS}
    deeplengineclient.cpp
    deeplengineclient.h
    deeplengineplugin.cpp
    deeplengineplugin.h
    deeplengineconfiguredialog.h
    deeplengineconfiguredialog.cpp
    deeplengineconfigurewidget.h
    deeplengineconfigurewidget.cpp
    deeplengineutil.h
    deeplengineutil.cpp
)

target_link_libraries(translator_deepl PRIVATE KF5::TextTranslator KF5::I18n KF5::ConfigCore)

install(TARGETS translator_deepl  DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf${QT_MAJOR_VERSION}/translator/)
if (BUILD_TESTING)
    add_subdirectory(autotests)
endif()

