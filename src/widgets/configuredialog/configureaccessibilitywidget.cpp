/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configureaccessibilitywidget.h"
#include "textedittexttospeech_version.h"
#include <TextEditTextToSpeech/TextToSpeechConfigWidget>

#include <QShowEvent>
#include <QVBoxLayout>

ConfigureAccessibilityWidget::ConfigureAccessibilityWidget(QWidget *parent)
    : QWidget{parent}
    , mTextToSpeechWidget(new TextEditTextToSpeech::TextToSpeechConfigWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mTextToSpeechWidget->setObjectName(QStringLiteral("mTextToSpeechWidget"));
    mainLayout->addWidget(mTextToSpeechWidget);
}

ConfigureAccessibilityWidget::~ConfigureAccessibilityWidget() = default;

void ConfigureAccessibilityWidget::save()
{
    if (mWasInitialized) {
        mTextToSpeechWidget->writeConfig();
    }
}

void ConfigureAccessibilityWidget::load()
{
#if TEXTEDITTEXTTOSPEECH_VERSION >= QT_VERSION_CHECK(1, 4, 46)
    mTextToSpeechWidget->initializeSettings();
#endif
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
