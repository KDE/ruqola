/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratoruserstabwidget.h"
#include "administratoruserswidget.h"
#include <KLocalizedString>
#include <QTabWidget>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
AdministratorUsersTabWidget::AdministratorUsersTabWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mTabWidget(new QTabWidget(this))
    , mAllUsers(new AdministratorUsersWidget(AdministratorUsersWidget::UsersType::All, account, this))
    , mPendingUsers(new AdministratorUsersWidget(AdministratorUsersWidget::UsersType::Pending, account, this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mTabWidget->setObjectName(QStringLiteral("mTabWidget"));
    mainLayout->addWidget(mTabWidget);

    mAllUsers->setObjectName("mAllUsers"_L1);
    mPendingUsers->setObjectName("mPendingUsers"_L1);

    mTabWidget->addTab(mAllUsers, i18n("All"));
    mTabWidget->addTab(mPendingUsers, i18n("Pending"));
}

AdministratorUsersTabWidget::~AdministratorUsersTabWidget() = default;

#include "moc_administratoruserstabwidget.cpp"
