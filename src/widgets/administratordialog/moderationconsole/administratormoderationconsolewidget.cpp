/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratormoderationconsolewidget.h"
#include "administratormoderationreportedmessageconsoletreewidget.h"
#include "rocketchataccount.h"

#include <KLocalizedString>
#include <QVBoxLayout>

AdministratorModerationConsoleWidget::AdministratorModerationConsoleWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mAdministratorModerationReportedMessageConsoleTreeWidget(new AdministratorModerationReportedMessageConsoleTreeWidget(account, this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);

    mAdministratorModerationReportedMessageConsoleTreeWidget->setObjectName(QStringLiteral("mAdministratorModerationReportedMessageConsoleTreeWidget"));
    mainLayout->addWidget(mAdministratorModerationReportedMessageConsoleTreeWidget);
}

AdministratorModerationConsoleWidget::~AdministratorModerationConsoleWidget() = default;

void AdministratorModerationConsoleWidget::initialize()
{
    mAdministratorModerationReportedMessageConsoleTreeWidget->initialize();
}

#include "moc_administratormoderationconsolewidget.cpp"
