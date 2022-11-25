# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none

add_library(translator_lingva MODULE ${lingvatranslator_PART_SRCS})
target_sources(translator_lingva PRIVATE
    lingvaengineclient.cpp
    lingvaengineclient.h
    lingvaengineplugin.cpp
    lingvaengineplugin.h
)
ecm_qt_declare_logging_category(translator_lingva
    HEADER lingvatranslator_debug.h
    IDENTIFIER TRANSLATOR_lingva
    CATEGORY_NAME org.kde.pim.pimcommontexttranslator.lingva
    DESCRIPTION "PimCommon lingva translator"
    EXPORT PIMCOMMON
)

target_link_libraries(translator_lingva PRIVATE KF5::PimCommonTextTranslator KF5::I18n KF5::ConfigCore)

install(TARGETS translator_lingva  DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf${QT_MAJOR_VERSION}/translator/)
