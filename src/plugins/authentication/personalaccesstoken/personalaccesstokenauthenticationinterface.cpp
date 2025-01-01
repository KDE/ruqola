/*
   SPDX-FileCopyrightText:  2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "personalaccesstokenauthenticationinterface.h"
#include "authenticationmanager/ddpauthenticationmanager.h"
#include "authenticationmanager/restauthenticationmanager.h"
#include "config-ruqola.h"
#include "connection.h"
#include "ddpapi/ddpclient.h"
#include "personalaccesstokenauthenticationconfigwidget.h"
#include "personalaccesstokenauthenticationplugin_debug.h"
#include "rocketchataccount.h"
#include "rocketchataccountsettings.h"

PersonalAccessTokenAuthenticationInterface::PersonalAccessTokenAuthenticationInterface(QObject *parent)
    : PluginAuthenticationInterface(parent)
{
}

PersonalAccessTokenAuthenticationInterface::~PersonalAccessTokenAuthenticationInterface() = default;

bool PersonalAccessTokenAuthenticationInterface::login()
{
    qCDebug(RUQOLA_PERSONALACCESSTOKENAUTHENTICATION_PLUGIN_LOG) << " login personal access token";
    // FIXME: expire token!
    if (!mAccount->settings()->authToken().isEmpty()) {
        // https://developer.rocket.chat/reference/api/rest-api#access-tokens
        mAccount->restApi()->authenticationManager()->setAuthToken(mAccount->settings()->authToken());
        mAccount->restApi()->authenticationManager()->setUserId(QString::fromLatin1(mAccount->settings()->userId()));
        // qDebug() << " mAccount->settings()->authToken() " << mAccount->settings()->authToken() << "mAccount->settings()->userId()  "
        //          << mAccount->settings()->userId();
        mAccount->restApi()->authenticationManager()->setLoginStatus(AuthenticationManager::LoggedIn);

        // https://developer.rocket.chat/reference/api/realtime-api/method-calls/authentication/login#using-an-authentication-token
        mAccount->ddp()->authenticationManager()->setAuthToken(mAccount->settings()->authToken());
        if (!mAccount->ddp()->authenticationManager()->login()) {
            return false;
        }
        return true;
    }
    qCDebug(RUQOLA_PERSONALACCESSTOKENAUTHENTICATION_PLUGIN_LOG) << " authToken is empty !. Can't connect";
    return false;
}

PluginAuthenticationConfigureWidget *PersonalAccessTokenAuthenticationInterface::configureWidget(QWidget *parent)
{
    return new PersonalAccessTokenAuthenticationConfigWidget(parent);
}

#include "moc_personalaccesstokenauthenticationinterface.cpp"
