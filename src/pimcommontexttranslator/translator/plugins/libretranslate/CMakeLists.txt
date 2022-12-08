# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none

add_library(translator_libretranslate MODULE)
target_sources(translator_libretranslate PRIVATE
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
ecm_qt_declare_logging_category(translator_libretranslate
    HEADER libretranslatetranslator_debug.h
    IDENTIFIER TRANSLATOR_libretranslate
    CATEGORY_NAME org.kde.pim.pimcommontexttranslator.libretranslate
    DESCRIPTION "PimCommon libretranslate translator"
    EXPORT PIMCOMMON
)

target_link_libraries(translator_libretranslate PRIVATE KF5::PimCommonTextTranslator KF5::I18n KF5::ConfigCore)

install(TARGETS translator_libretranslate  DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf${QT_MAJOR_VERSION}/ruqola-translator/)

if (BUILD_TESTING)
    add_subdirectory(autotests)
endif()
