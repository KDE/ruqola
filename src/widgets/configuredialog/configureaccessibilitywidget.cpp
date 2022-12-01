/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configureaccessibilitywidget.h"
#include <KPIMTextEditTextToSpeech/TextToSpeechConfigWidget>

#include <QVBoxLayout>

ConfigureAccessibilityWidget::ConfigureAccessibilityWidget(QWidget *parent)
    : QWidget{parent}
    , mTextToSpeechWidget(new KPIMTextEditTextToSpeech::TextToSpeechConfigWidget(this))
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
    // TODO
}

void ConfigureAccessibilityWidget::load()
{
    // TODO
}
