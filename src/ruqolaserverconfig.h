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

#ifndef RUQOLASERVERCONFIG_H
#define RUQOLASERVERCONFIG_H

#include "libruqola_private_export.h"
#include "authenticationmanager.h"
#include <QString>

class LIBRUQOLACORE_TESTS_EXPORT RuqolaServerConfig
{
public:
    RuqolaServerConfig();

    QString uniqueId() const;
    void setUniqueId(const QString &uniqueId);

    QString jitsiMeetUrl() const;
    void setJitsiMeetUrl(const QString &jitsiMeetUrl);

    QString jitsiMeetPrefix() const;
    void setJitsiMeetPrefix(const QString &jitsiMeetPrefix);

    QString fileUploadStorageType() const;
    void setFileUploadStorageType(const QString &type);

    void setAllowMessageEditing(bool state);
    bool allowMessageEditing() const;

    void setBlockEditingMessageInMinutes(int minutes);
    int blockEditingMessageInMinutes() const;

    bool otrEnabled() const;
    void setOtrEnabled(bool otrEnabled);

    bool needAdaptNewSubscriptionRC60() const;

    void setServerVersion(const QString &version);

    void addOauthService(const QString &service);

    AuthenticationManager::OauthTypes serverOauthTypes() const;

    bool serverHasSupportForOauthType(AuthenticationManager::OauthType type) const;

    bool ruqolaHasSupportForOauthType(AuthenticationManager::OauthType type) const;
    bool canShowOauthService(AuthenticationManager::OauthType type) const;
    void addRuqolaAuthenticationSupport(AuthenticationManager::OauthType type);
    AuthenticationManager::OauthTypes ruqolaOauthTypes() const;

    QString siteUrl() const;
    void setSiteUrl(const QString &siteUrl);

    QString siteName() const;
    void setSiteName(const QString &siteName);

    bool hasAtLeastVersion(int major, int minor, int patch);
private:
    void adaptToServerVersion();
    QString mUniqueId;
    QString mJitsiMeetUrl;
    QString mJitsiMeetPrefix;
    QString mFileUploadStorageType;
    QString mSiteUrl;
    QString mSiteName;
    AuthenticationManager::OauthTypes mServerOauthTypes = AuthenticationManager::OauthType::Password;
    AuthenticationManager::OauthTypes mRuqolaOauthTypes = AuthenticationManager::OauthType::Password;
    int mBlockEditingMessageInMinutes = 5;
    int mServerVersionMajor = -1;
    int mServerVersionMinor = -1;
    int mServerVersionPatch = -1;

    bool mAllowEditingMessage = true;
    bool mOtrEnabled = true;
    bool mNeedAdaptNewSubscriptionRC60 = false;
};
#endif // RUQOLASERVERCONFIG_H
