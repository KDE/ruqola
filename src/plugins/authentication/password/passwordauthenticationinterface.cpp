/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "passwordauthenticationinterface.h"
#include "common/authenticationloginwidget.h"
#include "config-ruqola.h"
#include "rocketchataccount.h"
#include "rocketchataccountsettings.h"

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

bool PasswordAuthenticationInterface::login()
{
    if (!mAccount->settings()->authToken().isEmpty() && !mAccount->settings()->tokenExpired()) {
        if (Ruqola::useRestApiLogin()) {
            mAccount->restApi()->authenticationManager()->setAuthToken(mAccount->settings()->authToken());
            if (!mAccount->restApi()->authenticationManager()->login()) {
                return false;
            }
        } else {
            mAccount->ddp()->authenticationManager()->setAuthToken(mAccount->settings()->authToken());
            if (!mAccount->ddp()->authenticationManager()->login()) {
                return false;
            }
        }
        return true;
    }

    if (!mAccount->settings()->twoFactorAuthenticationCode().isEmpty()) {
        if (Ruqola::useRestApiLogin()) {
            if (!mAccount->restApi()->authenticationManager()->sendOTP(mAccount->settings()->twoFactorAuthenticationCode())) {
                return false;
            }
        } else {
            if (!mAccount->ddp()->authenticationManager()->sendOTP(mAccount->settings()->twoFactorAuthenticationCode())) {
                return false;
            }
        }
        return true;
    }

    if (mAccount->settings()->password().isEmpty()) {
        return false;
    }

    if (Ruqola::useRestApiLogin()) {
        if (mAccount->ldapEnabled()) {
            if (!mAccount->restApi()->authenticationManager()->loginLDAP(mAccount->settings()->userName(), mAccount->settings()->password())) {
                return false;
            }
        } else {
            if (!mAccount->restApi()->authenticationManager()->loginPassword(mAccount->settings()->userName(), mAccount->settings()->password())) {
                return false;
            }
        }
    } else {
        if (mAccount->ldapEnabled()) {
            if (!mAccount->ddp()->authenticationManager()->loginLDAP(mAccount->settings()->userName(), mAccount->settings()->password())) {
                return false;
            }
        } else {
            if (!mAccount->ddp()->authenticationManager()->loginPassword(mAccount->settings()->userName(), mAccount->settings()->password())) {
                return false;
            }
        }
    }
    return true;
}

PluginAuthenticationConfigureWidget *PasswordAuthenticationInterface::configureWidget(QWidget *parent)
{
    return new AuthenticationLoginWidget(parent);
}

#include "moc_passwordauthenticationinterface.cpp"
