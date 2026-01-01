/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "authenticationmanager/authenticationmanagerbase.h"
#include "libruqolacore_export.h"
class QJsonObject;
class QObject;

class Connection;
class LIBRUQOLACORE_EXPORT RESTAuthenticationManager : public AuthenticationManagerBase
{
    Q_OBJECT
public:
    explicit RESTAuthenticationManager(Connection *restApiConnection, QObject *parent = nullptr);
    ~RESTAuthenticationManager() override;

    [[nodiscard]] QByteArray authenticationName() const override;

protected:
    void callLoginImpl(const QJsonArray &params, Method method) override;

private:
    LIBRUQOLACORE_NO_EXPORT QJsonObject generateJsonMethod(const QString &method, const QJsonDocument &params, quint64 id);

    Connection *const mRestApiConnection;
    int mIndex = 5000;
};
