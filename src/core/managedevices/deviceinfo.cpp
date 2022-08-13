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
    return d;
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
