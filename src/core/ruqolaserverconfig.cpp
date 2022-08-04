/*
   SPDX-FileCopyrightText: 2017-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolaserverconfig.h"
#include "ruqola_debug.h"
#include <QJsonArray>
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

bool RuqolaServerConfig::ruqolaHasSupportForOauthType(AuthenticationManager::OauthType type) const
{
    return mRuqolaOauthTypes & type;
}

bool RuqolaServerConfig::canShowOauthService(AuthenticationManager::OauthType type) const
{
    return serverHasSupportForOauthType(type) && ruqolaHasSupportForOauthType(type);
}

void RuqolaServerConfig::addRuqolaAuthenticationSupport(AuthenticationManager::OauthType type)
{
    mRuqolaOauthTypes |= type;
}

bool RuqolaServerConfig::serverHasSupportForOauthType(AuthenticationManager::OauthType type) const
{
    return mServerOauthTypes & type;
}

void RuqolaServerConfig::addOauthService(const QString &service)
{
    const QString serviceLower = service.toLower();
    if (serviceLower.endsWith(QLatin1String("twitter"))) {
        mServerOauthTypes |= AuthenticationManager::OauthType::Twitter;
    } else if (serviceLower.endsWith(QLatin1String("facebook"))) {
        mServerOauthTypes |= AuthenticationManager::OauthType::FaceBook;
    } else if (serviceLower.endsWith(QLatin1String("github"))) {
        mServerOauthTypes |= AuthenticationManager::OauthType::GitHub;
    } else if (serviceLower.endsWith(QLatin1String("gitlab"))) {
        mServerOauthTypes |= AuthenticationManager::OauthType::GitLab;
    } else if (serviceLower.endsWith(QLatin1String("google"))) {
        mServerOauthTypes |= AuthenticationManager::OauthType::Google;
    } else if (serviceLower.endsWith(QLatin1String("linkedin"))) {
        mServerOauthTypes |= AuthenticationManager::OauthType::Linkedin;
    } else if (serviceLower.endsWith(QLatin1String("wordpress"))) {
        mServerOauthTypes |= AuthenticationManager::OauthType::Wordpress;
    } else if (serviceLower.endsWith(QLatin1String("_oauth_proxy_host"))) {
        // Hide warning as it's not a service
        qCDebug(RUQOLA_LOG) << "_OAuth_Proxy_host found ";
    } else if (serviceLower.endsWith(QLatin1String("_oauth_meteor"))) {
        qCDebug(RUQOLA_LOG) << "Accounts_OAuth_Meteor found ";
    } else {
        qCDebug(RUQOLA_LOG) << "Unknown service type: " << service;
    }
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
        if (updateArray.at(0).toString() == QLatin1String("updated")) {
            loadSettings(updateArray.at(1).toObject());
            qDebug() << "Update settings " << *this;
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
    if (registrationFromValue == QStringLiteral("Public")) {
        mServerConfigFeatureTypes |= ServerConfigFeatureType::RegistrationFromEnabled;
    } else if (registrationFromValue == QStringLiteral("Disabled")) {
    } else if (registrationFromValue == QStringLiteral("Secret URL")) {
        // TODO implement it.
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

AuthenticationManager::OauthTypes RuqolaServerConfig::ruqolaOauthTypes() const
{
    return mRuqolaOauthTypes;
}

AuthenticationManager::OauthTypes RuqolaServerConfig::serverOauthTypes() const
{
    return mServerOauthTypes;
}

QString RuqolaServerConfig::logoUrl() const
{
    return mLogoUrl;
}

void RuqolaServerConfig::setLogoUrl(const QString &url)
{
    mLogoUrl = url;
}

QString RuqolaServerConfig::faviconUrl() const
{
    return mFaviconUrl;
}

void RuqolaServerConfig::setFaviconUrl(const QString &url)
{
    mFaviconUrl = url;
}

QDebug operator<<(QDebug d, const RuqolaServerConfig &t)
{
    d << "mUniqueId  " << t.uniqueId();
    d << "mJitsiMeetUrl " << t.jitsiMeetUrl();
    d << "mJitsiMeetPrefix " << t.jitsiMeetPrefix();
    d << "mFileUploadStorageType " << t.fileUploadStorageType();
    d << "mSiteUrl " << t.siteUrl();
    d << "mSiteName " << t.siteName();
    d << "mServerOauthTypes " << t.serverOauthTypes();
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
    d << "mViewMode " << t.viewMode();
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

void RuqolaServerConfig::loadSettings(const QJsonObject &currentConfObject)
{
    const QString id = currentConfObject[QStringLiteral("_id")].toString();
    const QVariant value = currentConfObject[QStringLiteral("value")].toVariant();
    static const QRegularExpression regExp(QStringLiteral("^Accounts_OAuth_\\w+"));
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
    } else if (id.contains(regExp)) {
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
    } else if (id == QLatin1String("Message_AllowSnippeting")) {
        assignSettingValue(value.toBool(), ServerConfigFeatureType::AllowMessageSnippeting);
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
        setLogoUrl(value.toJsonObject()[QStringLiteral("url")].toString());
    } else if (id == QLatin1String("Assets_favicon")) {
        setFaviconUrl(value.toJsonObject()[QStringLiteral("url")].toString());
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
    } else if (id == QLatin1String("Accounts_Default_User_Preferences_viewMode")) {
        setViewMode(value.toInt());
    } else {
        qCDebug(RUQOLA_LOG) << "Other public settings id " << id << value;
    }
}

int RuqolaServerConfig::viewMode() const
{
    return mViewMode;
}

void RuqolaServerConfig::setViewMode(int newViewMode)
{
    mViewMode = newViewMode;
}

bool RuqolaServerConfig::useRealName() const
{
    return mUIUseRealName;
}

void RuqolaServerConfig::setUseRealName(bool newUIUseRealName)
{
    mUIUseRealName = newUIUseRealName;
}

void RuqolaServerConfig::parsePublicSettings(const QJsonObject &obj)
{
    // qDebug() << " void RuqolaServerConfig::parsePublicSettings(const QJsonObject &obj)" << obj;
    QJsonArray configs = obj.value(QLatin1String("result")).toArray();
    mServerConfigFeatureTypes = ServerConfigFeatureType::None;
    for (QJsonValueRef currentConfig : configs) {
        const QJsonObject currentConfObject = currentConfig.toObject();
        loadSettings(currentConfObject);
    }
    // TODO add Accounts_AllowUserStatusMessageChange when we will have a RestAPI method for it.
}
