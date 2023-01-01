/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorcustomuserstatuswidget.h"
#include "customuserstatustreewidget.h"

#include <KTreeWidgetSearchLineWidget>
#include <QVBoxLayout>
AdministratorCustomUserStatusWidget::AdministratorCustomUserStatusWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mCustomUserStatusTreeWidget(new CustomUserStatusTreeWidget(account, this))
    , mSearchLineWidget(new KTreeWidgetSearchLineWidget(this, mCustomUserStatusTreeWidget))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mSearchLineWidget->setObjectName(QStringLiteral("mSearchLineWidget"));
    mainLayout->addWidget(mSearchLineWidget);

    mCustomUserStatusTreeWidget->setObjectName(QStringLiteral("mCustomUserStatusTreeWidget"));
    mainLayout->addWidget(mCustomUserStatusTreeWidget);
}

AdministratorCustomUserStatusWidget::~AdministratorCustomUserStatusWidget() = default;

void AdministratorCustomUserStatusWidget::initialize()
{
    mCustomUserStatusTreeWidget->initialize();
}
