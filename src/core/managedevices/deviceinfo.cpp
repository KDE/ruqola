/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "deviceinfo.h"

DeviceInfo::DeviceInfo() = default;

DeviceInfo::~DeviceInfo() = default;

QDebug operator<<(QDebug d, const DeviceInfo &t)
{
    d << "Identifier " << t.identifier();
    d << "Host " << t.host();
    d << "SessionId " << t.sessionId();
    d << "Ip " << t.ip();
    d << "UserId " << t.userId();
    d << "LoginAt " << t.loginAt();
    return d;
}

void DeviceInfo::parseDeviceInfo(const QJsonObject &obj)
{
    mIdentifier = obj[QStringLiteral("_id")].toString();
    mHost = obj[QStringLiteral("host")].toString();
    mSessionId = obj[QStringLiteral("sessionId")].toString();
    mIp = obj[QStringLiteral("ip")].toString();
    mUserId = obj[QStringLiteral("userId")].toString();
    // TODO LoginAt
    // TODO
}

bool DeviceInfo::operator==(const DeviceInfo &other) const
{
    return mIdentifier == other.identifier() && mIp == other.ip() && mHost == other.host() && mSessionId == other.sessionId() && mUserId == other.userId()
        && mLoginAt == other.loginAt() && mOs == other.os() && mClient == other.client();
}

const QString &DeviceInfo::identifier() const
{
    return mIdentifier;
}

void DeviceInfo::setIdentifier(const QString &newIdentifier)
{
    mIdentifier = newIdentifier;
}

const QString &DeviceInfo::host() const
{
    return mHost;
}

void DeviceInfo::setHost(const QString &newHost)
{
    mHost = newHost;
}

const QString &DeviceInfo::sessionId() const
{
    return mSessionId;
}

void DeviceInfo::setSessionId(const QString &newSessionId)
{
    mSessionId = newSessionId;
}

const QString &DeviceInfo::ip() const
{
    return mIp;
}

void DeviceInfo::setIp(const QString &newIp)
{
    mIp = newIp;
}

const QString &DeviceInfo::userId() const
{
    return mUserId;
}

void DeviceInfo::setUserId(const QString &newUserId)
{
    mUserId = newUserId;
}

qint64 DeviceInfo::loginAt() const
{
    return mLoginAt;
}

void DeviceInfo::setLoginAt(qint64 newLoginAt)
{
    mLoginAt = newLoginAt;
}

const QString &DeviceInfo::client() const
{
    return mClient;
}

void DeviceInfo::setClient(const QString &newClient)
{
    mClient = newClient;
}

const QString &DeviceInfo::os() const
{
    return mOs;
}

void DeviceInfo::setOs(const QString &newOs)
{
    mOs = newOs;
}
