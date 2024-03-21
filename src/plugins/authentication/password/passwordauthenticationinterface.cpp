/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "passwordauthenticationinterface.h"
#include "config-ruqola.h"
#include "rocketchataccount.h"

#include "authenticationmanager/ddpauthenticationmanager.h"
#include "authenticationmanager/restauthenticationmanager.h"
#include "connection.h"
#include "ddpapi/ddpclient.h"

#include "ruqola.h"

PasswordAuthenticationInterface::PasswordAuthenticationInterface(QObject *parent)
    : PluginAuthenticationInterface(parent)
{
}

PasswordAuthenticationInterface::~PasswordAuthenticationInterface() = default;

void PasswordAuthenticationInterface::login()
{
    if (!mAccount->settings()->authToken().isEmpty() && !mAccount->settings()->tokenExpired()) {
        if (Ruqola::self()->useRestApiLogin()) {
            mAccount->restApi()->authenticationManager()->setAuthToken(mAccount->settings()->authToken());
            mAccount->restApi()->authenticationManager()->login();
        } else {
            mAccount->ddp()->authenticationManager()->setAuthToken(mAccount->settings()->authToken());
            mAccount->ddp()->authenticationManager()->login();
        }
        return;
    }

    if (!mAccount->settings()->twoFactorAuthenticationCode().isEmpty()) {
        if (Ruqola::self()->useRestApiLogin()) {
            mAccount->restApi()->authenticationManager()->sendOTP(mAccount->settings()->twoFactorAuthenticationCode());
        } else {
            mAccount->ddp()->authenticationManager()->sendOTP(mAccount->settings()->twoFactorAuthenticationCode());
        }
        return;
    }

    if (mAccount->settings()->password().isEmpty()) {
        return;
    }

    if (Ruqola::self()->useRestApiLogin()) {
        if (mAccount->ldapEnabled()) {
            mAccount->restApi()->authenticationManager()->loginLDAP(mAccount->settings()->userName(), mAccount->settings()->password());
        } else {
            mAccount->restApi()->authenticationManager()->loginPassword(mAccount->settings()->userName(), mAccount->settings()->password());
        }
    } else {
        if (mAccount->ldapEnabled()) {
            mAccount->ddp()->authenticationManager()->loginLDAP(mAccount->settings()->userName(), mAccount->settings()->password());
        } else {
            mAccount->ddp()->authenticationManager()->loginPassword(mAccount->settings()->userName(), mAccount->settings()->password());
        }
    }
}

PluginAuthenticationConfigureWidget *PasswordAuthenticationInterface::configureWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    // Nothing
    return nullptr;
}

#include "moc_passwordauthenticationinterface.cpp"
