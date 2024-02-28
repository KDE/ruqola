/*
   SPDX-FileCopyrightText:  2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "personalaccesstokenauthenticationinterface.h"
#include "connection.h"
#include "personalaccesstokenauthenticationconfigwidget.h"
#include "personalaccesstokenauthenticationplugin_debug.h"
#include "rocketchataccount.h"
#include <QPointer>

PersonalAccessTokenAuthenticationInterface::PersonalAccessTokenAuthenticationInterface(QObject *parent)
    : PluginAuthenticationInterface(parent)
{
}

PersonalAccessTokenAuthenticationInterface::~PersonalAccessTokenAuthenticationInterface() = default;

void PersonalAccessTokenAuthenticationInterface::login()
{
    qCDebug(RUQOLA_PERSONALACCESSTOKENAUTHENTICATION_PLUGIN_LOG) << " login personal access token";
    if (!mAccount->settings()->authToken().isEmpty() && !mAccount->settings()->tokenExpired()) {
        // https://developer.rocket.chat/reference/api/rest-api#access-tokens
        mAccount->restApi()->setAuthToken(mAccount->settings()->authToken());
        mAccount->restApi()->setUserId(mAccount->settings()->userId());

        // https://developer.rocket.chat/reference/api/realtime-api/method-calls/authentication/login#using-an-authentication-token
        // TODO login ddpclient
        mAccount->ddp()->authenticationManager()->setAuthToken(mAccount->settings()->authToken());
        mAccount->ddp()->authenticationManager()->login();
        return;
    }
    qCDebug(RUQOLA_PERSONALACCESSTOKENAUTHENTICATION_PLUGIN_LOG) << " authToken is empty !!!!!. Can't connect";
}

PluginAuthenticationConfigureWidget *PersonalAccessTokenAuthenticationInterface::configureWidget(QWidget *parent)
{
    return new PersonalAccessTokenAuthenticationConfigWidget(parent);
}

#include "moc_personalaccesstokenauthenticationinterface.cpp"
