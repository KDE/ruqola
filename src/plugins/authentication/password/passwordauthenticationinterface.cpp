/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "passwordauthenticationinterface.h"
#include "ddpapi/ddpauthenticationmanager.h"
#include "ddpapi/ddpclient.h"
#include "rocketchataccount.h"

PasswordAuthenticationInterface::PasswordAuthenticationInterface(QObject *parent)
    : PluginAuthenticationInterface(parent)
{
}

PasswordAuthenticationInterface::~PasswordAuthenticationInterface() = default;

void PasswordAuthenticationInterface::login()
{
    if (!mAccount->settings()->authToken().isEmpty() && !mAccount->settings()->tokenExpired()) {
        mAccount->ddp()->authenticationManager()->setAuthToken(mAccount->settings()->authToken());
        mAccount->ddp()->authenticationManager()->login();
        return;
    }

    if (!mAccount->settings()->twoFactorAuthenticationCode().isEmpty()) {
        mAccount->ddp()->authenticationManager()->sendOTP(mAccount->settings()->twoFactorAuthenticationCode());
        return;
    }

    if (mAccount->settings()->password().isEmpty()) {
        return;
    }

    if (mAccount->ldapEnabled()) {
        mAccount->ddp()->authenticationManager()->loginLDAP(mAccount->settings()->userName(), mAccount->settings()->password());
    } else {
        mAccount->ddp()->authenticationManager()->login(mAccount->settings()->userName(), mAccount->settings()->password());
    }
}
