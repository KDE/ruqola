/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configureaccessibilitywidget.h"
#include "ruqolaglobalconfig.h"

#include <KLocalizedString>
#include <QCheckBox>
#include <QShowEvent>
#include <QVBoxLayout>
#include <TextEditTextToSpeech/TextToSpeechConfigWidget>

using namespace Qt::Literals::StringLiterals;
ConfigureAccessibilityWidget::ConfigureAccessibilityWidget(QWidget *parent)
    : QWidget{parent}
    , mTextToSpeechWidget(new TextEditTextToSpeech::TextToSpeechConfigWidget(this))
    , mEnableTextToSpeech(new QCheckBox(i18nc("@option:check", "Enable Text To Speech"), this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mEnableTextToSpeech->setObjectName(u"mEnableTextToSpeech"_s);
    mainLayout->addWidget(mEnableTextToSpeech);

    mTextToSpeechWidget->setObjectName(u"mTextToSpeechWidget"_s);
    mainLayout->addWidget(mTextToSpeechWidget);
}

ConfigureAccessibilityWidget::~ConfigureAccessibilityWidget() = default;

void ConfigureAccessibilityWidget::save()
{
    if (mWasInitialized) {
        mEnableTextToSpeech->setChecked(RuqolaGlobalConfig::self()->enableTextToSpeech());
        mTextToSpeechWidget->writeConfig();
    }
}

void ConfigureAccessibilityWidget::load()
{
    mTextToSpeechWidget->initializeSettings();
    RuqolaGlobalConfig::self()->setEnableTextToSpeech(mEnableTextToSpeech->isChecked());
}

void ConfigureAccessibilityWidget::restoreToDefaults()
{
    // TODO
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
