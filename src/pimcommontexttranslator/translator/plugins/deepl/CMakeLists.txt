# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none

add_library(translator_deepl MODULE ${deepltranslator_PART_SRCS})
target_sources(translator_deepl PRIVATE
    deeplengineclient.cpp
    deeplengineclient.h
    deeplengineplugin.cpp
    deeplengineplugin.h
)
ecm_qt_declare_logging_category(translator_deepl
    HEADER deepltranslator_debug.h
    IDENTIFIER TRANSLATOR_deepl
    CATEGORY_NAME org.kde.pim.pimcommontexttranslator.deepl
    DESCRIPTION "PimCommon deepl translator"
    EXPORT PIMCOMMON
)

target_link_libraries(translator_deepl PRIVATE KF5::PimCommonTextTranslator KF5::I18n KF5::ConfigCore)

install(TARGETS translator_deepl  DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf${QT_MAJOR_VERSION}/ruqola-translator/)
