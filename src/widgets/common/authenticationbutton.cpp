/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "authenticationbutton.h"

AuthenticationButton::AuthenticationButton(QWidget *parent)
    : QPushButton(parent)
{
    connect(this, &QPushButton::clicked, this, [this]() {
        Q_EMIT authentication(mAuthenticationInfo.oauthType());
    });
}

AuthenticationButton::~AuthenticationButton() = default;

void AuthenticationButton::setAuthenticationInfo(const AuthenticationInfo &info)
{
    mAuthenticationInfo = info;
    setText(info.name());
    setIcon(QIcon::fromTheme(info.iconName()));
}

AuthenticationInfo AuthenticationButton::authenticationInfo() const
{
    return mAuthenticationInfo;
}

#include "moc_authenticationbutton.cpp"
