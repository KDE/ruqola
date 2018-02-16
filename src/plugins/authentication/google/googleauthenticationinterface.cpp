/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "googleauthenticationinterface.h"
#include "googleauthenticationplugin_debug.h"
#include "googlejob.h"
#include "ddpapi/ddpclient.h"
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

    //When this signal is emitted from google.cpp it means it has called the login 'method'
    //by sending credentialToken and credentialSecret
    connect(api, &GoogleJob::loginMethodCalled, [=] {
        mAccount->ddp()->setLoginJobId(api->oauthLoginJob);
    });
    connect(api, &GoogleJob::linkingFailed, this, &GoogleAuthenticationInterface::loginFailed);
    qCWarning(RUQOLA_GOOGLEAUTHENTICATION_PLUGIN_LOG) << "Not implemented yet";
}
