/*

 * SPDX-FileCopyrightText: 2020 Alessandro Ambrosano <alessandro.ambrosano@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#include "authenticationmanager/ddpauthenticationmanager.h"

#include "ddpapi/ddpclient.h"

#include <QJsonArray>

DDPAuthenticationManager::DDPAuthenticationManager(DDPClient *ddpClient, QObject *parent)
    : DDPManager(ddpClient, parent)
{
    connect(ddpClient, &DDPClient::connectedChanged, this, &DDPAuthenticationManager::clientConnectedChangedSlot);
    connect(ddpClient, &DDPClient::connecting, this, [this]() {
        setLoginStatus(AuthenticationManager::LoginStatus::Connecting);
    });
}

DDPAuthenticationManager::~DDPAuthenticationManager() = default;

QByteArray DDPAuthenticationManager::authenticationName() const
{
    return QByteArrayLiteral("DDP");
}

void DDPAuthenticationManager::callLoginImpl(const QJsonArray &params, AuthenticationManagerBase::Method method)
{
    const QString methodName = AuthenticationManagerBase::convertMethodEnumToString(method);
    ddpClient()->invokeMethodAndRegister(methodName, params, this, static_cast<int>(method));
}

void DDPAuthenticationManager::clientConnectedChangedSlot(bool connected)
{
    if (mLoginStatus == AuthenticationManager::FailedToLoginPluginProblem) {
        return;
    }
    if (checkGenericError()) {
        return;
    }
    if (!connected) {
        // Just disconnected -> whatever state we're in, need to change to LoggedOut
        setLoginStatus(AuthenticationManager::LoginStatus::LoggedOut);
    }
    // Otherwise, we just connected, so technically we are logged out, but calling
    // the above would also disconnect REST, we don't want that.
}

#include "moc_ddpauthenticationmanager.cpp"
