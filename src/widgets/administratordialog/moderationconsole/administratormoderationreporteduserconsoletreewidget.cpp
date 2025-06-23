/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratormoderationreporteduserconsoletreewidget.h"
using namespace Qt::Literals::StringLiterals;

#include "moderationreporteduserconsoletreewidget.h"

#include <QVBoxLayout>

AdministratorModerationReportedUserConsoleTreeWidget::AdministratorModerationReportedUserConsoleTreeWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mModerationConsoleTreeWidget(new ModerationReportedUserConsoleTreeWidget(account, this))
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

AdministratorModerationReportedUserConsoleTreeWidget::~AdministratorModerationReportedUserConsoleTreeWidget() = default;

void AdministratorModerationReportedUserConsoleTreeWidget::initialize()
{
    mModerationConsoleTreeWidget->initialize();
}

#include "moc_administratormoderationreporteduserconsoletreewidget.cpp"
