/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configureaccountwidget.h"
#include "configureaccountserverwidget.h"
#include <QVBoxLayout>

ConfigureAccountWidget::ConfigureAccountWidget(QWidget *parent)
    : QWidget(parent)
    , mConfigureAccountServerWidget(new ConfigureAccountServerWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mConfigureAccountServerWidget->setObjectName(QStringLiteral("mConfigureAccountServerWidget"));
    mainLayout->addWidget(mConfigureAccountServerWidget);
}

ConfigureAccountWidget::~ConfigureAccountWidget() = default;

void ConfigureAccountWidget::save()
{
    mConfigureAccountServerWidget->save();
}

void ConfigureAccountWidget::load()
{
    mConfigureAccountServerWidget->load();
}
