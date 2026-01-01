/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configureaccessibilitywidget.h"
#include "ruqolaglobalconfig.h"

#include <KLocalizedString>
#include <QCheckBox>
#include <QShowEvent>
#include <QVBoxLayout>
#include <TextEditTextToSpeech/TextToSpeech>
#include <TextEditTextToSpeech/TextToSpeechConfigWidget>

using namespace Qt::Literals::StringLiterals;
ConfigureAccessibilityWidget::ConfigureAccessibilityWidget(QWidget *parent)
    : QWidget{parent}
    , mTextToSpeechWidget(new TextEditTextToSpeech::TextToSpeechConfigWidget(this))
    , mEnableTextToSpeech(new QCheckBox(i18nc("@option:check", "Enable Text To Speech"), this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mEnableTextToSpeech->setObjectName(u"mEnableTextToSpeech"_s);
    mainLayout->addWidget(mEnableTextToSpeech);

    mTextToSpeechWidget->setObjectName(u"mTextToSpeechWidget"_s);
    mainLayout->addWidget(mTextToSpeechWidget);
    connect(mEnableTextToSpeech, &QCheckBox::clicked, mTextToSpeechWidget, &TextEditTextToSpeech::TextToSpeechConfigWidget::setEnabled);
}

ConfigureAccessibilityWidget::~ConfigureAccessibilityWidget() = default;

void ConfigureAccessibilityWidget::save()
{
    if (mWasInitialized) {
        RuqolaGlobalConfig::self()->setEnableTextToSpeech(mEnableTextToSpeech->isChecked());
        mTextToSpeechWidget->writeConfig();
        TextEditTextToSpeech::TextToSpeech::self()->reloadSettings();
    }
}

void ConfigureAccessibilityWidget::load()
{
    mTextToSpeechWidget->initializeSettings();
    mEnableTextToSpeech->setChecked(RuqolaGlobalConfig::self()->enableTextToSpeech());
    mTextToSpeechWidget->setEnabled(mEnableTextToSpeech->isChecked());
}

void ConfigureAccessibilityWidget::restoreToDefaults()
{
    const bool bUseDefaults = RuqolaGlobalConfig::self()->useDefaults(true);
    const bool enableTextToSpeech = RuqolaGlobalConfig::self()->enableTextToSpeech();
    mEnableTextToSpeech->setChecked(enableTextToSpeech);
    RuqolaGlobalConfig::self()->useDefaults(bUseDefaults);
    mTextToSpeechWidget->restoreDefaults();
}

void ConfigureAccessibilityWidget::showEvent(QShowEvent *event)
{
    if (!event->spontaneous() && !mWasInitialized) {
        mWasInitialized = true;
        mTextToSpeechWidget->readConfig();
    }
    QWidget::showEvent(event);
}

#include "moc_configureaccessibilitywidget.cpp"
