/*
   Copyright (c) 2017-2020 Laurent Montel <montel@kde.org>

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

    Q_REQUIRED_RESULT QString uniqueId() const;
    void setUniqueId(const QString &uniqueId);

    Q_REQUIRED_RESULT QString jitsiMeetUrl() const;
    void setJitsiMeetUrl(const QString &jitsiMeetUrl);

    Q_REQUIRED_RESULT QString jitsiMeetPrefix() const;
    void setJitsiMeetPrefix(const QString &jitsiMeetPrefix);

    Q_REQUIRED_RESULT QString fileUploadStorageType() const;
    void setFileUploadStorageType(const QString &type);

    void setAllowMessageEditing(bool state);
    Q_REQUIRED_RESULT bool allowMessageEditing() const;

    void setBlockEditingMessageInMinutes(int minutes);
    Q_REQUIRED_RESULT int blockEditingMessageInMinutes() const;

    Q_REQUIRED_RESULT bool otrEnabled() const;
    void setOtrEnabled(bool otrEnabled);

    Q_REQUIRED_RESULT bool needAdaptNewSubscriptionRC60() const;

    void setServerVersion(const QString &version);

    void addOauthService(const QString &service);

    Q_REQUIRED_RESULT AuthenticationManager::OauthTypes serverOauthTypes() const;

    Q_REQUIRED_RESULT bool serverHasSupportForOauthType(AuthenticationManager::OauthType type) const;

    Q_REQUIRED_RESULT bool ruqolaHasSupportForOauthType(AuthenticationManager::OauthType type) const;
    Q_REQUIRED_RESULT bool canShowOauthService(AuthenticationManager::OauthType type) const;
    void addRuqolaAuthenticationSupport(AuthenticationManager::OauthType type);
    Q_REQUIRED_RESULT AuthenticationManager::OauthTypes ruqolaOauthTypes() const;

    Q_REQUIRED_RESULT QString siteUrl() const;
    void setSiteUrl(const QString &siteUrl);

    Q_REQUIRED_RESULT QString siteName() const;
    void setSiteName(const QString &siteName);

    Q_REQUIRED_RESULT bool hasAtLeastVersion(int major, int minor, int patch) const;

    Q_REQUIRED_RESULT bool encryptionEnabled() const;
    void setEncryptionEnabled(bool encryptionEnabled);

    Q_REQUIRED_RESULT QString serverVersionStr() const;

    Q_REQUIRED_RESULT int serverVersionMajor() const;

    Q_REQUIRED_RESULT int serverVersionMinor() const;

    Q_REQUIRED_RESULT int serverVersionPatch() const;

    Q_REQUIRED_RESULT bool allowMessagePinningEnabled() const;
    void setAllowMessagePinningEnabled(bool allowMessagePinningEnabled);

    Q_REQUIRED_RESULT bool allowMessageSnippetingEnabled() const;
    void setAllowMessageSnippetingEnabled(bool allowMessageSnippetingEnabled);

    Q_REQUIRED_RESULT bool allowMessageStarringEnabled() const;
    void setAllowMessageStarringEnabled(bool allowMessageStarringEnabled);

    Q_REQUIRED_RESULT bool allowMessageDeletingEnabled() const;
    void setAllowMessageDeletingEnabled(bool allowMessageDeletingEnabled);

    Q_REQUIRED_RESULT bool jitsiEnabled() const;
    void setJitsiEnabled(bool jitsiEnabled);

    Q_REQUIRED_RESULT bool threadsEnabled() const;
    void setThreadsEnabled(bool threadEnabled);

    Q_REQUIRED_RESULT bool discussionEnabled() const;
    void setDiscussionEnabled(bool discussionEnabled);

    Q_REQUIRED_RESULT bool autoTranslateEnabled() const;
    void setAutoTranslateEnabled(bool autoTranslateEnabled);

    Q_REQUIRED_RESULT QString autoTranslateGoogleKey() const;
    void setAutoTranslateGoogleKey(const QString &autoTranslateGoogleKey);

private:
    void adaptToServerVersion();
    QString mUniqueId;
    QString mJitsiMeetUrl;
    QString mJitsiMeetPrefix;
    QString mFileUploadStorageType;
    QString mSiteUrl;
    QString mSiteName;
    QString mServerVersionStr;
    QString mAutoTranslateGoogleKey;
    AuthenticationManager::OauthTypes mServerOauthTypes = AuthenticationManager::OauthType::Password;
    AuthenticationManager::OauthTypes mRuqolaOauthTypes = AuthenticationManager::OauthType::Password;
    int mBlockEditingMessageInMinutes = 5;
    int mServerVersionMajor = -1;
    int mServerVersionMinor = -1;
    int mServerVersionPatch = -1;

    bool mAllowEditingMessage = true;
    bool mOtrEnabled = true;
    bool mNeedAdaptNewSubscriptionRC60 = false;
    bool mEncryptionEnabled = false;
    bool mAllowMessagePinningEnabled = false;
    bool mAllowMessageSnippetingEnabled = false;
    bool mAllowMessageStarringEnabled = false;
    bool mAllowMessageDeletingEnabled = false;
    bool mJitsiEnabled = false;
    bool mThreadsEnabled = false;
    bool mDiscussionEnabled = false;
    bool mAutoTranslateEnabled = false;
};
LIBRUQOLACORE_EXPORT QDebug operator <<(QDebug d, const RuqolaServerConfig &t);
#endif // RUQOLASERVERCONFIG_H
