/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratormoderationconsolewidget.h"
#include "administratormoderationrangewidget.h"
#include "moderationconsoletreewidget.h"
#include "rocketchataccount.h"

#include <KLocalizedString>
#include <QVBoxLayout>

AdministratorModerationConsoleWidget::AdministratorModerationConsoleWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mModerationConsoleTreeWidget(new ModerationConsoleTreeWidget(account, this))
    , mAdministratorModerationRangeWidget(new AdministratorModerationRangeWidget(this))
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mAdministratorModerationRangeWidget->setObjectName(QStringLiteral("mAdministratorModerationRangeWidget"));
    mainLayout->addWidget(mAdministratorModerationRangeWidget);

    mModerationConsoleTreeWidget->setObjectName(QStringLiteral("mModerationConsoleTreeWidget"));
    mainLayout->addWidget(mModerationConsoleTreeWidget);
    connect(mAdministratorModerationRangeWidget, &AdministratorModerationRangeWidget::rangeChanged, this, [this]() {
        mModerationConsoleTreeWidget->setModerationRanges(mAdministratorModerationRangeWidget->range());
    });
    connect(mModerationConsoleTreeWidget, &ModerationConsoleTreeWidget::refreshList, this, &AdministratorModerationConsoleWidget::slotRefreshList);
}

void AdministratorModerationConsoleWidget::initialize()
{
    // TODO
}

AdministratorModerationConsoleWidget::~AdministratorModerationConsoleWidget() = default;

void AdministratorModerationConsoleWidget::slotRefreshList()
{
    // TODO
}

#include "moc_administratormoderationconsolewidget.cpp"
