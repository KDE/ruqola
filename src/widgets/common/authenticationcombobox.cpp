/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "authenticationcombobox.h"
#include "model/loginmethodmodel.h"
#include "rocketchataccount.h"

AuthenticationComboBox::AuthenticationComboBox(QWidget *parent)
    : QComboBox(parent)
{
}

AuthenticationComboBox::~AuthenticationComboBox() = default;

void AuthenticationComboBox::switchRocketChatAccount(RocketChatAccount *account)
{
    const QVector<AuthenticationInfo> authenticationInfos = account->loginMethodModel()->authentications();
    for (const AuthenticationInfo &info : authenticationInfos) {
        addItem(QIcon::fromTheme(info.iconName()), info.name(), QVariant::fromValue(info.oauthType()));
    }
}
