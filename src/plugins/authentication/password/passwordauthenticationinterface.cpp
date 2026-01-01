/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

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
    auto settings = mAccount->settings();
    if (!settings->authToken().isEmpty() && !settings->tokenExpired()) {
        if (Ruqola::useRestApiLogin()) {
            mAccount->restApi()->authenticationManager()->setAuthToken(settings->authToken());
            if (!mAccount->restApi()->authenticationManager()->login()) {
                return false;
            }
        } else {
            mAccount->ddp()->authenticationManager()->setAuthToken(settings->authToken());
            if (!mAccount->ddp()->authenticationManager()->login()) {
                return false;
            }
        }
        return true;
    }

    if (!settings->twoFactorAuthenticationCode().isEmpty()) {
        if (Ruqola::useRestApiLogin()) {
            if (!mAccount->restApi()->authenticationManager()->sendOTP(settings->twoFactorAuthenticationCode())) {
                return false;
            }
        } else {
            if (!mAccount->ddp()->authenticationManager()->sendOTP(settings->twoFactorAuthenticationCode())) {
                return false;
            }
        }
        return true;
    }

    if (settings->password().isEmpty()) {
        return false;
    }

    if (Ruqola::useRestApiLogin()) {
        if (mAccount->ruqolaServerConfig()->ldapEnabled()) {
            if (!mAccount->restApi()->authenticationManager()->loginLDAP(settings->userName(), settings->password())) {
                return false;
            }
        } else {
            if (!mAccount->restApi()->authenticationManager()->loginPassword(settings->userName(), settings->password())) {
                return false;
            }
        }
    } else {
        if (mAccount->ruqolaServerConfig()->ldapEnabled()) {
            if (!mAccount->ddp()->authenticationManager()->loginLDAP(settings->userName(), settings->password())) {
                return false;
            }
        } else {
            if (!mAccount->ddp()->authenticationManager()->loginPassword(settings->userName(), settings->password())) {
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
