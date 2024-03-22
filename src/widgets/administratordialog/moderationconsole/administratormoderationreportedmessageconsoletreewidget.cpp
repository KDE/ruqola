/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratormoderationreportedmessageconsoletreewidget.h"

#include "moderationreportedmessageconsoletreewidget.h"

#include <QVBoxLayout>

AdministratorModerationReportedMessageConsoleTreeWidget::AdministratorModerationReportedMessageConsoleTreeWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mModerationConsoleTreeWidget(new ModerationReportedMessageConsoleTreeWidget(account, this))
    , mAdministratorModerationRangeWidget(new AdministratorModerationRangeWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});
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

AdministratorModerationReportedMessageConsoleTreeWidget::~AdministratorModerationReportedMessageConsoleTreeWidget() = default;

void AdministratorModerationReportedMessageConsoleTreeWidget::initialize()
{
    mModerationConsoleTreeWidget->initialize();
}
