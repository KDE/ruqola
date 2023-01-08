# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none
include(ECMAddQtDesignerPlugin)
ecm_qtdesignerplugin_widget(TextTranslator::TranslatorWidget
    INCLUDE_FILE "TextTranslator/TranslatorWidget"
    TOOLTIP "A widget to make translate string"
    WHATSTHIS "A widget to make translate string."
    GROUP "Input (kf5)"
    )

ecm_add_qtdesignerplugin(texttranslatorwidgets
	NAME TextTranslatorWidgets
    OUTPUT_NAME texttranslatorwidgets
    WIDGETS
    TextTranslator::TranslatorWidget
    LINK_LIBRARIES
    KF5::TextTranslator
    INSTALL_DESTINATION "${KDE_INSTALL_QTPLUGINDIR}/designer"
    COMPONENT Devel
    )

