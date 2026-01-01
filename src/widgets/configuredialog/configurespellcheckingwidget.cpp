/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configurespellcheckingwidget.h"
using namespace Qt::Literals::StringLiterals;

#include <QVBoxLayout>
#include <Sonnet/ConfigWidget>

ConfigureSpellCheckingWidget::ConfigureSpellCheckingWidget(QWidget *parent)
    : QWidget(parent)
    , mConfigWidget(new Sonnet::ConfigWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
    mConfigWidget->setObjectName(u"mConfigWidget"_s);
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

void ConfigureSpellCheckingWidget::restoreToDefaults()
{
    mConfigWidget->slotDefault();
}

#include "moc_configurespellcheckingwidget.cpp"
