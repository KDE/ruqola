# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none
include_directories(${CMAKE_CURRENT_BINARY_DIR}/)
add_library(translator_lingva MODULE)

ecm_qt_declare_logging_category(translator_lingva_debug_SRCS
    HEADER lingvatranslator_debug.h
    IDENTIFIER TRANSLATOR_LINGVA_LOG
    CATEGORY_NAME org.kde.kf5.texttranslator.lingva
    DESCRIPTION "lingva translator"
    EXPORT KTEXTADDONS
)

target_sources(translator_lingva PRIVATE
    ${translator_lingva_debug_SRCS}
    lingvaengineclient.cpp
    lingvaengineclient.h
    lingvaengineplugin.cpp
    lingvaengineplugin.h
    lingvaenginedialog.h
    lingvaenginedialog.cpp
    lingvaenginewidget.h
    lingvaenginewidget.cpp
    lingvaengineutil.h
    lingvaengineutil.cpp
)

target_link_libraries(translator_lingva PRIVATE KF5::TextTranslator KF5::I18n KF5::ConfigCore)

install(TARGETS translator_lingva  DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf${QT_MAJOR_VERSION}/translator/)
if (BUILD_TESTING)
    add_subdirectory(autotests)
endif()
