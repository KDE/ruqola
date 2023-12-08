/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "authenticationwidget.h"
#include "authenticationbutton.h"
#include "authenticationinfo.h"
#include "model/loginmethodmodel.h"
#include "rocketchataccount.h"
#include <QVBoxLayout>

AuthenticationWidget::AuthenticationWidget(QWidget *parent)
    : QWidget{parent}
    , mMainLayout(new QVBoxLayout(this))
{
    mMainLayout->setObjectName(QStringLiteral("mainLayout"));
    mMainLayout->setContentsMargins({});
}

AuthenticationWidget::~AuthenticationWidget() = default;

void AuthenticationWidget::switchRocketChatAccount(RocketChatAccount *account)
{
    const QVector<AuthenticationInfo> authenticationInfos = account->loginMethodModel()->authentications();
    for (const AuthenticationInfo &info : authenticationInfos) {
        auto button = new AuthenticationButton(this);
        connect(button, &AuthenticationButton::authentication, this, &AuthenticationWidget::authentication);
        button->setAuthenticationInfo(info);
        mMainLayout->addWidget(button);
    }
}

#include "moc_authenticationwidget.cpp"
