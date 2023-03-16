/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "authenticationmanager.h"
#include "libruqolacore_export.h"
#include <QString>

class LIBRUQOLACORE_EXPORT RuqolaServerConfig
{
    Q_GADGET
public:
    RuqolaServerConfig();

    enum ServerConfigFeatureType {
        None = 0,
        AllowEditingMessage = 1,
        OtrEnabled = 2,
        // NeedAdaptNewSubscriptionRC60 = 4,
        EncryptionEnabled = 8,
        AllowMessagePinning = 0x10,
        AllowMessageStarring = 0x40,
        AllowMessageDeleting = 0x80,
        JitsiEnabled = 0x100,
        ThreadsEnabled = 0x200,
        DiscussionEnabled = 0x400,
        AutoTranslateEnabled = 0x800,
        UploadFileEnabled = 0x1000,
        BroadCastEnabled = 0x2000,
        VideoRecorderEnabled = 0x4000,
        AudioRecorderEnabled = 0x8000,
        AllowDeleteOwnAccount = 0x10000,
        RegistrationFromEnabled = 0x20000,
        AllowPasswordReset = 0x40000,
        AllowEmailChange = 0x80000,
        AllowPasswordChange = 0x100000,
        AllowUsernameChange = 0x200000,
        AllowUserProfileChange = 0x400000,
        AllowUserAvatarChange = 0x800000,
        LdapEnabled = 0x1000000,
        JitsiEnabledTeams = 0x2000000,
        JitsiEnabledChannels = 0x4000000,
        TwoFactorAuthenticationEnabled = 0x8000000,
        TwoFactorAuthenticationByEmailEnabled = 0x10000000,
        TwoFactorAuthenticationByTOTPEnabled = 0x20000000,
        TwoFactorAuthenticationEnforcePasswordFallback = 0x40000000,
    };
    Q_DECLARE_FLAGS(ServerConfigFeatureTypes, ServerConfigFeatureType)
    Q_FLAG(ServerConfigFeatureTypes)

    Q_REQUIRED_RESULT QString uniqueId() const;
    void setUniqueId(const QString &uniqueId);

    Q_REQUIRED_RESULT QString jitsiMeetUrl() const;
    void setJitsiMeetUrl(const QString &jitsiMeetUrl);

    Q_REQUIRED_RESULT QString jitsiMeetPrefix() const;
    void setJitsiMeetPrefix(const QString &jitsiMeetPrefix);

    Q_REQUIRED_RESULT QString fileUploadStorageType() const;
    void setFileUploadStorageType(const QString &type);

    void setBlockEditingMessageInMinutes(int minutes);
    Q_REQUIRED_RESULT int blockEditingMessageInMinutes() const;

    void setBlockDeletingMessageInMinutes(int minutes);
    Q_REQUIRED_RESULT int blockDeletingMessageInMinutes() const;

    Q_REQUIRED_RESULT bool needAdaptNewSubscriptionRC60() const;

    void setServerVersion(const QString &version);
    Q_REQUIRED_RESULT QString serverVersion() const;

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

    Q_REQUIRED_RESULT QString serverVersionStr() const;

    Q_REQUIRED_RESULT int serverVersionMajor() const;

    Q_REQUIRED_RESULT int serverVersionMinor() const;

    Q_REQUIRED_RESULT int serverVersionPatch() const;

    Q_REQUIRED_RESULT QString autoTranslateGoogleKey() const;
    void setAutoTranslateGoogleKey(const QString &autoTranslateGoogleKey);

    void parsePublicSettings(const QJsonObject &obj);

    Q_REQUIRED_RESULT qint64 fileMaxFileSize() const;
    void setFileMaxFileSize(qint64 fileMaxFileSize);

    Q_REQUIRED_RESULT QString logoUrl() const;
    void setLogoUrl(const QString &logoUrl);

    Q_REQUIRED_RESULT QString faviconUrl() const;
    void setFaviconUrl(const QString &faviconUrl);

    void setAllowRegistrationFrom(const QString &registrationFromValue);

    Q_REQUIRED_RESULT RuqolaServerConfig::ServerConfigFeatureTypes serverConfigFeatureTypes() const;
    void setServerConfigFeatureTypes(ServerConfigFeatureTypes serverConfigFeatureTypes);

    Q_REQUIRED_RESULT int loginExpiration() const;
    void setLoginExpiration(int loginExpiration);

    void setUserNameValidation(const QString &str);
    Q_REQUIRED_RESULT const QString &userNameValidation() const;

    void setChannelNameValidation(const QString &str);
    Q_REQUIRED_RESULT const QString &channelNameValidation() const;

    Q_REQUIRED_RESULT int messageMaximumAllowedSize() const;
    void setMessageMaximumAllowedSize(int newMessageMaximumAllowedSize);

    Q_REQUIRED_RESULT bool messageAllowConvertLongMessagesToAttachment() const;
    void setMessageAllowConvertLongMessagesToAttachment(bool newMessageAllowConvertLongMessagesToAttachment);

    void privateSettingsUpdated(const QJsonArray &replyArray);

    Q_REQUIRED_RESULT bool useRealName() const;
    void setUseRealName(bool newUIUseRealName);

    Q_REQUIRED_RESULT bool hasEnterpriseSupport() const;
    void setHasEnterpriseSupport(bool newHasEnterpriseSupport);

    Q_REQUIRED_RESULT bool accountsAllowInvisibleStatusOption() const;
    void setAccountsAllowInvisibleStatusOption(bool newAccountsAllowInvisibleStatusOption);

    Q_REQUIRED_RESULT bool userDataDownloadEnabled() const;
    void setUserDataDownloadEnabled(bool newUserDataDownloadEnabled);

    Q_REQUIRED_RESULT bool deviceManagementEnableLoginEmails() const;
    void setDeviceManagementEnableLoginEmails(bool newDeviceManagementEnableLoginEmails);

    Q_REQUIRED_RESULT bool deviceManagementAllowLoginEmailpreference() const;
    void setDeviceManagementAllowLoginEmailpreference(bool newDeviceManagementAllowLoginEmailpreference);

    Q_REQUIRED_RESULT int messageGroupingPeriod() const;
    void setMessageGroupingPeriod(int newMessageGroupingPeriod);

    Q_REQUIRED_RESULT int directMessageMaximumUser() const;
    void setDirectMessageMaximumUser(int newDirectMessageMaximumUser);

    Q_REQUIRED_RESULT int messageQuoteChainLimit() const;
    void setMessageQuoteChainLimit(int newMessageQuoteChainLimit);

    Q_REQUIRED_RESULT bool allowCustomStatusMessage() const;
    void setAllowCustomStatusMessage(bool newAllowCustomStatusMessage);

    Q_REQUIRED_RESULT QStringList mediaWhiteList() const;
    void setMediaWhiteList(const QStringList &newMediaWhiteList);

    Q_REQUIRED_RESULT QStringList mediaBlackList() const;
    void setMediaBlackList(const QStringList &newMediaBlackList);

private:
    Q_DISABLE_COPY(RuqolaServerConfig)
    void adaptToServerVersion();
    void assignSettingValue(bool value, ServerConfigFeatureType type);
    void loadSettings(const QJsonObject &currentConfObject);
    QString mUniqueId;
    QString mJitsiMeetUrl;
    QString mJitsiMeetPrefix;
    QString mFileUploadStorageType;
    QString mSiteUrl;
    QString mSiteName;
    QString mServerVersionStr;
    QString mAutoTranslateGoogleKey;
    QString mChannelNameValidation;
    QString mUserNameValidation;
    AuthenticationManager::OauthTypes mServerOauthTypes = AuthenticationManager::OauthType::Password;
    AuthenticationManager::OauthTypes mRuqolaOauthTypes = AuthenticationManager::OauthType::Password;
    int mBlockEditingMessageInMinutes = 5;
    int mBlockDeletingMessageInMinutes = 5;
    int mServerVersionMajor = -1;
    int mServerVersionMinor = -1;
    int mServerVersionPatch = -1;
    qint64 mFileMaxFileSize = -1;

    bool mNeedAdaptNewSubscriptionRC60 = false;
    bool mMessageAllowConvertLongMessagesToAttachment = false;
    bool mUIUseRealName = false;

    ServerConfigFeatureTypes mServerConfigFeatureTypes = ServerConfigFeatureType::None;

    QStringList mMediaWhiteList;
    QStringList mMediaBlackList;

    QString mLogoUrl;
    QString mFaviconUrl;
    int mLoginExpiration = -1;
    int mMessageMaximumAllowedSize = -1;
    int mMessageGroupingPeriod = -1;
    int mDirectMessageMaximumUser = -1;
    int mMessageQuoteChainLimit = -1;

    bool mHasEnterpriseSupport = false;
    bool mAccountsAllowInvisibleStatusOption = true; // Default true
    bool mUserDataDownloadEnabled = true;
    bool mDeviceManagementEnableLoginEmails = false;
    bool mDeviceManagementAllowLoginEmailpreference = false;
    bool mAllowCustomStatusMessage = false;
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const RuqolaServerConfig &t);
