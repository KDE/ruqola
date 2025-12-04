/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "deviceinfo.h"
QT_IMPL_METATYPE_EXTERN_TAGGED(DeviceInfo, Ruqola_DeviceInfo)

#include "utils.h"
using namespace Qt::Literals::StringLiterals;
DeviceInfo::DeviceInfo() = default;

DeviceInfo::~DeviceInfo() = default;

QDebug operator<<(QDebug d, const DeviceInfo &t)
{
    d.space() << "Identifier" << t.identifier();
    d.space() << "Host" << t.host();
    d.space() << "SessionId" << t.sessionId();
    d.space() << "Ip" << t.ip();
    d.space() << "UserId" << t.userId();
    d.space() << "LoginAt" << t.loginAt();
    d.space() << "client" << t.client();
    d.space() << "os" << t.os();
    return d;
}

void DeviceInfo::parseDeviceInfo(const QJsonObject &obj)
{
    mIdentifier = obj["_id"_L1].toString().toLatin1();
    mHost = obj["host"_L1].toString();
    mSessionId = obj["sessionId"_L1].toString().toLatin1();
    mIp = obj["ip"_L1].toString();
    mUserId = obj["userId"_L1].toString().toLatin1();
    const QJsonObject deviceObj = obj["device"_L1].toObject();
    if (!deviceObj.isEmpty()) {
        const QJsonObject osObj = deviceObj["os"_L1].toObject();
        if (!osObj.isEmpty()) {
            mOs = osObj["name"_L1].toString() + u' ' + osObj["version"_L1].toString();
        }
        mClient = deviceObj["name"_L1].toString();
    }
    setLoginAt(Utils::parseIsoDate(u"loginAt"_s, obj));
    //"device":{"longVersion":"103.0","name":"Firefox","os":{"name":"Linux","version":"x86_64"},"type":"browser","version":"103.0"}
    // TODO LoginAt
    // TODO
}

bool DeviceInfo::operator==(const DeviceInfo &other) const
{
    return mIdentifier == other.identifier() && mIp == other.ip() && mHost == other.host() && mSessionId == other.sessionId() && mUserId == other.userId()
        && mLoginAt == other.loginAt() && mOs == other.os() && mClient == other.client();
}

const QByteArray &DeviceInfo::identifier() const
{
    return mIdentifier;
}

void DeviceInfo::setIdentifier(const QByteArray &newIdentifier)
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

const QByteArray &DeviceInfo::sessionId() const
{
    return mSessionId;
}

void DeviceInfo::setSessionId(const QByteArray &newSessionId)
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

const QByteArray &DeviceInfo::userId() const
{
    return mUserId;
}

void DeviceInfo::setUserId(const QByteArray &newUserId)
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
    QLocale l;
    mLoginAtDateTimeStr = l.toString(QDateTime::fromMSecsSinceEpoch(mLoginAt), QLocale::LongFormat);
}

QString DeviceInfo::loginAtDisplay() const
{
    return mLoginAtDateTimeStr;
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
