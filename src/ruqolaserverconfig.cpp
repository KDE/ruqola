/*
   Copyright (c) 2017-2018 Montel Laurent <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "ruqolaserverconfig.h"
#include <QStringList>
#include "ruqola_debug.h"

RuqolaServerConfig::RuqolaServerConfig()
{
}

QString RuqolaServerConfig::uniqueId() const
{
    return mUniqueId;
}

void RuqolaServerConfig::setUniqueId(const QString &uniqueId)
{
    mUniqueId = uniqueId;
}

QString RuqolaServerConfig::jitsiMeetUrl() const
{
    return mJitsiMeetUrl;
}

void RuqolaServerConfig::setJitsiMeetUrl(const QString &jitsiMeetUrl)
{
    mJitsiMeetUrl = jitsiMeetUrl;
}

QString RuqolaServerConfig::jitsiMeetPrefix() const
{
    return mJitsiMeetPrefix;
}

void RuqolaServerConfig::setJitsiMeetPrefix(const QString &jitsiMeetPrefix)
{
    mJitsiMeetPrefix = jitsiMeetPrefix;
}

QString RuqolaServerConfig::fileUploadStorageType() const
{
    return mFileUploadStorageType;
}

void RuqolaServerConfig::setFileUploadStorageType(const QString &type)
{
    mFileUploadStorageType = type;
}

void RuqolaServerConfig::setAllowMessageEditing(bool state)
{
    mAllowEditingMessage = state;
}

bool RuqolaServerConfig::allowMessageEditing() const
{
    return mAllowEditingMessage;
}

void RuqolaServerConfig::setBlockEditingMessageInMinutes(int minutes)
{
    mBlockEditingMessageInMinutes = minutes;
}

int RuqolaServerConfig::blockEditingMessageInMinutes() const
{
    return mBlockEditingMessageInMinutes;
}

bool RuqolaServerConfig::otrEnabled() const
{
    return mOtrEnabled;
}

void RuqolaServerConfig::setOtrEnabled(bool otrEnabled)
{
    mOtrEnabled = otrEnabled;
}

bool RuqolaServerConfig::needAdaptNewSubscriptionRC60() const
{
    return mNeedAdaptNewSubscriptionRC60;
}

void RuqolaServerConfig::setServerVersion(const QString &version)
{
    qCDebug(RUQOLA_LOG) << " void RocketChatAccount::setServerVersion(const QString &version)" << version;
    const QStringList lst = version.split(QLatin1Char('.'));
    if (lst.count() == 3) {
        bool ok;
        int value = lst.at(0).toInt(&ok);
        if (ok) {
            mServerVersionMajor = value;
        }
        value = lst.at(1).toInt(&ok);
        if (ok) {
            mServerVersionMinor = value;
        }
        value = lst.at(2).toInt(&ok);
        if (ok) {
            mServerVersionPatch = value;
        }
    }
    adaptToServerVersion();
}

void RuqolaServerConfig::adaptToServerVersion()
{
    mNeedAdaptNewSubscriptionRC60 = (mServerVersionMajor == 0) && (mServerVersionMinor >= 60);
}
