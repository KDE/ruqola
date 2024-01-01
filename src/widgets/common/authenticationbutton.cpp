/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "authenticationbutton.h"
#include <KLocalizedString>

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
    setText(i18n("Sign in with %1", info.name()));
    setIcon(QIcon::fromTheme(info.iconName()));
}

AuthenticationInfo AuthenticationButton::authenticationInfo() const
{
    return mAuthenticationInfo;
}

#include "moc_authenticationbutton.cpp"
