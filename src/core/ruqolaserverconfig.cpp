/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolaserverconfig.h"
using namespace Qt::Literals::StringLiterals;

#include "ruqola_authentication_debug.h"
#include "ruqola_debug.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>
#include <QStringList>

RuqolaServerConfig::RuqolaServerConfig() = default;

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

void RuqolaServerConfig::setBlockDeletingMessageInMinutes(int minutes)
{
    mBlockDeletingMessageInMinutes = minutes;
}

void RuqolaServerConfig::setBlockEditingMessageInMinutes(int minutes)
{
    mBlockEditingMessageInMinutes = minutes;
}

int RuqolaServerConfig::blockEditingMessageInMinutes() const
{
    return mBlockEditingMessageInMinutes;
}

bool RuqolaServerConfig::hasAtLeastVersion(int major, int minor, int patch) const
{
    //    qDebug() << " major " << major << " mServerVersionMajor " << mServerVersionMajor << " (major <= mServerVersionMajor) " << (major <=
    //    mServerVersionMajor) <<
    //                " minor " << minor << " mServerVersionMinor  " << mServerVersionMinor << " (minor <= mServerVersionMinor) " << (minor <=
    //                mServerVersionMinor) << " patch " << patch << " mServerVersionPatch " << mServerVersionPatch << " (patch <= mServerVersionPatch) " <<
    //                (patch <= mServerVersionPatch);
    if (mServerVersionMajor > major) {
        return true;
    }
    return (major <= mServerVersionMajor) && (minor <= mServerVersionMinor) && (patch <= mServerVersionPatch);
}

void RuqolaServerConfig::setServerVersion(const QString &version)
{
    mServerVersionStr = version;
    const QStringList lst = version.split(QLatin1Char('.'));
    // 0.70.0-rc.1 has 4 "."
    if (lst.count() >= 3) {
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
        } else { // Perhaps it has "rc"/"beta" etc.
            mServerVersionPatch = 0;
        }
    } else if (lst.count() == 2) { // As "4.0"
        bool ok;
        int value = lst.at(0).toInt(&ok);
        if (ok) {
            mServerVersionMajor = value;
        }
        value = lst.at(1).toInt(&ok);
        if (ok) {
            mServerVersionMinor = value;
        }
        mServerVersionPatch = 0;
    }
}

QString RuqolaServerConfig::serverVersion() const
{
    return mServerVersionStr;
}

bool RuqolaServerConfig::ruqolaHasSupportForAuthMethodType(AuthenticationManager::AuthMethodType type) const
{
    return mRuqolaAuthMethodTypes & type;
}

bool RuqolaServerConfig::canShowAuthMethod(AuthenticationManager::AuthMethodType type) const
{
    const bool hasSupport = serverHasSupportForAuthMethodType(type) && ruqolaHasSupportForAuthMethodType(type);
    return hasSupport;
}

void RuqolaServerConfig::addRuqolaAuthenticationSupport(AuthenticationManager::AuthMethodType type)
{
    mRuqolaAuthMethodTypes |= type;
}

bool RuqolaServerConfig::serverHasSupportForAuthMethodType(AuthenticationManager::AuthMethodType type) const
{
    return mServerAuthTypes & type;
}

void RuqolaServerConfig::addOauthService(const QString &service)
{
    qCDebug(RUQOLA_AUTHENTICATION_LOG) << " serviceLower " << service;
    if (service.endsWith("twitter"_L1, Qt::CaseInsensitive)) {
        mServerAuthTypes |= AuthenticationManager::AuthMethodType::Twitter;
    } else if (service.endsWith("facebook"_L1, Qt::CaseInsensitive)) {
        mServerAuthTypes |= AuthenticationManager::AuthMethodType::FaceBook;
    } else if (service.endsWith("github"_L1, Qt::CaseInsensitive)) {
        mServerAuthTypes |= AuthenticationManager::AuthMethodType::GitHub;
    } else if (service.endsWith("gitlab"_L1, Qt::CaseInsensitive)) {
        mServerAuthTypes |= AuthenticationManager::AuthMethodType::GitLab;
    } else if (service.endsWith("google"_L1, Qt::CaseInsensitive)) {
        mServerAuthTypes |= AuthenticationManager::AuthMethodType::Google;
    } else if (service.endsWith("linkedin"_L1, Qt::CaseInsensitive)) {
        mServerAuthTypes |= AuthenticationManager::AuthMethodType::Linkedin;
    } else if (service.endsWith("wordpress"_L1, Qt::CaseInsensitive)) {
        mServerAuthTypes |= AuthenticationManager::AuthMethodType::Wordpress;
    } else if (service.endsWith("apple"_L1, Qt::CaseInsensitive)) {
        mServerAuthTypes |= AuthenticationManager::AuthMethodType::Apple;
    } else if (service.endsWith("nextcloud"_L1, Qt::CaseInsensitive)) {
        mServerAuthTypes |= AuthenticationManager::AuthMethodType::NextCloud;
    } else if (service.endsWith("_oauth_proxy_host"_L1, Qt::CaseInsensitive)) {
        // Hide warning as it's not a service
        qCDebug(RUQOLA_LOG) << "_OAuth_Proxy_host found ";
    } else if (service.endsWith("_oauth_meteor"_L1, Qt::CaseInsensitive)) {
        qCDebug(RUQOLA_LOG) << "Accounts_OAuth_Meteor found ";
    } else {
        qCDebug(RUQOLA_LOG) << "Unknown service type: " << service;
    }
    qCDebug(RUQOLA_AUTHENTICATION_LOG) << " authentication service " << mServerAuthTypes;
}

bool RuqolaServerConfig::messageAllowConvertLongMessagesToAttachment() const
{
    return mMessageAllowConvertLongMessagesToAttachment;
}

void RuqolaServerConfig::setMessageAllowConvertLongMessagesToAttachment(bool newMessageAllowConvertLongMessagesToAttachment)
{
    mMessageAllowConvertLongMessagesToAttachment = newMessageAllowConvertLongMessagesToAttachment;
}

void RuqolaServerConfig::privateSettingsUpdated(const QJsonArray &updateArray)
{
    if (updateArray.count() == 2) {
        const QString updateArrayInfo{updateArray.at(0).toString()};
        if (updateArrayInfo == "updated"_L1) {
            loadSettings(updateArray.at(1).toObject());
            qDebug() << "Update settings " << *this;
        } else {
            qCWarning(RUQOLA_LOG) << "UpdateArray info unknown: " << updateArrayInfo;
        }
    } else {
        qCWarning(RUQOLA_LOG) << "Error in privateSettingsUpdated " << updateArray;
    }
}

int RuqolaServerConfig::messageMaximumAllowedSize() const
{
    return mMessageMaximumAllowedSize;
}

void RuqolaServerConfig::setMessageMaximumAllowedSize(int newMessageMaximumAllowedSize)
{
    mMessageMaximumAllowedSize = newMessageMaximumAllowedSize;
}

const QString &RuqolaServerConfig::userNameValidation() const
{
    return mUserNameValidation;
}

const QString &RuqolaServerConfig::channelNameValidation() const
{
    return mChannelNameValidation;
}

int RuqolaServerConfig::loginExpiration() const
{
    return mLoginExpiration;
}

void RuqolaServerConfig::setChannelNameValidation(const QString &str)
{
    mChannelNameValidation = str;
}

void RuqolaServerConfig::setUserNameValidation(const QString &str)
{
    mUserNameValidation = str;
}

void RuqolaServerConfig::setLoginExpiration(int loginExpiration)
{
    mLoginExpiration = loginExpiration;
}

RuqolaServerConfig::ServerConfigFeatureTypes RuqolaServerConfig::serverConfigFeatureTypes() const
{
    return mServerConfigFeatureTypes;
}

void RuqolaServerConfig::setServerConfigFeatureTypes(ServerConfigFeatureTypes serverConfigFeatureTypes)
{
    mServerConfigFeatureTypes = serverConfigFeatureTypes;
}

void RuqolaServerConfig::setAllowRegistrationFrom(const QString &registrationFromValue)
{
    if (registrationFromValue == "Public"_L1) {
        mServerConfigFeatureTypes |= ServerConfigFeatureType::RegistrationFormEnabled;
    } else if (registrationFromValue == "Disabled"_L1) {
        // Nothing => disabled
        ;
    } else if (registrationFromValue == "Secret URL"_L1) {
        qCWarning(RUQOLA_LOG) << " Registration Secret Url not implemented";
    }
}

qint64 RuqolaServerConfig::fileMaxFileSize() const
{
    return mFileMaxFileSize;
}

void RuqolaServerConfig::setFileMaxFileSize(qint64 fileMaxFileSize)
{
    mFileMaxFileSize = fileMaxFileSize;
}

int RuqolaServerConfig::blockDeletingMessageInMinutes() const
{
    return mBlockDeletingMessageInMinutes;
}

QString RuqolaServerConfig::autoTranslateGoogleKey() const
{
    return mAutoTranslateGoogleKey;
}

void RuqolaServerConfig::setAutoTranslateGoogleKey(const QString &autoTranslateGoogleKey)
{
    mAutoTranslateGoogleKey = autoTranslateGoogleKey;
}

int RuqolaServerConfig::serverVersionPatch() const
{
    return mServerVersionPatch;
}

int RuqolaServerConfig::serverVersionMinor() const
{
    return mServerVersionMinor;
}

int RuqolaServerConfig::serverVersionMajor() const
{
    return mServerVersionMajor;
}

QString RuqolaServerConfig::serverVersionStr() const
{
    return mServerVersionStr;
}

QString RuqolaServerConfig::siteName() const
{
    return mSiteName;
}

void RuqolaServerConfig::setSiteName(const QString &siteName)
{
    mSiteName = siteName;
}

QString RuqolaServerConfig::siteUrl() const
{
    return mSiteUrl;
}

void RuqolaServerConfig::setSiteUrl(const QString &siteUrl)
{
    mSiteUrl = siteUrl;
}

AuthenticationManager::AuthMethodTypes RuqolaServerConfig::ruqolaOauthTypes() const
{
    return mRuqolaAuthMethodTypes;
}

AuthenticationManager::AuthMethodTypes RuqolaServerConfig::serverAuthMethodTypes() const
{
    return mServerAuthTypes;
}

RuqolaServerConfig::ConfigWithDefaultValue RuqolaServerConfig::logoUrl() const
{
    return mLogoUrl;
}

void RuqolaServerConfig::setLogoUrl(const RuqolaServerConfig::ConfigWithDefaultValue &url)
{
    mLogoUrl = url;
}

RuqolaServerConfig::ConfigWithDefaultValue RuqolaServerConfig::faviconUrl() const
{
    return mFaviconUrl;
}

void RuqolaServerConfig::setFaviconUrl(const RuqolaServerConfig::ConfigWithDefaultValue &url)
{
    mFaviconUrl = url;
}

QDebug operator<<(QDebug d, const RuqolaServerConfig::ConfigWithDefaultValue &t)
{
    d.space() << " Value " << t.url;
    d.space() << " Default Value " << t.defaultUrl;
    return d;
}

QDebug operator<<(QDebug d, const RuqolaServerConfig &t)
{
    d.space() << "mUniqueId  " << t.uniqueId();
    d.space() << "mJitsiMeetUrl " << t.jitsiMeetUrl();
    d.space() << "mJitsiMeetPrefix " << t.jitsiMeetPrefix();
    d.space() << "mFileUploadStorageType " << t.fileUploadStorageType();
    d.space() << "mSiteUrl " << t.siteUrl();
    d.space() << "mSiteName " << t.siteName();
    d.space() << "mServerOauthTypes " << t.serverAuthMethodTypes();
    d.space() << "mRuqolaOauthTypes " << t.ruqolaOauthTypes();
    d.space() << "mBlockEditingMessageInMinutes " << t.blockEditingMessageInMinutes();
    d.space() << "mServerVersionMajor " << t.serverVersionMajor() << " mServerVersionMinor " << t.serverVersionMinor() << " mServerVersionPatch "
              << t.serverVersionPatch();
    d.space() << "mLogoUrl " << t.logoUrl();
    d.space() << "mFaviconUrl " << t.faviconUrl();
    d.space() << "mLoginExpiration " << t.loginExpiration();
    d.space() << "mChannelNameValidation " << t.channelNameValidation();
    d.space() << "mUserNameValidation " << t.userNameValidation();
    d.space() << "mMessageMaximumAllowedSize " << t.messageMaximumAllowedSize();
    d.space() << "mMessageAllowConvertLongMessagesToAttachment " << t.messageAllowConvertLongMessagesToAttachment();
    d.space() << "mUIUseRealName " << t.useRealName();
    d.space() << "mAccountsAllowInvisibleStatusOption" << t.accountsAllowInvisibleStatusOption();
    d.space() << "mUserDataDownloadEnabled " << t.userDataDownloadEnabled();
    d.space() << "mMediaBlackList " << t.mediaBlackList();
    d.space() << "mMediaWhiteList " << t.mediaWhiteList();
    d.space() << "previewEmbed " << t.previewEmbed();
    d.space() << "embedCacheExpirationDays " << t.embedCacheExpirationDays();
    d.space() << "accountsDefaultUserPreferencesPushNotifications " << t.accountsDefaultUserPreferencesPushNotifications();
    d.space() << "accountsDefaultUserPreferencesDesktopNotifications " << t.accountsDefaultUserPreferencesDesktopNotifications();
    return d;
}

void RuqolaServerConfig::assignSettingValue(bool value, ServerConfigFeatureType type)
{
    if (value) {
        mServerConfigFeatureTypes |= type;
    } else {
        mServerConfigFeatureTypes &= ~type;
    }
}

RuqolaServerConfig::ConfigWithDefaultValue RuqolaServerConfig::parseConfigWithDefaultValue(const QJsonObject &o)
{
    RuqolaServerConfig::ConfigWithDefaultValue value;
    value.defaultUrl = o["defaultUrl"_L1].toString();
    value.url = o["url"_L1].toString();
    return value;
}

bool RuqolaServerConfig::allowEmailNotifications() const
{
    return mAllowEmailNotifications;
}

void RuqolaServerConfig::setAllowEmailNotifications(bool newAllowEmailNotifications)
{
    mAllowEmailNotifications = newAllowEmailNotifications;
}

QString RuqolaServerConfig::accountsDefaultUserPreferencesPushNotifications() const
{
    return mAccountsDefaultUserPreferencesPushNotifications;
}

void RuqolaServerConfig::setAccountsDefaultUserPreferencesPushNotifications(const QString &newAccountsDefaultUserPreferencesPushNotifications)
{
    mAccountsDefaultUserPreferencesPushNotifications = newAccountsDefaultUserPreferencesPushNotifications;
}

QString RuqolaServerConfig::accountsDefaultUserPreferencesDesktopNotifications() const
{
    return mAccountsDefaultUserPreferencesDesktopNotifications;
}

void RuqolaServerConfig::setAccountsDefaultUserPreferencesDesktopNotifications(const QString &newAccountsDefaultUserPreferencesDesktopNotifications)
{
    mAccountsDefaultUserPreferencesDesktopNotifications = newAccountsDefaultUserPreferencesDesktopNotifications;
}

int RuqolaServerConfig::embedCacheExpirationDays() const
{
    return mEmbedCacheExpirationDays;
}

void RuqolaServerConfig::setEmbedCacheExpirationDays(int newEmbedCacheExpirationDays)
{
    mEmbedCacheExpirationDays = newEmbedCacheExpirationDays;
}

bool RuqolaServerConfig::previewEmbed() const
{
    return mPreviewEmbed;
}

void RuqolaServerConfig::setPreviewEmbed(bool newPreviewEmbed)
{
    mPreviewEmbed = newPreviewEmbed;
}

void RuqolaServerConfig::loadSettings(const QJsonObject &currentConfObject)
{
    // qDebug() << " currentConfObject " << currentConfObject;
    const QString id = currentConfObject["_id"_L1].toString();
    const QVariant value = currentConfObject["value"_L1].toVariant();
    static const QRegularExpression regularExpressionOAuth(QStringLiteral("^Accounts_OAuth_\\w+"));
    if (id == "uniqueID"_L1) {
        setUniqueId(value.toString());
    } else if (id == "Jitsi_Enabled"_L1) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::JitsiEnabled);
    } else if (id == "Jitsi_Enable_Teams"_L1) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::JitsiEnabledTeams);
    } else if (id == "Jitsi_Enable_Channels"_L1) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::JitsiEnabledChannels);
    } else if (id == "Jitsi_Domain"_L1) {
        setJitsiMeetUrl(value.toString());
    } else if (id == "Jitsi_URL_Room_Prefix"_L1) {
        setJitsiMeetPrefix(value.toString());
    } else if (id == "FileUpload_Storage_Type"_L1) {
        setFileUploadStorageType(value.toString());
    } else if (id == "Message_AllowEditing"_L1) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::AllowEditingMessage);
    } else if (id == "Message_AllowEditing_BlockEditInMinutes"_L1) {
        setBlockEditingMessageInMinutes(value.toInt());
    } else if (id == "Message_AllowDeleting_BlockDeleteInMinutes"_L1) {
        setBlockDeletingMessageInMinutes(value.toInt());
    } else if (id == "OTR_Enable"_L1) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::OtrEnabled);
    } else if (id.contains(regularExpressionOAuth)) {
        if (value.toBool()) {
            addOauthService(id);
        }
    } else if (id == "Site_Url"_L1) {
        setSiteUrl(value.toString());
    } else if (id == "Site_Name"_L1) {
        setSiteName(value.toString());
    } else if (id == "E2E_Enable"_L1) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::EncryptionEnabled);
    } else if (id == "Message_AllowPinning"_L1) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::AllowMessagePinning);
    } else if (id == "Message_AllowStarring"_L1) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::AllowMessageStarring);
    } else if (id == "Message_AllowDeleting"_L1) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::AllowMessageDeleting);
    } else if (id == "Threads_enabled"_L1) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::ThreadsEnabled);
    } else if (id == "Discussion_enabled"_L1) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::DiscussionEnabled);
    } else if (id == "AutoTranslate_Enabled"_L1) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::AutoTranslateEnabled);
    } else if (id == "AutoTranslate_GoogleAPIKey"_L1) {
        setAutoTranslateGoogleKey(value.toString());
    } else if (id == "FileUpload_Enabled"_L1) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::UploadFileEnabled);
    } else if (id == "FileUpload_MaxFileSize"_L1) {
        setFileMaxFileSize(value.toULongLong());
    } else if (id == "Broadcasting_enabled"_L1) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::BroadCastEnabled);
    } else if (id == "Message_VideoRecorderEnabled"_L1) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::VideoRecorderEnabled);
    } else if (id == "Message_AudioRecorderEnabled"_L1) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::AudioRecorderEnabled);
    } else if (id == "Assets_logo"_L1) {
        setLogoUrl(parseConfigWithDefaultValue(value.toJsonObject()));
    } else if (id == "Assets_favicon"_L1) {
        setFaviconUrl(parseConfigWithDefaultValue(value.toJsonObject()));
    } else if (id == "Accounts_AllowDeleteOwnAccount"_L1) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::AllowDeleteOwnAccount);
    } else if (id == "Accounts_RegistrationForm"_L1) {
        setAllowRegistrationFrom(value.toString());
    } else if (id == "Accounts_PasswordReset"_L1) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::AllowPasswordReset);
    } else if (id == "Accounts_AllowEmailChange"_L1) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::AllowEmailChange);
    } else if (id == "Accounts_AllowPasswordChange"_L1) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::AllowPasswordChange);
    } else if (id == "Accounts_AllowUsernameChange"_L1) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::AllowUsernameChange);
    } else if (id == "Accounts_AllowUserProfileChange"_L1) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::AllowUserProfileChange);
    } else if (id == "Accounts_AllowUserAvatarChange"_L1) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::AllowUserAvatarChange);
    } else if (id == "LDAP_Enable"_L1) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::LdapEnabled);
    } else if (id == "Accounts_LoginExpiration"_L1) {
        setLoginExpiration(value.toInt());
    } else if (id == "Accounts_TwoFactorAuthentication_Enabled"_L1) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::TwoFactorAuthenticationEnabled);
    } else if (id == "Accounts_TwoFactorAuthentication_By_Email_Enabled"_L1) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::TwoFactorAuthenticationByEmailEnabled);
    } else if (id == "Accounts_TwoFactorAuthentication_By_TOTP_Enabled"_L1) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::TwoFactorAuthenticationByTOTPEnabled);
    } else if (id == "Accounts_TwoFactorAuthentication_Enforce_Password_Fallback"_L1) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::TwoFactorAuthenticationEnforcePasswordFallback);
    } else if (id == "UTF8_Channel_Names_Validation"_L1) {
        setChannelNameValidation(value.toString());
    } else if (id == "UTF8_User_Names_Validation"_L1) {
        setUserNameValidation(value.toString());
    } else if (id == "Message_MaxAllowedSize"_L1) {
        setMessageMaximumAllowedSize(value.toInt());
    } else if (id == "Message_AllowConvertLongMessagesToAttachment"_L1) {
        setMessageAllowConvertLongMessagesToAttachment(value.toBool());
    } else if (id == "UI_Use_Real_Name"_L1) {
        setUseRealName(value.toBool());
    } else if (id == "Accounts_AllowInvisibleStatusOption"_L1) {
        setAccountsAllowInvisibleStatusOption(value.toBool());
    } else if (id == "UserData_EnableDownload"_L1) {
        setUserDataDownloadEnabled(value.toBool());
    } else if (id == "Device_Management_Enable_Login_Emails"_L1) {
        setDeviceManagementEnableLoginEmails(value.toBool());
    } else if (id == "Device_Management_Allow_Login_Email_preference"_L1) {
        setDeviceManagementAllowLoginEmailpreference(value.toBool());
    } else if (id == "Message_GroupingPeriod"_L1) {
        setMessageGroupingPeriod(value.toInt());
    } else if (id == "DirectMesssage_maxUsers"_L1) {
        setDirectMessageMaximumUser(value.toInt());
    } else if (id == "Message_QuoteChainLimit"_L1) {
        setMessageQuoteChainLimit(value.toInt());
    } else if (id == "Accounts_AllowUserStatusMessageChange"_L1) {
        setAllowCustomStatusMessage(value.toBool());
    } else if (id == "FileUpload_MediaTypeWhiteList"_L1) {
        setMediaWhiteList(value.toString().split(QLatin1Char(','), Qt::SkipEmptyParts));
    } else if (id == "FileUpload_MediaTypeBlackList"_L1) {
        setMediaBlackList(value.toString().split(QLatin1Char(','), Qt::SkipEmptyParts));
    } else if (id == "Accounts_ShowFormLogin"_L1) {
        if (value.toBool()) {
            mServerAuthTypes |= AuthenticationManager::AuthMethodType::Password;
        } else {
            mServerAuthTypes &= ~AuthenticationManager::AuthMethodType::Password;
        }
    } else if (id == "AuthenticationServerMethod"_L1) {
        mServerAuthTypes = static_cast<AuthenticationManager::AuthMethodTypes>(value.toInt());
    } else if (id == "API_Embed"_L1) {
        setPreviewEmbed(value.toBool());
    } else if (id == "API_EmbedCacheExpirationDays"_L1) {
        setEmbedCacheExpirationDays(value.toInt());
    } else if (id == "Accounts_Default_User_Preferences_desktopNotifications"_L1) {
        setAccountsDefaultUserPreferencesDesktopNotifications(value.toString());
    } else if (id == "Accounts_Default_User_Preferences_pushNotifications"_L1) {
        setAccountsDefaultUserPreferencesPushNotifications(value.toString());
    } else if (id == "Accounts_AllowEmailNotifications"_L1) {
        setAllowEmailNotifications(value.toBool());
    } else {
        qCDebug(RUQOLA_LOG) << "Other public settings id " << id << value;
    }
}

QStringList RuqolaServerConfig::mediaBlackList() const
{
    return mMediaBlackList;
}

void RuqolaServerConfig::setMediaBlackList(const QStringList &newMediaBlackList)
{
    mMediaBlackList = newMediaBlackList;
}

QJsonObject RuqolaServerConfig::createJsonObject(const QString &identifier, const QString &value)
{
    QJsonObject v;
    v["_id"_L1] = identifier;
    v["value"_L1] = value;
    return v;
}

QJsonObject RuqolaServerConfig::createJsonObject(const QString &identifier, const RuqolaServerConfig::ConfigWithDefaultValue &value)
{
    QJsonObject v;
    v["_id"_L1] = identifier;
    QJsonObject customUrl;
    customUrl["defaultUrl"_L1] = value.defaultUrl;
    customUrl["url"_L1] = value.url;
    v["value"_L1] = customUrl;
    return v;
}

QJsonObject RuqolaServerConfig::createJsonObject(const QString &identifier, qint64 value)
{
    QJsonObject v;
    v["_id"_L1] = identifier;
    v["value"_L1] = static_cast<qint64>(value);
    return v;
}

QJsonObject RuqolaServerConfig::createJsonObject(const QString &identifier, bool value)
{
    QJsonObject v;
    v["_id"_L1] = identifier;
    v["value"_L1] = value;
    return v;
}

QJsonObject RuqolaServerConfig::createJsonObject(const QString &identifier, int value)
{
    QJsonObject v;
    v["_id"_L1] = identifier;
    v["value"_L1] = value;
    return v;
}

QByteArray RuqolaServerConfig::serialize(bool toBinary)
{
    QJsonObject o;
    QJsonArray array;
    array.append(createJsonObject(QStringLiteral("uniqueID"), mUniqueId));
    array.append(createJsonObject(QStringLiteral("Jitsi_Enabled"), static_cast<bool>(serverConfigFeatureTypes() & ServerConfigFeatureType::JitsiEnabled)));
    array.append(
        createJsonObject(QStringLiteral("Jitsi_Enable_Teams"), static_cast<bool>(serverConfigFeatureTypes() & ServerConfigFeatureType::JitsiEnabledTeams)));
    array.append(createJsonObject(QStringLiteral("Jitsi_Enable_Channels"),
                                  static_cast<bool>(serverConfigFeatureTypes() & ServerConfigFeatureType::JitsiEnabledChannels)));
    array.append(createJsonObject(QStringLiteral("Jitsi_Domain"), jitsiMeetUrl()));
    array.append(createJsonObject(QStringLiteral("Jitsi_URL_Room_Prefix"), jitsiMeetPrefix()));
    array.append(createJsonObject(QStringLiteral("FileUpload_Storage_Type"), fileUploadStorageType()));
    array.append(
        createJsonObject(QStringLiteral("Message_AllowEditing"), static_cast<bool>(serverConfigFeatureTypes() & ServerConfigFeatureType::AllowEditingMessage)));
    array.append(createJsonObject(QStringLiteral("Message_AllowEditing_BlockEditInMinutes"), blockEditingMessageInMinutes()));
    array.append(createJsonObject(QStringLiteral("Message_AllowDeleting_BlockDeleteInMinutes"), blockDeletingMessageInMinutes()));
    array.append(createJsonObject(QStringLiteral("OTR_Enable"), static_cast<bool>(serverConfigFeatureTypes() & ServerConfigFeatureType::OtrEnabled)));
    array.append(createJsonObject(QStringLiteral("Site_Url"), mSiteUrl));
    array.append(createJsonObject(QStringLiteral("Site_Name"), siteName()));
    array.append(createJsonObject(QStringLiteral("E2E_Enable"), static_cast<bool>(serverConfigFeatureTypes() & ServerConfigFeatureType::EncryptionEnabled)));
    array.append(
        createJsonObject(QStringLiteral("Message_AllowPinning"), static_cast<bool>(serverConfigFeatureTypes() & ServerConfigFeatureType::AllowMessagePinning)));
    array.append(createJsonObject(QStringLiteral("Message_AllowStarring"),
                                  static_cast<bool>(serverConfigFeatureTypes() & ServerConfigFeatureType::AllowMessageStarring)));
    array.append(createJsonObject(QStringLiteral("Message_AllowDeleting"),
                                  static_cast<bool>(serverConfigFeatureTypes() & ServerConfigFeatureType::AllowMessageDeleting)));
    array.append(createJsonObject(QStringLiteral("Threads_enabled"), static_cast<bool>(serverConfigFeatureTypes() & ServerConfigFeatureType::ThreadsEnabled)));
    array.append(
        createJsonObject(QStringLiteral("Discussion_enabled"), static_cast<bool>(serverConfigFeatureTypes() & ServerConfigFeatureType::DiscussionEnabled)));
    array.append(createJsonObject(QStringLiteral("AutoTranslate_Enabled"),
                                  static_cast<bool>(serverConfigFeatureTypes() & ServerConfigFeatureType::AutoTranslateEnabled)));
    array.append(
        createJsonObject(QStringLiteral("FileUpload_Enabled"), static_cast<bool>(serverConfigFeatureTypes() & ServerConfigFeatureType::UploadFileEnabled)));
    array.append(createJsonObject(QStringLiteral("AutoTranslate_GoogleAPIKey"), autoTranslateGoogleKey()));
    array.append(
        createJsonObject(QStringLiteral("Broadcasting_enabled"), static_cast<bool>(serverConfigFeatureTypes() & ServerConfigFeatureType::BroadCastEnabled)));
    array.append(createJsonObject(QStringLiteral("Message_VideoRecorderEnabled"),
                                  static_cast<bool>(serverConfigFeatureTypes() & ServerConfigFeatureType::VideoRecorderEnabled)));
    array.append(createJsonObject(QStringLiteral("Message_AudioRecorderEnabled"),
                                  static_cast<bool>(serverConfigFeatureTypes() & ServerConfigFeatureType::AudioRecorderEnabled)));
    array.append(createJsonObject(QStringLiteral("Accounts_AllowDeleteOwnAccount"),
                                  static_cast<bool>(serverConfigFeatureTypes() & ServerConfigFeatureType::AllowDeleteOwnAccount)));
    array.append(createJsonObject(QStringLiteral("Accounts_PasswordReset"),
                                  static_cast<bool>(serverConfigFeatureTypes() & ServerConfigFeatureType::AllowPasswordReset)));
    array.append(createJsonObject(QStringLiteral("Accounts_AllowEmailChange"),
                                  static_cast<bool>(serverConfigFeatureTypes() & ServerConfigFeatureType::AllowEmailChange)));
    array.append(createJsonObject(QStringLiteral("Accounts_AllowPasswordChange"),
                                  static_cast<bool>(serverConfigFeatureTypes() & ServerConfigFeatureType::AllowPasswordChange)));
    array.append(createJsonObject(QStringLiteral("Accounts_AllowUsernameChange"),
                                  static_cast<bool>(serverConfigFeatureTypes() & ServerConfigFeatureType::AllowUsernameChange)));

    array.append(createJsonObject(QStringLiteral("Accounts_AllowUserProfileChange"),
                                  static_cast<bool>(serverConfigFeatureTypes() & ServerConfigFeatureType::AllowUserProfileChange)));
    array.append(createJsonObject(QStringLiteral("Accounts_AllowUserAvatarChange"),
                                  static_cast<bool>(serverConfigFeatureTypes() & ServerConfigFeatureType::AllowUserAvatarChange)));
    array.append(createJsonObject(QStringLiteral("LDAP_Enable"), static_cast<bool>(serverConfigFeatureTypes() & ServerConfigFeatureType::LdapEnabled)));
    array.append(createJsonObject(QStringLiteral("Accounts_TwoFactorAuthentication_Enabled"),
                                  static_cast<bool>(serverConfigFeatureTypes() & ServerConfigFeatureType::TwoFactorAuthenticationEnabled)));
    array.append(createJsonObject(QStringLiteral("Accounts_TwoFactorAuthentication_By_Email_Enabled"),
                                  static_cast<bool>(serverConfigFeatureTypes() & ServerConfigFeatureType::TwoFactorAuthenticationByEmailEnabled)));
    array.append(createJsonObject(QStringLiteral("Accounts_TwoFactorAuthentication_By_TOTP_Enabled"),
                                  static_cast<bool>(serverConfigFeatureTypes() & ServerConfigFeatureType::TwoFactorAuthenticationByTOTPEnabled)));
    array.append(createJsonObject(QStringLiteral("Accounts_TwoFactorAuthentication_Enforce_Password_Fallback"),
                                  static_cast<bool>(serverConfigFeatureTypes() & ServerConfigFeatureType::TwoFactorAuthenticationEnforcePasswordFallback)));
    array.append(createJsonObject(QStringLiteral("Assets_logo"), logoUrl()));
    array.append(createJsonObject(QStringLiteral("Assets_favicon"), faviconUrl()));
    array.append(createJsonObject(QStringLiteral("Accounts_LoginExpiration"), loginExpiration()));
    array.append(createJsonObject(QStringLiteral("UTF8_Channel_Names_Validation"), channelNameValidation()));
    array.append(createJsonObject(QStringLiteral("UTF8_User_Names_Validation"), userNameValidation()));
    array.append(createJsonObject(QStringLiteral("Message_MaxAllowedSize"), messageMaximumAllowedSize()));
    array.append(createJsonObject(QStringLiteral("Message_AllowConvertLongMessagesToAttachment"), messageAllowConvertLongMessagesToAttachment()));
    array.append(createJsonObject(QStringLiteral("UI_Use_Real_Name"), useRealName()));
    array.append(createJsonObject(QStringLiteral("Accounts_AllowInvisibleStatusOption"), accountsAllowInvisibleStatusOption()));
    array.append(createJsonObject(QStringLiteral("UserData_EnableDownload"), userDataDownloadEnabled()));
    array.append(createJsonObject(QStringLiteral("Device_Management_Enable_Login_Emails"), deviceManagementEnableLoginEmails()));
    array.append(createJsonObject(QStringLiteral("Device_Management_Allow_Login_Email_preference"), deviceManagementAllowLoginEmailpreference()));
    array.append(createJsonObject(QStringLiteral("Message_GroupingPeriod"), messageGroupingPeriod()));
    array.append(createJsonObject(QStringLiteral("DirectMesssage_maxUsers"), directMessageMaximumUser()));
    array.append(createJsonObject(QStringLiteral("Message_QuoteChainLimit"), messageQuoteChainLimit()));
    array.append(createJsonObject(QStringLiteral("Accounts_AllowUserStatusMessageChange"), allowCustomStatusMessage()));
    array.append(createJsonObject(QStringLiteral("FileUpload_MediaTypeWhiteList"), mMediaWhiteList.join(QLatin1Char(','))));
    array.append(createJsonObject(QStringLiteral("FileUpload_MediaTypeBlackList"), mMediaBlackList.join(QLatin1Char(','))));
    array.append(createJsonObject(QStringLiteral("FileUpload_MaxFileSize"), mFileMaxFileSize));
    array.append(createJsonObject(QStringLiteral("AuthenticationServerMethod"), static_cast<int>(mServerAuthTypes)));
    array.append(createJsonObject(QStringLiteral("API_Embed"), previewEmbed()));
    array.append(createJsonObject(QStringLiteral("API_EmbedCacheExpirationDays"), embedCacheExpirationDays()));
    array.append(
        createJsonObject(QStringLiteral("Accounts_Default_User_Preferences_desktopNotifications"), accountsDefaultUserPreferencesDesktopNotifications()));
    array.append(createJsonObject(QStringLiteral("Accounts_Default_User_Preferences_pushNotifications"), accountsDefaultUserPreferencesPushNotifications()));
    array.append(createJsonObject(QStringLiteral("Accounts_AllowEmailNotifications"), allowEmailNotifications()));

    o["result"_L1] = array;
#if 0
} else if (id.contains(regExp)) {
    if (value.toBool()) {
        addOauthService(id);
    }
} else if (id == "Accounts_RegistrationForm"_L1) {
    setAllowRegistrationFrom(value.toString());

#endif
    if (toBinary) {
        return QCborValue::fromJsonValue(o).toCbor();
    }
    QJsonDocument d;
    d.setObject(o);
    return d.toJson(QJsonDocument::Indented);
}

void RuqolaServerConfig::deserialize(const QJsonObject &obj)
{
    QJsonArray configs = obj.value("result"_L1).toArray();
    mServerConfigFeatureTypes = ServerConfigFeatureType::None;
    for (QJsonValueRef currentConfig : configs) {
        const QJsonObject currentConfObject = currentConfig.toObject();
        loadSettings(currentConfObject);
    }
}

QStringList RuqolaServerConfig::mediaWhiteList() const
{
    return mMediaWhiteList;
}

void RuqolaServerConfig::setMediaWhiteList(const QStringList &newMediaWhiteList)
{
    mMediaWhiteList = newMediaWhiteList;
}

bool RuqolaServerConfig::allowCustomStatusMessage() const
{
    return mAllowCustomStatusMessage;
}

void RuqolaServerConfig::setAllowCustomStatusMessage(bool newAllowCustomStatusMessage)
{
    mAllowCustomStatusMessage = newAllowCustomStatusMessage;
}

int RuqolaServerConfig::messageQuoteChainLimit() const
{
    return mMessageQuoteChainLimit;
}

void RuqolaServerConfig::setMessageQuoteChainLimit(int newMessageQuoteChainLimit)
{
    mMessageQuoteChainLimit = newMessageQuoteChainLimit;
}

int RuqolaServerConfig::directMessageMaximumUser() const
{
    return mDirectMessageMaximumUser;
}

void RuqolaServerConfig::setDirectMessageMaximumUser(int newDirectMessageMaximumUser)
{
    mDirectMessageMaximumUser = newDirectMessageMaximumUser;
}

int RuqolaServerConfig::messageGroupingPeriod() const
{
    return mMessageGroupingPeriod;
}

void RuqolaServerConfig::setMessageGroupingPeriod(int newMessageGroupingPeriod)
{
    mMessageGroupingPeriod = newMessageGroupingPeriod;
}

bool RuqolaServerConfig::deviceManagementAllowLoginEmailpreference() const
{
    return mDeviceManagementAllowLoginEmailpreference;
}

void RuqolaServerConfig::setDeviceManagementAllowLoginEmailpreference(bool newDeviceManagementAllowLoginEmailpreference)
{
    mDeviceManagementAllowLoginEmailpreference = newDeviceManagementAllowLoginEmailpreference;
}

bool RuqolaServerConfig::deviceManagementEnableLoginEmails() const
{
    return mDeviceManagementEnableLoginEmails;
}

void RuqolaServerConfig::setDeviceManagementEnableLoginEmails(bool newDeviceManagementEnableLoginEmails)
{
    mDeviceManagementEnableLoginEmails = newDeviceManagementEnableLoginEmails;
}

bool RuqolaServerConfig::userDataDownloadEnabled() const
{
    return mUserDataDownloadEnabled;
}

void RuqolaServerConfig::setUserDataDownloadEnabled(bool newUserDataDownloadEnabled)
{
    mUserDataDownloadEnabled = newUserDataDownloadEnabled;
}

bool RuqolaServerConfig::accountsAllowInvisibleStatusOption() const
{
    return mAccountsAllowInvisibleStatusOption;
}

void RuqolaServerConfig::setAccountsAllowInvisibleStatusOption(bool newAccountsAllowInvisibleStatusOption)
{
    mAccountsAllowInvisibleStatusOption = newAccountsAllowInvisibleStatusOption;
}

bool RuqolaServerConfig::hasEnterpriseSupport() const
{
    return mHasEnterpriseSupport;
}

void RuqolaServerConfig::setHasEnterpriseSupport(bool newHasEnterpriseSupport)
{
    mHasEnterpriseSupport = newHasEnterpriseSupport;
}

bool RuqolaServerConfig::useRealName() const
{
    return mUIUseRealName;
}

void RuqolaServerConfig::setUseRealName(bool newUIUseRealName)
{
    mUIUseRealName = newUIUseRealName;
}

void RuqolaServerConfig::parsePublicSettings(const QJsonObject &obj, bool update)
{
    // qDebug() << " void RuqolaServerConfig::parsePublicSettings(const QJsonObject &obj)" << obj;
    QJsonArray configs = obj.value("result"_L1).toArray();
    if (!update) {
        mServerConfigFeatureTypes = ServerConfigFeatureType::None;
    }
    for (QJsonValueRef currentConfig : configs) {
        const QJsonObject currentConfObject = currentConfig.toObject();
        loadSettings(currentConfObject);
    }
}

bool RuqolaServerConfig::ConfigWithDefaultValue::operator==(const RuqolaServerConfig::ConfigWithDefaultValue &other) const
{
    return other.url == url && other.defaultUrl == defaultUrl;
}

bool RuqolaServerConfig::operator==(const RuqolaServerConfig &other) const
{
    return mUniqueId == other.mUniqueId && mJitsiMeetUrl == other.mJitsiMeetUrl && mJitsiMeetPrefix == other.mJitsiMeetPrefix
        && mFileUploadStorageType == other.mFileUploadStorageType && mSiteUrl == other.mSiteUrl && mSiteName == other.mSiteName
        && mServerVersionStr == other.mServerVersionStr && mAutoTranslateGoogleKey == other.mAutoTranslateGoogleKey
        && mChannelNameValidation == other.mChannelNameValidation && mUserNameValidation == other.mUserNameValidation
        && mServerAuthTypes == other.mServerAuthTypes && mRuqolaAuthMethodTypes == other.mRuqolaAuthMethodTypes
        && mBlockEditingMessageInMinutes == other.mBlockEditingMessageInMinutes && mBlockDeletingMessageInMinutes == other.mBlockDeletingMessageInMinutes
        && mServerVersionMajor == other.mServerVersionMajor && mServerVersionMinor == other.mServerVersionMinor
        && mServerVersionPatch == other.mServerVersionPatch && mFileMaxFileSize == other.mFileMaxFileSize
        && mMessageAllowConvertLongMessagesToAttachment == other.mMessageAllowConvertLongMessagesToAttachment && mUIUseRealName == other.mUIUseRealName
        && mServerConfigFeatureTypes == other.mServerConfigFeatureTypes && mMediaWhiteList == other.mMediaWhiteList && mMediaBlackList == other.mMediaBlackList
        && mLogoUrl == other.mLogoUrl && mFaviconUrl == other.mFaviconUrl && mLoginExpiration == other.mLoginExpiration
        && mMessageMaximumAllowedSize == other.mMessageMaximumAllowedSize && mMessageGroupingPeriod == other.mMessageGroupingPeriod
        && mDirectMessageMaximumUser == other.mDirectMessageMaximumUser && mMessageQuoteChainLimit == other.mMessageQuoteChainLimit
        && mHasEnterpriseSupport == other.mHasEnterpriseSupport && mAccountsAllowInvisibleStatusOption == other.mAccountsAllowInvisibleStatusOption
        && mUserDataDownloadEnabled == other.mUserDataDownloadEnabled && mDeviceManagementEnableLoginEmails == other.mDeviceManagementEnableLoginEmails
        && mDeviceManagementAllowLoginEmailpreference == other.mDeviceManagementAllowLoginEmailpreference
        && mAllowCustomStatusMessage == other.mAllowCustomStatusMessage && mPreviewEmbed == other.mPreviewEmbed
        && mEmbedCacheExpirationDays == other.mEmbedCacheExpirationDays
        && mAccountsDefaultUserPreferencesDesktopNotifications == other.mAccountsDefaultUserPreferencesDesktopNotifications
        && mAccountsDefaultUserPreferencesPushNotifications == other.mAccountsDefaultUserPreferencesPushNotifications
        && mAllowEmailNotifications == other.mAllowEmailNotifications;
}

void RuqolaServerConfig::loadAccountSettingsFromLocalDataBase(const QByteArray &ba)
{
    const QJsonDocument doc = QJsonDocument::fromJson(ba);
    const QJsonObject newObj = doc.object();
    deserialize(newObj);
}

#include "moc_ruqolaserverconfig.cpp"
