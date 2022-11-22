# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none
include(ECMAddQtDesignerPlugin)
ecm_qtdesignerplugin_widget(PimCommonTextTranslator::TranslatorWidget
    INCLUDE_FILE "PimCommonTextTranslator/TranslatorWidget"
    TOOLTIP "A widget to make translate string (KDE-PIM)"
    WHATSTHIS "A widget to make translate string."
    GROUP "Input (KDE-PIM)"
    )

ecm_add_qtdesignerplugin(pimcommonitexttranslatorwidgets
	NAME PimCommonTextTranslatorWidgets
    OUTPUT_NAME pimcommontexttranslatorwidgets
    WIDGETS
    PimCommonTextTranslator::TranslatorWidget
    LINK_LIBRARIES
    KF5::PimCommonTextTranslator
    INSTALL_DESTINATION "${KDE_INSTALL_QTPLUGINDIR}/designer"
    COMPONENT Devel
    )

