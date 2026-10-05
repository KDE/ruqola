/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configurespeechtotextconfigurewidget.h"
#include "ruqolaglobalconfig.h"

#include <KLocalizedString>
#include <QCheckBox>
#include <QVBoxLayout>
#include <TextSpeechToText/SpeechToTextConfigureWidget>

using namespace Qt::Literals::StringLiterals;
ConfigureSpeechToTextConfigureWidget::ConfigureSpeechToTextConfigureWidget(QWidget *parent)
    : QWidget{parent}
    , mSpeechToTextWidget(new TextSpeechToText::SpeechToTextConfigureWidget(this))
    , mEnableSpeechToText(new QCheckBox(i18nc("@option:check", "Enable Speech To Text"), this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mEnableSpeechToText->setObjectName(u"mEnableSpeechToText"_s);
    mainLayout->addWidget(mEnableSpeechToText);

    mSpeechToTextWidget->setObjectName(u"mSpeechToTextWidget"_s);
    mainLayout->addWidget(mSpeechToTextWidget);
    connect(mEnableSpeechToText, &QCheckBox::clicked, mSpeechToTextWidget, &TextSpeechToText::SpeechToTextConfigureWidget::setEnabled);
}

ConfigureSpeechToTextConfigureWidget::~ConfigureSpeechToTextConfigureWidget() = default;

void ConfigureSpeechToTextConfigureWidget::save()
{
    RuqolaGlobalConfig::self()->setEnableSpeechToText(mEnableSpeechToText->isChecked());
    RuqolaGlobalConfig::self()->save();
    mSpeechToTextWidget->saveSettings();
}

void ConfigureSpeechToTextConfigureWidget::load()
{
    mSpeechToTextWidget->loadSettings();
    mEnableSpeechToText->setChecked(RuqolaGlobalConfig::self()->enableSpeechToText());
    mSpeechToTextWidget->setEnabled(mEnableSpeechToText->isChecked());
}

void ConfigureSpeechToTextConfigureWidget::restoreToDefaults()
{
    const bool bUseDefaults = RuqolaGlobalConfig::self()->useDefaults(true);
    const bool enableSpeechToText = RuqolaGlobalConfig::self()->enableSpeechToText();
    mEnableSpeechToText->setChecked(enableSpeechToText);
    mSpeechToTextWidget->setEnabled(enableSpeechToText);
    RuqolaGlobalConfig::self()->useDefaults(bUseDefaults);
}

#include "moc_configurespeechtotextconfigurewidget.cpp"
