/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rocketchataccountsettings.h"
#include "config-ruqola.h"
#include "managerdatapaths.h"
#include "ruqola_debug.h"
#include "ruqola_password_core_debug.h"

#include <QDateTime>
#include <QFile>
#include <QSettings>
#include <QStandardPaths>
#include <qt6keychain/keychain.h>
using namespace QKeychain;
using namespace Qt::Literals::StringLiterals;

RocketChatAccountSettings::RocketChatAccountSettings(const QString &accountFileName, QObject *parent)
    : QObject(parent)
{
    initializeSettings(accountFileName);
}

RocketChatAccountSettings::~RocketChatAccountSettings()
{
    mSetting->sync();
    delete mSetting;
}

bool RocketChatAccountSettings::isValid() const
{
    if (mAuthMethodType == AuthenticationManager::AuthMethodType::Password) {
        return !mServerUrl.isEmpty() && !mUserName.isEmpty();
    } else if (mAuthMethodType == AuthenticationManager::AuthMethodType::PersonalAccessToken) {
        return !mServerUrl.isEmpty() && !mAuthToken.isEmpty() && !mUserId.isEmpty();
    } else {
        qCWarning(RUQOLA_LOG) << "Need to define isValid for  " << mAuthMethodType;
    }
    return !mServerUrl.isEmpty() && !mUserName.isEmpty();
}

void RocketChatAccountSettings::initializeSettings(const QString &accountFileName)
{
    delete mSetting;
    mSetting = new QSettings(accountFileName, QSettings::IniFormat);
    qCDebug(RUQOLA_LOG) << "accountFileName " << accountFileName;

    mServerUrl = mSetting->value("serverURL"_L1, "open.rocket.chat"_L1).toString();
    mUserName = mSetting->value("username"_L1).toString();
    mUserId = mSetting->value("userID"_L1).toByteArray();
    mAuthToken = mSetting->value("authToken"_L1).toString();
    mExpireToken = mSetting->value("expireToken"_L1).toLongLong();
    mAccountName = mSetting->value("accountName"_L1).toString();
    mActivities = mSetting->value("activities"_L1).toStringList();
    mUseLdap = mSetting->value("useLdap"_L1).toBool();
    mAccountEnabled = mSetting->value("enabled"_L1, true).toBool();
    mActivityEnabled = mSetting->value("ActivityEnabled"_L1, false).toBool();
    mDisplayName = mSetting->value("displayName"_L1).toString();
    mLastCheckedPreviewUrlCacheDate = mSetting->value("lastCheckedPreviewUrlDate"_L1).toDate();
    mInviteToken = mSetting->value("inviteToken"_L1).toString();
    mAuthMethodType =
        mSetting->value("authenticationMethodType"_L1, AuthenticationManager::AuthMethodType::Password).value<AuthenticationManager::AuthMethodType>();
    mKeySaved = mSetting->value("keySaved"_L1, false).toBool();
    // Password is ok when we use Password authentication method.
    // Not sure for other.
    if (mAccountEnabled && !mAccountName.isEmpty()) {
        qCDebug(RUQOLA_PASSWORD_CORE_LOG) << "Load password from QKeychain: accountname " << mAccountName;
        auto readJob = new ReadPasswordJob(u"Ruqola"_s);
        connect(readJob, &Job::finished, this, &RocketChatAccountSettings::slotPasswordRead);
        readJob->setKey(mAccountName);
        readJob->start();
    }
}

void RocketChatAccountSettings::slotPasswordRead(QKeychain::Job *baseJob)
{
    auto job = qobject_cast<ReadPasswordJob *>(baseJob);
    Q_ASSERT(job);
    if (!job->error()) {
        mPassword = job->textData();
        qCDebug(RUQOLA_PASSWORD_CORE_LOG) << "OK, we have the password now";
        Q_EMIT passwordAvailable();
    } else {
        qCWarning(RUQOLA_PASSWORD_CORE_LOG) << "We have an error during reading password " << job->errorString() << " Account name " << mAccountName;
    }
}

void RocketChatAccountSettings::slotPasswordWritten(QKeychain::Job *baseJob)
{
    if (baseJob->error()) {
        qCWarning(RUQOLA_PASSWORD_CORE_LOG) << "Error writing password using QKeychain:" << baseJob->errorString();
    }
}

QString RocketChatAccountSettings::inviteToken() const
{
    return mInviteToken;
}

void RocketChatAccountSettings::setInviteToken(const QString &newInviteToken)
{
    if (mInviteToken == newInviteToken) {
        return;
    }

    mSetting->setValue("inviteToken"_L1, newInviteToken);
    mSetting->sync();
    mInviteToken = newInviteToken;
}

bool RocketChatAccountSettings::activityEnabled() const
{
    return mActivityEnabled;
}

void RocketChatAccountSettings::setActivityEnabled(bool newActivityEnabled)
{
    if (mActivityEnabled == newActivityEnabled) {
        return;
    }

    mSetting->setValue(u"ActivityEnabled"_s, newActivityEnabled);
    mSetting->sync();
    mActivityEnabled = newActivityEnabled;
    Q_EMIT activitiesChanged();
}

int RocketChatAccountSettings::cacheVersion() const
{
    return mSetting->value("cacheVersion"_L1, 0).toInt();
}

void RocketChatAccountSettings::setCacheVersion(int newVersion)
{
    mSetting->setValue("cacheVersion"_L1, newVersion);
    mSetting->sync();
}

QStringList RocketChatAccountSettings::activities() const
{
    return mActivities;
}

void RocketChatAccountSettings::setActivities(const QStringList &newActivities)
{
    if (mActivities == newActivities) {
        return;
    }

    mSetting->setValue(u"activities"_s, newActivities);
    mSetting->sync();
    mActivities = newActivities;
    Q_EMIT activitiesChanged();
}

AuthenticationManager::AuthMethodType RocketChatAccountSettings::authMethodType() const
{
    return mAuthMethodType;
}

void RocketChatAccountSettings::setAuthMethodType(AuthenticationManager::AuthMethodType newAuthMethodType)
{
    if (mAuthMethodType != newAuthMethodType) {
        mAuthMethodType = newAuthMethodType;
        mSetting->setValue("authenticationMethodType"_L1, mAuthMethodType);
        mSetting->sync();
    }
}

QDate RocketChatAccountSettings::lastCheckedPreviewUrlCacheDate() const
{
    return mLastCheckedPreviewUrlCacheDate;
}

void RocketChatAccountSettings::setLastCheckedPreviewUrlCacheDate(const QDate &newLastCheckedPreviewUrlCacheDate)
{
    if (mLastCheckedPreviewUrlCacheDate != newLastCheckedPreviewUrlCacheDate) {
        mLastCheckedPreviewUrlCacheDate = newLastCheckedPreviewUrlCacheDate;
        mSetting->setValue("lastCheckedPreviewUrlDate"_L1, mLastCheckedPreviewUrlCacheDate);
        mSetting->sync();
    }
}

QString RocketChatAccountSettings::displayName() const
{
    if (mDisplayName.isEmpty()) {
        return mAccountName;
    }
    return mDisplayName;
}

void RocketChatAccountSettings::setDisplayName(const QString &displayName)
{
    if (mDisplayName != displayName) {
        mDisplayName = displayName;
        mSetting->setValue("displayName"_L1, mDisplayName);
        mSetting->sync();
    }
}

bool RocketChatAccountSettings::accountEnabled() const
{
    return isValid() && mAccountEnabled;
}

void RocketChatAccountSettings::setAccountEnabled(bool enabled)
{
    if (mAccountEnabled != enabled) {
        mAccountEnabled = enabled;
        mSetting->setValue("enabled"_L1, mAccountEnabled);
        mSetting->sync();
        Q_EMIT enableAccountChanged();
    }
}

QByteArray RocketChatAccountSettings::lastSelectedRoom() const
{
    return mSetting->value("SelectedRoom"_L1).toByteArray();
}

void RocketChatAccountSettings::setLastSelectedRoom(const QByteArray &roomId)
{
    if (!roomId.isEmpty()) {
        mSetting->setValue("SelectedRoom"_L1, roomId);
    }
}

qint64 RocketChatAccountSettings::expireToken() const
{
    return mExpireToken;
}

void RocketChatAccountSettings::setExpireToken(qint64 expireToken)
{
    if (mExpireToken != expireToken) {
        mExpireToken = expireToken;
        mSetting->setValue("expireToken"_L1, mExpireToken);
        mSetting->sync();
    }
}

bool RocketChatAccountSettings::tokenExpired() const
{
    return mExpireToken < QDateTime::currentMSecsSinceEpoch();
}

QByteArray RocketChatAccountSettings::userId() const
{
    return mUserId;
}

void RocketChatAccountSettings::setUserId(const QByteArray &userId)
{
    // Don't use if( m_userID != userID) as we need to Q_EMIT userIDChanged
    mUserId = userId;
    mSetting->setValue("userID"_L1, userId);
    mSetting->sync();
    Q_EMIT userIdChanged();
}

QString RocketChatAccountSettings::authToken() const
{
    return mAuthToken;
}

void RocketChatAccountSettings::setAuthToken(const QString &authToken)
{
    if (mAuthToken != authToken) {
        qCDebug(RUQOLA_LOG) << "Setting token to" << authToken;
        mAuthToken = authToken;
        mSetting->setValue("authToken"_L1, authToken);
    }
}

void RocketChatAccountSettings::logout()
{
    mSetting->setValue("authToken"_L1, QString());
    mSetting->setValue("expireToken"_L1, -1);
    mSetting->sync();
    mAuthToken.clear();
    mUserId.clear();
    mExpireToken = -1;
}

QString RocketChatAccountSettings::password() const
{
    return mPassword;
}

void RocketChatAccountSettings::setPassword(const QString &password)
{
    if (mPassword != password) {
        mPassword = password;

        auto writeJob = new WritePasswordJob(u"Ruqola"_s);
        connect(writeJob, &Job::finished, this, &RocketChatAccountSettings::slotPasswordWritten);
        writeJob->setKey(mAccountName);
        writeJob->setTextData(mPassword);
        writeJob->start();
    }
}

QString RocketChatAccountSettings::twoFactorAuthenticationCode() const
{
    return mTwoFactorAuthenticationCode;
}

void RocketChatAccountSettings::setTwoFactorAuthenticationCode(const QString &twoFactorAuthenticationCode)
{
    if (mTwoFactorAuthenticationCode != twoFactorAuthenticationCode) {
        mTwoFactorAuthenticationCode = twoFactorAuthenticationCode;

        Q_EMIT twoFactorAuthenticationCodeChanged();
    }
}

QString RocketChatAccountSettings::userName() const
{
    return mUserName;
}

void RocketChatAccountSettings::setUserName(const QString &userName)
{
    if (mUserName != userName) {
        mUserName = userName;
        mSetting->setValue("username"_L1, mUserName);
        mSetting->sync();
        Q_EMIT userNameChanged();
    }
}

QString RocketChatAccountSettings::accountName() const
{
    return mAccountName;
}

void RocketChatAccountSettings::setAccountName(const QString &accountName)
{
    if (mAccountName == accountName) {
        return;
    }

    initializeSettings(ManagerDataPaths::self()->accountConfigFileName(accountName));
    mSetting->setValue("accountName"_L1, accountName);
    mSetting->sync();
    mAccountName = accountName;
    Q_EMIT accountNameChanged();
}

QString RocketChatAccountSettings::serverUrl() const
{
    return mServerUrl;
}

void RocketChatAccountSettings::setServerUrl(const QString &serverUrl)
{
    if (mServerUrl == serverUrl) {
        return;
    }

    mSetting->setValue("serverURL"_L1, serverUrl);
    mSetting->sync();
    mServerUrl = serverUrl;
    Q_EMIT serverURLChanged();
}

void RocketChatAccountSettings::removeSettings()
{
    // Delete password
    auto deleteJob = new DeletePasswordJob(u"Ruqola"_s);
    deleteJob->setKey(mAccountName);
    deleteJob->start();
    QFile f(mSetting->fileName());
    if (!f.remove()) {
        qCWarning(RUQOLA_LOG) << "Impossible to delete config file: " << mSetting->fileName();
    }
}

bool RocketChatAccountSettings::keySaved() const
{
    return mKeySaved;
}

void RocketChatAccountSettings::setKeySaved(bool newKeySaved)
{
    if (mKeySaved != newKeySaved) {
        mKeySaved = newKeySaved;
        if (mKeySaved) {
            mSetting->setValue("keySaved"_L1, true);
        } else {
            mSetting->remove("keySaved"_L1);
        }
        mSetting->sync();
    }
}

QDebug operator<<(QDebug d, const RocketChatAccountSettings &t)
{
    d.space() << "mAuthMethodType" << t.authMethodType();
    d.space() << "mUserId" << t.userId();
    d.space() << "mAuthToken" << t.authToken();
    d.space() << "mServerUrl" << t.serverUrl();
    d.space() << "mAccountName" << t.accountName();
    d.space() << "mDisplayName" << t.displayName();
    d.space() << "mUserName" << t.userName();
    d.space() << "mTwoFactorAuthenticationCode" << t.twoFactorAuthenticationCode();
    d.space() << "mExpireToken" << t.expireToken();
    d.space() << "mAccountEnabled" << t.accountEnabled();
    d.space() << "mActivities" << t.activities();
    d.space() << "mActivityEnabled" << t.activityEnabled();
    d.space() << "mKeySaved" << t.keySaved();
    d.space() << "mInviteToken" << t.inviteToken();
    return d;
}

#include "moc_rocketchataccountsettings.cpp"
