/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

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
    , mDeactivatedUsers(new AdministratorUsersWidget(AdministratorUsersWidget::UsersType::Deactivated, account, this))
    , mActivatedUsers(new AdministratorUsersWidget(AdministratorUsersWidget::UsersType::Activate, account, this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName("mainLayout"_L1);
    mainLayout->setContentsMargins({});

    mTabWidget->setObjectName("mTabWidget"_L1);
    mainLayout->addWidget(mTabWidget);

    mAllUsers->setObjectName("mAllUsers"_L1);
    mPendingUsers->setObjectName("mPendingUsers"_L1);
    mDeactivatedUsers->setObjectName("mDeactivatedUsers"_L1);
    mActivatedUsers->setObjectName("mActivatedUsers"_L1);

    mTabWidget->addTab(mAllUsers, i18n("All"));
    mTabWidget->addTab(mPendingUsers, i18n("Pending"));
    mTabWidget->addTab(mDeactivatedUsers, i18n("Deactivated"));
    mTabWidget->addTab(mActivatedUsers, i18n("Activated"));
}

AdministratorUsersTabWidget::~AdministratorUsersTabWidget() = default;

void AdministratorUsersTabWidget::initialize()
{
    mAllUsers->initialize();
    mPendingUsers->initialize();
    mDeactivatedUsers->initialize();
    mActivatedUsers->initialize();
}

#include "moc_administratoruserstabwidget.cpp"
