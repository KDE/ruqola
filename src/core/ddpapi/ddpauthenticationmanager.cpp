/*

 * SPDX-FileCopyrightText: 2020 Alessandro Ambrosano <alessandro.ambrosano@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#include "ddpapi/ddpauthenticationmanager.h"

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

void DDPAuthenticationManager::callLoginImpl(const QJsonArray &params, AuthenticationManagerBase::Method method)
{
    const QString methodName = AuthenticationManagerBase::convertMethodEnumToString(method);
    ddpClient()->invokeMethodAndRegister(methodName, params, this, static_cast<int>(method));
}

void DDPAuthenticationManager::clientConnectedChangedSlot()
{
    if (mLoginStatus == AuthenticationManager::FailedToLoginPluginProblem) {
        return;
    }
    if (checkGenericError()) {
        return;
    }
    // Just connected -> not logged in yet -> state = LoggedOut
    // Just disconnected -> whatever state we're in, need to change to LoggedOut
    setLoginStatus(AuthenticationManager::LoginStatus::LoggedOut);
}

#include "moc_ddpauthenticationmanager.cpp"
