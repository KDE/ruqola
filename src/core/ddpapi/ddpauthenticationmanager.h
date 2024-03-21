/*

 * SPDX-FileCopyrightText: 2020 Alessandro Ambrosano <alessandro.ambrosano@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#pragma once

#include "ddpapi/ddpmanager.h"
#include "libruqolacore_export.h"

#include <QJsonObject>

class LIBRUQOLACORE_EXPORT DDPAuthenticationManager : public DDPManager
{
    Q_OBJECT

public:
    explicit DDPAuthenticationManager(DDPClient *ddpClient, QObject *parent = nullptr);
    ~DDPAuthenticationManager() override;

    void login() override;
    void login(const QString &user, const QString &password) override;
    void loginLDAP(const QString &user, const QString &password) override; // TODO: LDAP options?
    void loginOAuth(const QString &credentialToken, const QString &credentialSecret) override;
    void sendOTP(const QString &otp) override;
    void logout() override;
    void logoutAndCleanup() override;

private:
    LIBRUQOLACORE_NO_EXPORT void processMethodResponseImpl(int operationId, const QJsonObject &response) override;

    // Authentication doesn't involve any subscriptions
    LIBRUQOLACORE_NO_EXPORT void processSubscriptionResultImpl(int subscriptionId, const QJsonObject &result) override
    {
        Q_UNUSED(subscriptionId)
        Q_UNUSED(result)
    }

    LIBRUQOLACORE_NO_EXPORT void loginImpl(const QJsonArray &params);
    LIBRUQOLACORE_NO_EXPORT void clientConnectedChangedSlot();
};
