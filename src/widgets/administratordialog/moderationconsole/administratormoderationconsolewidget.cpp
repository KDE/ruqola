/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratormoderationconsolewidget.h"
using namespace Qt::Literals::StringLiterals;

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
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);

    auto tabbar = new QTabWidget(this);
    tabbar->setObjectName(u"tabbar"_s);
    mainLayout->addWidget(tabbar);

    mAdministratorModerationReportedMessageConsoleTreeWidget->setObjectName(u"mAdministratorModerationReportedMessageConsoleTreeWidget"_s);
    tabbar->addTab(mAdministratorModerationReportedMessageConsoleTreeWidget, i18n("Reported Messages"));

    mAdministratorModerationReportedUserConsoleTreeWidget->setObjectName(u"mAdministratorModerationReportedUserConsoleTreeWidget"_s);
    tabbar->addTab(mAdministratorModerationReportedUserConsoleTreeWidget, i18n("Reported Users"));
}

AdministratorModerationConsoleWidget::~AdministratorModerationConsoleWidget() = default;

void AdministratorModerationConsoleWidget::initialize()
{
    mAdministratorModerationReportedMessageConsoleTreeWidget->initialize();
    mAdministratorModerationReportedUserConsoleTreeWidget->initialize();
}

#include "moc_administratormoderationconsolewidget.cpp"
