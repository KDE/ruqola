/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratormoderationreportedmessageconsoletreewidget.h"
using namespace Qt::Literals::StringLiterals;

#include "moderationreportedmessageconsoletreewidget.h"

#include <QVBoxLayout>

AdministratorModerationReportedMessageConsoleTreeWidget::AdministratorModerationReportedMessageConsoleTreeWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mModerationConsoleTreeWidget(new ModerationReportedMessageConsoleTreeWidget(account, this))
    , mAdministratorModerationRangeWidget(new AdministratorModerationRangeWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);

    mAdministratorModerationRangeWidget->setObjectName(u"mAdministratorModerationRangeWidget"_s);
    mainLayout->addWidget(mAdministratorModerationRangeWidget);

    mModerationConsoleTreeWidget->setObjectName(u"mModerationConsoleTreeWidget"_s);
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

#include "moc_administratormoderationreportedmessageconsoletreewidget.cpp"
