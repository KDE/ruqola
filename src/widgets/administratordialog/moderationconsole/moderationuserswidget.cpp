/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationuserswidget.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <QTextBrowser>
#include <QVBoxLayout>

ModerationUsersWidget::ModerationUsersWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mTextBrowser(new QTextBrowser(this))
    , mCurrentRocketChatAccount(account)
// TODO add list ? or QTextDocument
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mainLayout->addWidget(mTextBrowser);
}

ModerationUsersWidget::~ModerationUsersWidget() = default;

void ModerationUsersWidget::setModerationReportUserInfos(const ModerationReportUserInfos &infos)
{
    // TODO generate infos
}

#include "moc_moderationuserswidget.cpp"
