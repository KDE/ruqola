/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratormoderationreporteduserconsoletreewidget.h"

#include "moderationreporteduserconsoletreewidget.h"

#include <QVBoxLayout>

AdministratorModerationReportedUserConsoleTreeWidget::AdministratorModerationReportedUserConsoleTreeWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mModerationConsoleTreeWidget(new ModerationReportedUserConsoleTreeWidget(account, this))
    , mAdministratorModerationRangeWidget(new AdministratorModerationRangeWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);

    mAdministratorModerationRangeWidget->setObjectName(QStringLiteral("mAdministratorModerationRangeWidget"));
    mainLayout->addWidget(mAdministratorModerationRangeWidget);

    mModerationConsoleTreeWidget->setObjectName(QStringLiteral("mModerationConsoleTreeWidget"));
    mainLayout->addWidget(mModerationConsoleTreeWidget);
    connect(mAdministratorModerationRangeWidget, &AdministratorModerationRangeWidget::rangeChanged, this, [this]() {
        mModerationConsoleTreeWidget->setModerationRanges(mAdministratorModerationRangeWidget->range());
    });
}

AdministratorModerationReportedUserConsoleTreeWidget::~AdministratorModerationReportedUserConsoleTreeWidget() = default;

void AdministratorModerationReportedUserConsoleTreeWidget::initialize()
{
    mModerationConsoleTreeWidget->initialize();
}

#include "moc_administratormoderationreporteduserconsoletreewidget.cpp"
