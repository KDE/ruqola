/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configureaccessibilitytabwidget.h"
#include "config-ruqola.h"
#if HAVE_TEXT_TO_SPEECH
#include "configureaccessibilitywidget.h"
#endif
#if HAVE_SPEECH_TO_TEXT
#include <TextSpeechToText/SpeechToTextConfigureWidget>
#endif
#include <KLocalizedString>

ConfigureAccessibilityTabWidget::ConfigureAccessibilityTabWidget(QWidget *parent)
    : QTabWidget(parent)
#if HAVE_TEXT_TO_SPEECH
    , mConfigureAccessibilityWidget(new ConfigureAccessibilityWidget(this))
#endif
#if HAVE_SPEECH_TO_TEXT
    , mConfigureSpeechToTextWidget(new TextSpeechToText::SpeechToTextConfigureWidget(this))
#endif
{
    setTabBarAutoHide(true);
#if HAVE_TEXT_TO_SPEECH
    addTab(mConfigureAccessibilityWidget, i18n("Text to Speech"));
#endif
#if HAVE_SPEECH_TO_TEXT
    addTab(mConfigureSpeechToTextWidget, i18n("Speech to Text"));
#endif
}

ConfigureAccessibilityTabWidget::~ConfigureAccessibilityTabWidget() = default;

void ConfigureAccessibilityTabWidget::save()
{
#if HAVE_TEXT_TO_SPEECH
    mConfigureAccessibilityWidget->save();
#endif
#if HAVE_SPEECH_TO_TEXT
    mConfigureSpeechToTextWidget->saveSettings();
#endif
}

void ConfigureAccessibilityTabWidget::load()
{
#if HAVE_TEXT_TO_SPEECH
    mConfigureAccessibilityWidget->load();
#endif
#if HAVE_SPEECH_TO_TEXT
    mConfigureSpeechToTextWidget->loadSettings();
#endif
}

void ConfigureAccessibilityTabWidget::restoreToDefaults()
{
#if HAVE_TEXT_TO_SPEECH
    mConfigureAccessibilityWidget->restoreToDefaults();
#endif
#if HAVE_SPEECH_TO_TEXT
    // TODO mConfigureSpeechToTextWidget->restoreToDefaults();
#endif
}

#include "moc_configureaccessibilitytabwidget.cpp"
