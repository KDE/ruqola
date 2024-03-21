/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "passwordauthenticationinterface.h"
#include "config-ruqola.h"
#include "rocketchataccount.h"

#if USE_RESTAPI_LOGIN_CMAKE_SUPPORT
#include "authenticationmanager/restauthenticationmanager.h"
#include "connection.h"
#else
#include "ddpapi/ddpauthenticationmanager.h"
#include "ddpapi/ddpclient.h"
#endif

PasswordAuthenticationInterface::PasswordAuthenticationInterface(QObject *parent)
    : PluginAuthenticationInterface(parent)
{
}

PasswordAuthenticationInterface::~PasswordAuthenticationInterface() = default;

void PasswordAuthenticationInterface::login()
{
    if (!mAccount->settings()->authToken().isEmpty() && !mAccount->settings()->tokenExpired()) {
#if USE_RESTAPI_LOGIN_CMAKE_SUPPORT
        mAccount->restApi()->authenticationManager()->setAuthToken(mAccount->settings()->authToken());
        mAccount->restApi()->authenticationManager()->login();
#else
        mAccount->ddp()->authenticationManager()->setAuthToken(mAccount->settings()->authToken());
        mAccount->ddp()->authenticationManager()->login();
#endif
        return;
    }

    if (!mAccount->settings()->twoFactorAuthenticationCode().isEmpty()) {
#if USE_RESTAPI_LOGIN_CMAKE_SUPPORT
        mAccount->restApi()->authenticationManager()->sendOTP(mAccount->settings()->twoFactorAuthenticationCode());
#else
        mAccount->ddp()->authenticationManager()->sendOTP(mAccount->settings()->twoFactorAuthenticationCode());
#endif
        return;
    }

    if (mAccount->settings()->password().isEmpty()) {
        return;
    }

#if USE_RESTAPI_LOGIN_CMAKE_SUPPORT
    if (mAccount->ldapEnabled()) {
        mAccount->restApi()->authenticationManager()->loginLDAP(mAccount->settings()->userName(), mAccount->settings()->password());
    } else {
        mAccount->restApi()->authenticationManager()->loginPassword(mAccount->settings()->userName(), mAccount->settings()->password());
    }
#else
    if (mAccount->ldapEnabled()) {
        mAccount->ddp()->authenticationManager()->loginLDAP(mAccount->settings()->userName(), mAccount->settings()->password());
    } else {
        mAccount->ddp()->authenticationManager()->loginPassword(mAccount->settings()->userName(), mAccount->settings()->password());
    }
#endif
}

PluginAuthenticationConfigureWidget *PasswordAuthenticationInterface::configureWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    // Nothing
    return nullptr;
}

#include "moc_passwordauthenticationinterface.cpp"
