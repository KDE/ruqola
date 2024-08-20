/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationuserswidget.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <QVBoxLayout>

ModerationUsersWidget::ModerationUsersWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mCurrentRocketChatAccount(account)
// TODO add list ? or QTextDocument
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});
}

ModerationUsersWidget::~ModerationUsersWidget() = default;

#include "moc_moderationuserswidget.cpp"
