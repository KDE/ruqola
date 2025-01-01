/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratormoderationconsolewidget.h"
#include "administratormoderationreportedmessageconsoletreewidget.h"
#include "administratormoderationreporteduserconsoletreewidget.h"
#include "rocketchataccount.h"

#include <KLocalizedString>
#include <QTabWidget>
#include <QVBoxLayout>

AdministratorModerationConsoleWidget::AdministratorModerationConsoleWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mAdministratorModerationReportedMessageConsoleTreeWidget(new AdministratorModerationReportedMessageConsoleTreeWidget(account, this))
    , mAdministratorModerationReportedUserConsoleTreeWidget(new AdministratorModerationReportedUserConsoleTreeWidget(account, this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);

    auto tabbar = new QTabWidget(this);
    tabbar->setObjectName(QStringLiteral("tabbar"));
    mainLayout->addWidget(tabbar);

    mAdministratorModerationReportedMessageConsoleTreeWidget->setObjectName(QStringLiteral("mAdministratorModerationReportedMessageConsoleTreeWidget"));
    tabbar->addTab(mAdministratorModerationReportedMessageConsoleTreeWidget, i18n("Reported Messages"));

    mAdministratorModerationReportedUserConsoleTreeWidget->setObjectName(QStringLiteral("mAdministratorModerationReportedUserConsoleTreeWidget"));
    tabbar->addTab(mAdministratorModerationReportedUserConsoleTreeWidget, i18n("Reported Users"));
}

AdministratorModerationConsoleWidget::~AdministratorModerationConsoleWidget() = default;

void AdministratorModerationConsoleWidget::initialize()
{
    mAdministratorModerationReportedMessageConsoleTreeWidget->initialize();
    mAdministratorModerationReportedUserConsoleTreeWidget->initialize();
}

#include "moc_administratormoderationconsolewidget.cpp"
