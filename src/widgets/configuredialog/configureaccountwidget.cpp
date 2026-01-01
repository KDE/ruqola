/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configureaccountwidget.h"
using namespace Qt::Literals::StringLiterals;

#include "configureaccountserverwidget.h"
#include <QVBoxLayout>

ConfigureAccountWidget::ConfigureAccountWidget(QWidget *parent)
    : QWidget(parent)
    , mConfigureAccountServerWidget(new ConfigureAccountServerWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mConfigureAccountServerWidget->setObjectName(u"mConfigureAccountServerWidget"_s);
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

#include "moc_configureaccountwidget.cpp"
