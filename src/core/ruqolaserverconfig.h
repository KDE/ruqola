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
    struct LIBRUQOLACORE_EXPORT ConfigWithDefaultValue {
        QString url;
        QString defaultUrl;
        [[nodiscard]] bool isEmpty() const
        {
            return url.isEmpty() || defaultUrl.isEmpty();
        }
        [[nodiscard]] bool operator==(const RuqolaServerConfig::ConfigWithDefaultValue &other) const;
    };

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

    [[nodiscard]] QString uniqueId() const;
    void setUniqueId(const QString &uniqueId);

    [[nodiscard]] QString jitsiMeetUrl() const;
    void setJitsiMeetUrl(const QString &jitsiMeetUrl);

    [[nodiscard]] QString jitsiMeetPrefix() const;
    void setJitsiMeetPrefix(const QString &jitsiMeetPrefix);

    [[nodiscard]] QString fileUploadStorageType() const;
    void setFileUploadStorageType(const QString &type);

    void setBlockEditingMessageInMinutes(int minutes);
    [[nodiscard]] int blockEditingMessageInMinutes() const;

    void setBlockDeletingMessageInMinutes(int minutes);
    [[nodiscard]] int blockDeletingMessageInMinutes() const;

    [[nodiscard]] bool needAdaptNewSubscriptionRC60() const;

    void setServerVersion(const QString &version);
    [[nodiscard]] QString serverVersion() const;

    void addOauthService(const QString &service);

    [[nodiscard]] AuthenticationManager::AuthMethodTypes serverAuthMethodTypes() const;

    [[nodiscard]] bool serverHasSupportForAuthMethodType(AuthenticationManager::AuthMethodType type) const;

    [[nodiscard]] bool ruqolaHasSupportForAuthMethodType(AuthenticationManager::AuthMethodType type) const;
    [[nodiscard]] bool canShowAuthMethod(AuthenticationManager::AuthMethodType type) const;
    void addRuqolaAuthenticationSupport(AuthenticationManager::AuthMethodType type);
    [[nodiscard]] AuthenticationManager::AuthMethodTypes ruqolaOauthTypes() const;

    [[nodiscard]] QString siteUrl() const;
    void setSiteUrl(const QString &siteUrl);

    [[nodiscard]] QString siteName() const;
    void setSiteName(const QString &siteName);

    [[nodiscard]] bool hasAtLeastVersion(int major, int minor, int patch) const;

    [[nodiscard]] QString serverVersionStr() const;

    [[nodiscard]] int serverVersionMajor() const;

    [[nodiscard]] int serverVersionMinor() const;

    [[nodiscard]] int serverVersionPatch() const;

    [[nodiscard]] QString autoTranslateGoogleKey() const;
    void setAutoTranslateGoogleKey(const QString &autoTranslateGoogleKey);

    void parsePublicSettings(const QJsonObject &obj, bool update);

    [[nodiscard]] qint64 fileMaxFileSize() const;
    void setFileMaxFileSize(qint64 fileMaxFileSize);

    [[nodiscard]] RuqolaServerConfig::ConfigWithDefaultValue logoUrl() const;
    void setLogoUrl(const ConfigWithDefaultValue &logoUrl);

    [[nodiscard]] RuqolaServerConfig::ConfigWithDefaultValue faviconUrl() const;
    void setFaviconUrl(const RuqolaServerConfig::ConfigWithDefaultValue &faviconUrl);

    void setAllowRegistrationFrom(const QString &registrationFromValue);

    [[nodiscard]] RuqolaServerConfig::ServerConfigFeatureTypes serverConfigFeatureTypes() const;
    void setServerConfigFeatureTypes(ServerConfigFeatureTypes serverConfigFeatureTypes);

    [[nodiscard]] int loginExpiration() const;
    void setLoginExpiration(int loginExpiration);

    void setUserNameValidation(const QString &str);
    [[nodiscard]] const QString &userNameValidation() const;

    void setChannelNameValidation(const QString &str);
    [[nodiscard]] const QString &channelNameValidation() const;

    [[nodiscard]] int messageMaximumAllowedSize() const;
    void setMessageMaximumAllowedSize(int newMessageMaximumAllowedSize);

    [[nodiscard]] bool messageAllowConvertLongMessagesToAttachment() const;
    void setMessageAllowConvertLongMessagesToAttachment(bool newMessageAllowConvertLongMessagesToAttachment);

    void privateSettingsUpdated(const QJsonArray &replyArray);

    [[nodiscard]] bool useRealName() const;
    void setUseRealName(bool newUIUseRealName);

    [[nodiscard]] bool hasEnterpriseSupport() const;
    void setHasEnterpriseSupport(bool newHasEnterpriseSupport);

    [[nodiscard]] bool accountsAllowInvisibleStatusOption() const;
    void setAccountsAllowInvisibleStatusOption(bool newAccountsAllowInvisibleStatusOption);

    [[nodiscard]] bool userDataDownloadEnabled() const;
    void setUserDataDownloadEnabled(bool newUserDataDownloadEnabled);

    [[nodiscard]] bool deviceManagementEnableLoginEmails() const;
    void setDeviceManagementEnableLoginEmails(bool newDeviceManagementEnableLoginEmails);

    [[nodiscard]] bool deviceManagementAllowLoginEmailpreference() const;
    void setDeviceManagementAllowLoginEmailpreference(bool newDeviceManagementAllowLoginEmailpreference);

    [[nodiscard]] int messageGroupingPeriod() const;
    void setMessageGroupingPeriod(int newMessageGroupingPeriod);

    [[nodiscard]] int directMessageMaximumUser() const;
    void setDirectMessageMaximumUser(int newDirectMessageMaximumUser);

    [[nodiscard]] int messageQuoteChainLimit() const;
    void setMessageQuoteChainLimit(int newMessageQuoteChainLimit);

    [[nodiscard]] bool allowCustomStatusMessage() const;
    void setAllowCustomStatusMessage(bool newAllowCustomStatusMessage);

    [[nodiscard]] QStringList mediaWhiteList() const;
    void setMediaWhiteList(const QStringList &newMediaWhiteList);

    [[nodiscard]] QStringList mediaBlackList() const;
    void setMediaBlackList(const QStringList &newMediaBlackList);

    [[nodiscard]] QByteArray serialize(bool toBinary = true);

    void deserialize(const QJsonObject &source);

    [[nodiscard]] bool operator==(const RuqolaServerConfig &other) const;

    void loadAccountSettingsFromLocalDataBase(const QByteArray &ba);

private:
    Q_DISABLE_COPY(RuqolaServerConfig)
    LIBRUQOLACORE_NO_EXPORT void adaptToServerVersion();
    LIBRUQOLACORE_NO_EXPORT void assignSettingValue(bool value, ServerConfigFeatureType type);
    LIBRUQOLACORE_NO_EXPORT void loadSettings(const QJsonObject &currentConfObject);
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT static QJsonObject createJsonObject(const QString &identifier, const QString &value);
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT static QJsonObject createJsonObject(const QString &identifier, bool value);
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT static QJsonObject createJsonObject(const QString &identifier, int value);
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT static QJsonObject createJsonObject(const QString &identifier, qint64 value);
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT static QJsonObject createJsonObject(const QString &identifier,
                                                                              const RuqolaServerConfig::ConfigWithDefaultValue &value);
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT static RuqolaServerConfig::ConfigWithDefaultValue parseConfigWithDefaultValue(const QJsonObject &o);

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
    AuthenticationManager::AuthMethodTypes mServerAuthTypes = AuthenticationManager::AuthMethodType::Unknown;
    AuthenticationManager::AuthMethodTypes mRuqolaAuthMethodTypes = AuthenticationManager::AuthMethodType::Unknown;
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

    ConfigWithDefaultValue mLogoUrl;
    ConfigWithDefaultValue mFaviconUrl;
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
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const RuqolaServerConfig::ConfigWithDefaultValue &t);
