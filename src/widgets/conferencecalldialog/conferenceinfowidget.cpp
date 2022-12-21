/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "conferenceinfowidget.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <QLabel>
#include <QVBoxLayout>
ConferenceInfoWidget::ConferenceInfoWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
}

ConferenceInfoWidget::~ConferenceInfoWidget() = default;
