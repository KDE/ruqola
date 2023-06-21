/*
   SPDX-FileCopyrightText: 2018-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "googleauthenticationinterface.h"
#include "ddpapi/ddpclient.h"
#include "googleauthenticationplugin_debug.h"
#include "googlejob.h"
#include "rocketchataccount.h"

GoogleAuthenticationInterface::GoogleAuthenticationInterface(QObject *parent)
    : PluginAuthenticationInterface(parent)
{
}

GoogleAuthenticationInterface::~GoogleAuthenticationInterface()
{
}

void GoogleAuthenticationInterface::login()
{
    GoogleJob *api = new GoogleJob(this);
    api->doOAuth(O2::GrantFlowAuthorizationCode);

    // When this signal is emitted from google.cpp it means it has called the login 'method'
    // by sending credentialToken and credentialSecret
    connect(api, &GoogleJob::loginMethodCalled, this, &GoogleAuthenticationInterface::slotLoginProvider);
    connect(api, &GoogleJob::linkingFailed, this, &GoogleAuthenticationInterface::loginFailed);
}

void GoogleAuthenticationInterface::slotLoginProvider(const QString &credentialToken, const QString &credentialSecret)
{
    mAccount->ddp()->authenticationManager()->loginOAuth(credentialToken, credentialSecret);
}

#include "moc_googleauthenticationinterface.cpp"
