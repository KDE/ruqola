# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none

add_library(translator_google MODULE ${googletranslator_PART_SRCS})
target_sources(translator_google PRIVATE
    googleengineclient.h
    googleengineclient.cpp
    googleengineplugin.h
    googleengineplugin.cpp
)
ecm_qt_declare_logging_category(translator_google
    HEADER googletranslator_debug.h
    IDENTIFIER TRANSLATOR_GOOGLE
    CATEGORY_NAME org.kde.pim.pimcommontexttranslator.google
    DESCRIPTION "PimCommon google translator"
    EXPORT PIMCOMMON
)

target_link_libraries(translator_google PRIVATE KF5::PimCommonTextTranslator KF5::I18n)

install(TARGETS translator_google  DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf${QT_MAJOR_VERSION}/translator/)
