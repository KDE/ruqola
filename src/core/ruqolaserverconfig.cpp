/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolaserverconfig.h"
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

bool RuqolaServerConfig::needAdaptNewSubscriptionRC60() const
{
    return mNeedAdaptNewSubscriptionRC60;
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
    adaptToServerVersion();
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
    if (service.endsWith(QLatin1String("twitter"), Qt::CaseInsensitive)) {
        mServerAuthTypes |= AuthenticationManager::AuthMethodType::Twitter;
    } else if (service.endsWith(QLatin1String("facebook"), Qt::CaseInsensitive)) {
        mServerAuthTypes |= AuthenticationManager::AuthMethodType::FaceBook;
    } else if (service.endsWith(QLatin1String("github"), Qt::CaseInsensitive)) {
        mServerAuthTypes |= AuthenticationManager::AuthMethodType::GitHub;
    } else if (service.endsWith(QLatin1String("gitlab"), Qt::CaseInsensitive)) {
        mServerAuthTypes |= AuthenticationManager::AuthMethodType::GitLab;
    } else if (service.endsWith(QLatin1String("google"), Qt::CaseInsensitive)) {
        mServerAuthTypes |= AuthenticationManager::AuthMethodType::Google;
    } else if (service.endsWith(QLatin1String("linkedin"), Qt::CaseInsensitive)) {
        mServerAuthTypes |= AuthenticationManager::AuthMethodType::Linkedin;
    } else if (service.endsWith(QLatin1String("wordpress"), Qt::CaseInsensitive)) {
        mServerAuthTypes |= AuthenticationManager::AuthMethodType::Wordpress;
    } else if (service.endsWith(QLatin1String("apple"), Qt::CaseInsensitive)) {
        mServerAuthTypes |= AuthenticationManager::AuthMethodType::Apple;
    } else if (service.endsWith(QLatin1String("nextcloud"), Qt::CaseInsensitive)) {
        mServerAuthTypes |= AuthenticationManager::AuthMethodType::NextCloud;
    } else if (service.endsWith(QLatin1String("_oauth_proxy_host"), Qt::CaseInsensitive)) {
        // Hide warning as it's not a service
        qCDebug(RUQOLA_LOG) << "_OAuth_Proxy_host found ";
    } else if (service.endsWith(QLatin1String("_oauth_meteor"), Qt::CaseInsensitive)) {
        qCDebug(RUQOLA_LOG) << "Accounts_OAuth_Meteor found ";
    } else {
        qCDebug(RUQOLA_LOG) << "Unknown service type: " << service;
    }
    qCDebug(RUQOLA_AUTHENTICATION_LOG) << " authentication service " << mServerAuthTypes;
}

void RuqolaServerConfig::adaptToServerVersion()
{
    mNeedAdaptNewSubscriptionRC60 = (mServerVersionMajor >= 1) || ((mServerVersionMajor == 0) && (mServerVersionMinor >= 60));
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
        if (updateArrayInfo == QLatin1String("updated")) {
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
    if (registrationFromValue == QLatin1String("Public")) {
        mServerConfigFeatureTypes |= ServerConfigFeatureType::RegistrationFormEnabled;
    } else if (registrationFromValue == QLatin1String("Disabled")) {
        // Nothing => disabled
        ;
    } else if (registrationFromValue == QLatin1String("Secret URL")) {
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
    d << " Value " << t.url;
    d << " Default Value " << t.defaultUrl;
    return d;
}

QDebug operator<<(QDebug d, const RuqolaServerConfig &t)
{
    d << "mUniqueId  " << t.uniqueId();
    d << "mJitsiMeetUrl " << t.jitsiMeetUrl();
    d << "mJitsiMeetPrefix " << t.jitsiMeetPrefix();
    d << "mFileUploadStorageType " << t.fileUploadStorageType();
    d << "mSiteUrl " << t.siteUrl();
    d << "mSiteName " << t.siteName();
    d << "mServerOauthTypes " << t.serverAuthMethodTypes();
    d << "mRuqolaOauthTypes " << t.ruqolaOauthTypes();
    d << "mBlockEditingMessageInMinutes " << t.blockEditingMessageInMinutes();
    d << "mNeedAdaptNewSubscriptionRC60 " << t.needAdaptNewSubscriptionRC60();
    d << "mServerVersionMajor " << t.serverVersionMajor() << " mServerVersionMinor " << t.serverVersionMinor() << " mServerVersionPatch "
      << t.serverVersionPatch();
    d << "mLogoUrl " << t.logoUrl();
    d << "mFaviconUrl " << t.faviconUrl();
    d << "mLoginExpiration " << t.loginExpiration();
    d << "mChannelNameValidation " << t.channelNameValidation();
    d << "mUserNameValidation " << t.userNameValidation();
    d << "mMessageMaximumAllowedSize " << t.messageMaximumAllowedSize();
    d << "mMessageAllowConvertLongMessagesToAttachment " << t.messageAllowConvertLongMessagesToAttachment();
    d << "mUIUseRealName " << t.useRealName();
    d << "mAccountsAllowInvisibleStatusOption" << t.accountsAllowInvisibleStatusOption();
    d << "mUserDataDownloadEnabled " << t.userDataDownloadEnabled();
    d << "mMediaBlackList " << t.mediaBlackList();
    d << "mMediaWhiteList " << t.mediaWhiteList();
    d << "previewEmbed " << t.previewEmbed();
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
    value.defaultUrl = o[QLatin1String("defaultUrl")].toString();
    value.url = o[QLatin1String("url")].toString();
    return value;
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
    const QString id = currentConfObject[QLatin1String("_id")].toString();
    const QVariant value = currentConfObject[QLatin1String("value")].toVariant();
    static const QRegularExpression regularExpressionOAuth(QStringLiteral("^Accounts_OAuth_\\w+"));
    if (id == QLatin1String("uniqueID")) {
        setUniqueId(value.toString());
    } else if (id == QLatin1String("Jitsi_Enabled")) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::JitsiEnabled);
    } else if (id == QLatin1String("Jitsi_Enable_Teams")) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::JitsiEnabledTeams);
    } else if (id == QLatin1String("Jitsi_Enable_Channels")) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::JitsiEnabledChannels);
    } else if (id == QLatin1String("Jitsi_Domain")) {
        setJitsiMeetUrl(value.toString());
    } else if (id == QLatin1String("Jitsi_URL_Room_Prefix")) {
        setJitsiMeetPrefix(value.toString());
    } else if (id == QLatin1String("FileUpload_Storage_Type")) {
        setFileUploadStorageType(value.toString());
    } else if (id == QLatin1String("Message_AllowEditing")) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::AllowEditingMessage);
    } else if (id == QLatin1String("Message_AllowEditing_BlockEditInMinutes")) {
        setBlockEditingMessageInMinutes(value.toInt());
    } else if (id == QLatin1String("Message_AllowDeleting_BlockDeleteInMinutes")) {
        setBlockDeletingMessageInMinutes(value.toInt());
    } else if (id == QLatin1String("OTR_Enable")) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::OtrEnabled);
    } else if (id.contains(regularExpressionOAuth)) {
        if (value.toBool()) {
            addOauthService(id);
        }
    } else if (id == QLatin1String("Site_Url")) {
        setSiteUrl(value.toString());
    } else if (id == QLatin1String("Site_Name")) {
        setSiteName(value.toString());
    } else if (id == QLatin1String("E2E_Enable")) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::EncryptionEnabled);
    } else if (id == QLatin1String("Message_AllowPinning")) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::AllowMessagePinning);
    } else if (id == QLatin1String("Message_AllowStarring")) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::AllowMessageStarring);
    } else if (id == QLatin1String("Message_AllowDeleting")) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::AllowMessageDeleting);
    } else if (id == QLatin1String("Threads_enabled")) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::ThreadsEnabled);
    } else if (id == QLatin1String("Discussion_enabled")) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::DiscussionEnabled);
    } else if (id == QLatin1String("AutoTranslate_Enabled")) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::AutoTranslateEnabled);
    } else if (id == QLatin1String("AutoTranslate_GoogleAPIKey")) {
        setAutoTranslateGoogleKey(value.toString());
    } else if (id == QLatin1String("FileUpload_Enabled")) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::UploadFileEnabled);
    } else if (id == QLatin1String("FileUpload_MaxFileSize")) {
        setFileMaxFileSize(value.toULongLong());
    } else if (id == QLatin1String("Broadcasting_enabled")) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::BroadCastEnabled);
    } else if (id == QLatin1String("Message_VideoRecorderEnabled")) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::VideoRecorderEnabled);
    } else if (id == QLatin1String("Message_AudioRecorderEnabled")) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::AudioRecorderEnabled);
    } else if (id == QLatin1String("Assets_logo")) {
        setLogoUrl(parseConfigWithDefaultValue(value.toJsonObject()));
    } else if (id == QLatin1String("Assets_favicon")) {
        setFaviconUrl(parseConfigWithDefaultValue(value.toJsonObject()));
    } else if (id == QLatin1String("Accounts_AllowDeleteOwnAccount")) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::AllowDeleteOwnAccount);
    } else if (id == QLatin1String("Accounts_RegistrationForm")) {
        setAllowRegistrationFrom(value.toString());
    } else if (id == QLatin1String("Accounts_PasswordReset")) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::AllowPasswordReset);
    } else if (id == QLatin1String("Accounts_AllowEmailChange")) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::AllowEmailChange);
    } else if (id == QLatin1String("Accounts_AllowPasswordChange")) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::AllowPasswordChange);
    } else if (id == QLatin1String("Accounts_AllowUsernameChange")) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::AllowUsernameChange);
    } else if (id == QLatin1String("Accounts_AllowUserProfileChange")) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::AllowUserProfileChange);
    } else if (id == QLatin1String("Accounts_AllowUserAvatarChange")) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::AllowUserAvatarChange);
    } else if (id == QLatin1String("LDAP_Enable")) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::LdapEnabled);
    } else if (id == QLatin1String("Accounts_LoginExpiration")) {
        setLoginExpiration(value.toInt());
    } else if (id == QLatin1String("Accounts_TwoFactorAuthentication_Enabled")) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::TwoFactorAuthenticationEnabled);
    } else if (id == QLatin1String("Accounts_TwoFactorAuthentication_By_Email_Enabled")) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::TwoFactorAuthenticationByEmailEnabled);
    } else if (id == QLatin1String("Accounts_TwoFactorAuthentication_By_TOTP_Enabled")) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::TwoFactorAuthenticationByTOTPEnabled);
    } else if (id == QLatin1String("Accounts_TwoFactorAuthentication_Enforce_Password_Fallback")) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::TwoFactorAuthenticationEnforcePasswordFallback);
    } else if (id == QLatin1String("UTF8_Channel_Names_Validation")) {
        setChannelNameValidation(value.toString());
    } else if (id == QLatin1String("UTF8_User_Names_Validation")) {
        setUserNameValidation(value.toString());
    } else if (id == QLatin1String("Message_MaxAllowedSize")) {
        setMessageMaximumAllowedSize(value.toInt());
    } else if (id == QLatin1String("Message_AllowConvertLongMessagesToAttachment")) {
        setMessageAllowConvertLongMessagesToAttachment(value.toBool());
    } else if (id == QLatin1String("UI_Use_Real_Name")) {
        setUseRealName(value.toBool());
    } else if (id == QLatin1String("Accounts_AllowInvisibleStatusOption")) {
        setAccountsAllowInvisibleStatusOption(value.toBool());
    } else if (id == QLatin1String("UserData_EnableDownload")) {
        setUserDataDownloadEnabled(value.toBool());
    } else if (id == QLatin1String("Device_Management_Enable_Login_Emails")) {
        setDeviceManagementEnableLoginEmails(value.toBool());
    } else if (id == QLatin1String("Device_Management_Allow_Login_Email_preference")) {
        setDeviceManagementAllowLoginEmailpreference(value.toBool());
    } else if (id == QLatin1String("Message_GroupingPeriod")) {
        setMessageGroupingPeriod(value.toInt());
    } else if (id == QLatin1String("DirectMesssage_maxUsers")) {
        setDirectMessageMaximumUser(value.toInt());
    } else if (id == QLatin1String("Message_QuoteChainLimit")) {
        setMessageQuoteChainLimit(value.toInt());
    } else if (id == QLatin1String("Accounts_AllowUserStatusMessageChange")) {
        setAllowCustomStatusMessage(value.toBool());
    } else if (id == QLatin1String("FileUpload_MediaTypeWhiteList")) {
        setMediaWhiteList(value.toString().split(QLatin1Char(','), Qt::SkipEmptyParts));
    } else if (id == QLatin1String("FileUpload_MediaTypeBlackList")) {
        setMediaBlackList(value.toString().split(QLatin1Char(','), Qt::SkipEmptyParts));
    } else if (id == QLatin1String("Accounts_ShowFormLogin")) {
        if (value.toBool()) {
            mServerAuthTypes |= AuthenticationManager::AuthMethodType::Password;
        } else {
            mServerAuthTypes &= ~AuthenticationManager::AuthMethodType::Password;
        }
    } else if (id == QLatin1String("AuthenticationServerMethod")) {
        mServerAuthTypes = static_cast<AuthenticationManager::AuthMethodTypes>(value.toInt());
    } else if (id == QLatin1String("API_Embed")) {
        setPreviewEmbed(value.toBool());
    } else if (id == QLatin1String("API_EmbedCacheExpirationDays")) {
        qDebug() << "API_EmbedCacheExpirationDays value.toBool() " << value.toBool();
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
    v[QLatin1String("_id")] = identifier;
    v[QLatin1String("value")] = value;
    return v;
}

QJsonObject RuqolaServerConfig::createJsonObject(const QString &identifier, const RuqolaServerConfig::ConfigWithDefaultValue &value)
{
    QJsonObject v;
    v[QLatin1String("_id")] = identifier;
    QJsonObject customUrl;
    customUrl[QLatin1String("defaultUrl")] = value.defaultUrl;
    customUrl[QLatin1String("url")] = value.url;
    v[QLatin1String("value")] = customUrl;
    return v;
}

QJsonObject RuqolaServerConfig::createJsonObject(const QString &identifier, qint64 value)
{
    QJsonObject v;
    v[QLatin1String("_id")] = identifier;
    v[QLatin1String("value")] = static_cast<qint64>(value);
    return v;
}

QJsonObject RuqolaServerConfig::createJsonObject(const QString &identifier, bool value)
{
    QJsonObject v;
    v[QLatin1String("_id")] = identifier;
    v[QLatin1String("value")] = value;
    return v;
}

QJsonObject RuqolaServerConfig::createJsonObject(const QString &identifier, int value)
{
    QJsonObject v;
    v[QLatin1String("_id")] = identifier;
    v[QLatin1String("value")] = value;
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

    o[QLatin1String("result")] = array;
#if 0
} else if (id.contains(regExp)) {
    if (value.toBool()) {
        addOauthService(id);
    }
} else if (id == QLatin1String("Accounts_RegistrationForm")) {
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
    QJsonArray configs = obj.value(QLatin1String("result")).toArray();
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
    QJsonArray configs = obj.value(QLatin1String("result")).toArray();
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
        && mNeedAdaptNewSubscriptionRC60 == other.mNeedAdaptNewSubscriptionRC60
        && mMessageAllowConvertLongMessagesToAttachment == other.mMessageAllowConvertLongMessagesToAttachment && mUIUseRealName == other.mUIUseRealName
        && mServerConfigFeatureTypes == other.mServerConfigFeatureTypes && mMediaWhiteList == other.mMediaWhiteList && mMediaBlackList == other.mMediaBlackList
        && mLogoUrl == other.mLogoUrl && mFaviconUrl == other.mFaviconUrl && mLoginExpiration == other.mLoginExpiration
        && mMessageMaximumAllowedSize == other.mMessageMaximumAllowedSize && mMessageGroupingPeriod == other.mMessageGroupingPeriod
        && mDirectMessageMaximumUser == other.mDirectMessageMaximumUser && mMessageQuoteChainLimit == other.mMessageQuoteChainLimit
        && mHasEnterpriseSupport == other.mHasEnterpriseSupport && mAccountsAllowInvisibleStatusOption == other.mAccountsAllowInvisibleStatusOption
        && mUserDataDownloadEnabled == other.mUserDataDownloadEnabled && mDeviceManagementEnableLoginEmails == other.mDeviceManagementEnableLoginEmails
        && mDeviceManagementAllowLoginEmailpreference == other.mDeviceManagementAllowLoginEmailpreference
        && mAllowCustomStatusMessage == other.mAllowCustomStatusMessage && mPreviewEmbed == other.mPreviewEmbed;
}

void RuqolaServerConfig::loadAccountSettingsFromLocalDataBase(const QByteArray &ba)
{
    const QJsonDocument doc = QJsonDocument::fromJson(ba);
    const QJsonObject newObj = doc.object();
    deserialize(newObj);
}

#include "moc_ruqolaserverconfig.cpp"
