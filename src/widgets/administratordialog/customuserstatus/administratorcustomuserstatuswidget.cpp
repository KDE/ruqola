/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorcustomuserstatuswidget.h"
using namespace Qt::Literals::StringLiterals;

#include "customuserstatustreewidget.h"

#include <KTreeWidgetSearchLineWidget>
#include <QVBoxLayout>
AdministratorCustomUserStatusWidget::AdministratorCustomUserStatusWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mCustomUserStatusTreeWidget(new CustomUserStatusTreeWidget(account, this))
    , mSearchLineWidget(new KTreeWidgetSearchLineWidget(this, mCustomUserStatusTreeWidget))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);
    mSearchLineWidget->setObjectName(u"mSearchLineWidget"_s);
    mainLayout->addWidget(mSearchLineWidget);

    mCustomUserStatusTreeWidget->setObjectName(u"mCustomUserStatusTreeWidget"_s);
    mainLayout->addWidget(mCustomUserStatusTreeWidget);
}

AdministratorCustomUserStatusWidget::~AdministratorCustomUserStatusWidget() = default;

void AdministratorCustomUserStatusWidget::initialize()
{
    mCustomUserStatusTreeWidget->initialize();
}

#include "moc_administratorcustomuserstatuswidget.cpp"
