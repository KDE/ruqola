# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none

add_library(translator_bing MODULE)
target_sources(translator_bing PRIVATE
    bingengineclient.cpp
    bingengineclient.h
    bingengineplugin.cpp
    bingengineplugin.h
)
ecm_qt_declare_logging_category(translator_bing
    HEADER bingtranslator_debug.h
    IDENTIFIER TRANSLATOR_bing
    CATEGORY_NAME org.kde.pim.pimcommontexttranslator.bing
    DESCRIPTION "PimCommon bing translator"
    EXPORT PIMCOMMON
)

target_link_libraries(translator_bing PRIVATE KF5::PimCommonTextTranslator KF5::I18n)

install(TARGETS translator_bing  DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf${QT_MAJOR_VERSION}/ruqola-translator/)
if (BUILD_TESTING)
    add_subdirectory(autotests)
endif()

