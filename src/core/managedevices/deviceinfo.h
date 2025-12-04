/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QJsonObject>
#include <QString>

class LIBRUQOLACORE_EXPORT DeviceInfo
{
public:
    DeviceInfo();
    ~DeviceInfo();

    void parseDeviceInfo(const QJsonObject &object);

    [[nodiscard]] bool operator==(const DeviceInfo &other) const;

    [[nodiscard]] const QByteArray &identifier() const;
    void setIdentifier(const QByteArray &newIdentifier);

    [[nodiscard]] const QString &host() const;
    void setHost(const QString &newHost);

    [[nodiscard]] const QByteArray &sessionId() const;
    void setSessionId(const QByteArray &newSessionId);

    [[nodiscard]] const QString &ip() const;
    void setIp(const QString &newIp);

    [[nodiscard]] const QByteArray &userId() const;
    void setUserId(const QByteArray &newUserId);

    [[nodiscard]] qint64 loginAt() const;
    void setLoginAt(qint64 newLoginAt);

    [[nodiscard]] const QString &client() const;
    void setClient(const QString &newClient);

    [[nodiscard]] const QString &os() const;
    void setOs(const QString &newOs);

    [[nodiscard]] QString loginAtDisplay() const;

private:
    QByteArray mIdentifier;
    QString mHost;
    QByteArray mSessionId;
    QString mIp;
    QByteArray mUserId;
    QString mClient;
    QString mOs;
    QString mLoginAtDateTimeStr;
    qint64 mLoginAt = -1;
};

Q_DECLARE_METATYPE(DeviceInfo)
Q_DECLARE_TYPEINFO(DeviceInfo, Q_RELOCATABLE_TYPE);
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const DeviceInfo &t);
