/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configurespellcheckingwidget.h"
#include <QVBoxLayout>
#include <Sonnet/ConfigWidget>

ConfigureSpellCheckingWidget::ConfigureSpellCheckingWidget(QWidget *parent)
    : QWidget(parent)
    , mConfigWidget(new Sonnet::ConfigWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});
    mConfigWidget->setObjectName(QStringLiteral("mConfigWidget"));
    mainLayout->addWidget(mConfigWidget);
}

ConfigureSpellCheckingWidget::~ConfigureSpellCheckingWidget() = default;

void ConfigureSpellCheckingWidget::save()
{
    mConfigWidget->save();
}

void ConfigureSpellCheckingWidget::load()
{
    // Nothing
}

#include "moc_configurespellcheckingwidget.cpp"
