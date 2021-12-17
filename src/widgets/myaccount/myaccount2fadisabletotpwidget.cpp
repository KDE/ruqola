/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccount2fadisabletotpwidget.h"
#include "ddpapi/ddpclient.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <QVBoxLayout>

MyAccount2FaDisableTotpWidget::MyAccount2FaDisableTotpWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});
}

MyAccount2FaDisableTotpWidget::~MyAccount2FaDisableTotpWidget()
{
}
