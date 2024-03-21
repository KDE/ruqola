/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "authenticationmanagerbase.h"
#include "libruqolacore_export.h"
#include <QJsonObject>
#include <QObject>

class Connection;
class LIBRUQOLACORE_EXPORT RESTAuthenticationManager : public AuthenticationManagerBase
{
    Q_OBJECT
public:
    explicit RESTAuthenticationManager(Connection *restApiConnection, QObject *parent = nullptr);
    ~RESTAuthenticationManager() override;

    void login() override;
    void sendOTP(const QString &otp) override;
    void logout() override;
    void logoutAndCleanup() override;

    void processMethodResponseImpl(const QJsonObject &replyObject, RESTAuthenticationManager::Method method);

protected:
    virtual void callLoginImpl(const QJsonArray &params, RESTAuthenticationManager::Method method, const QString &methodName);
    void loginImpl(const QJsonArray &params) override;

private:
    LIBRUQOLACORE_NO_EXPORT void loginImpl(const QJsonArray &params, RESTAuthenticationManager::Method method, const QString &methodName);
    LIBRUQOLACORE_NO_EXPORT QJsonObject generateJsonMethod(const QString &method, const QJsonDocument &params, quint64 id);

    Connection *const mRestApiConnection;
    int mIndex = 5000;
};
