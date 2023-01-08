# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none

include_directories(${CMAKE_CURRENT_BINARY_DIR}/)
add_library(translator_libretranslate MODULE)

ecm_qt_declare_logging_category(translator_libretranslate_debug_SRCS
    HEADER libretranslatetranslator_debug.h
    IDENTIFIER TRANSLATOR_LIBRETRANSLATE_LOG
    CATEGORY_NAME org.kde.kf5.texttranslator.libretranslate
    DESCRIPTION "libretranslate translator"
    EXPORT KTEXTADDONS
)

target_sources(translator_libretranslate PRIVATE
    ${translator_libretranslate_debug_SRCS}
    libretranslateengineclient.cpp
    libretranslateengineclient.h
    libretranslateengineplugin.cpp
    libretranslateengineplugin.h
    libretranslateengineconfiguredialog.h
    libretranslateengineconfiguredialog.cpp
    libretranslateengineconfigurewidget.h
    libretranslateengineconfigurewidget.cpp
    libretranslateengineutil.h
    libretranslateengineutil.cpp
)

target_link_libraries(translator_libretranslate PRIVATE KF5::TextTranslator KF5::I18n KF5::ConfigCore)

install(TARGETS translator_libretranslate  DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf${QT_MAJOR_VERSION}/translator/)

if (BUILD_TESTING)
    add_subdirectory(autotests)
endif()
