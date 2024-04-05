/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rocketchataccountsettings.h"
#include "config-ruqola.h"
#include "managerdatapaths.h"
#include "ruqola_debug.h"
#include "ruqola_password_core_debug.h"

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QSettings>
#include <QStandardPaths>
#include <qt6keychain/keychain.h>
using namespace QKeychain;

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

    mServerUrl = mSetting->value(QStringLiteral("serverURL"), QStringLiteral("open.rocket.chat")).toString();
    mUserName = mSetting->value(QStringLiteral("username")).toString();
    mUserId = mSetting->value(QStringLiteral("userID")).toByteArray();
    mAuthToken = mSetting->value(QStringLiteral("authToken")).toString();
    mExpireToken = mSetting->value(QStringLiteral("expireToken")).toLongLong();
    mAccountName = mSetting->value(QStringLiteral("accountName")).toString();
    mActivities = mSetting->value(QStringLiteral("activites")).toStringList();
    mUseLdap = mSetting->value(QStringLiteral("useLdap")).toBool();
    mAccountEnabled = mSetting->value(QStringLiteral("enabled"), true).toBool();
    mDisplayName = mSetting->value(QStringLiteral("displayName")).toString();
    mLastCheckedPreviewUrlCacheDate = mSetting->value(QStringLiteral("lastCheckedPreviewUrlDate")).toDate();
    mAuthMethodType = mSetting->value(QStringLiteral("authenticationMethodType"), AuthenticationManager::AuthMethodType::Password)
                          .value<AuthenticationManager::AuthMethodType>();

    // Password is ok when we use Password authentication method.
    // Not sure for other.
    if (mAccountEnabled && !mAccountName.isEmpty()) {
        qCDebug(RUQOLA_PASSWORD_CORE_LOG) << "Load password from QKeychain: accountname " << mAccountName;
        auto readJob = new ReadPasswordJob(QStringLiteral("Ruqola"), this);
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
        Q_EMIT passwordChanged();
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

QStringList RocketChatAccountSettings::activities() const
{
    return mActivities;
}

void RocketChatAccountSettings::setActivities(const QStringList &newActivities)
{
    if (mActivities == newActivities) {
        return;
    }

    mSetting->setValue(QStringLiteral("activites"), newActivities);
    mSetting->sync();
    mActivities = newActivities;
    Q_EMIT activitiesChanged();
}

AuthenticationManager::AuthMethodType RocketChatAccountSettings::authMethodType() const
{
    return mAuthMethodType;
}

void RocketChatAccountSettings::setAuthMethodType(const AuthenticationManager::AuthMethodType &newAuthMethodType)
{
    if (mAuthMethodType != newAuthMethodType) {
        mAuthMethodType = newAuthMethodType;
        mSetting->setValue(QStringLiteral("authenticationMethodType"), mAuthMethodType);
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
        mSetting->setValue(QStringLiteral("lastCheckedPreviewUrlDate"), mLastCheckedPreviewUrlCacheDate);
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
        mSetting->setValue(QStringLiteral("displayName"), mDisplayName);
        mSetting->sync();
        Q_EMIT displayNameChanged();
    }
}

bool RocketChatAccountSettings::accountEnabled() const
{
    return mAccountEnabled;
}

void RocketChatAccountSettings::setAccountEnabled(bool enabled)
{
    if (mAccountEnabled != enabled) {
        mAccountEnabled = enabled;
        mSetting->setValue(QStringLiteral("enabled"), mAccountEnabled);
        mSetting->sync();
        Q_EMIT enableAccountChanged();
    }
}

QByteArray RocketChatAccountSettings::lastSelectedRoom() const
{
    return mSetting->value(QStringLiteral("SelectedRoom")).toByteArray();
}

void RocketChatAccountSettings::setLastSelectedRoom(const QByteArray &roomId)
{
    if (!roomId.isEmpty()) {
        mSetting->setValue(QStringLiteral("SelectedRoom"), roomId);
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
        mSetting->setValue(QStringLiteral("expireToken"), mExpireToken);
        mSetting->sync();
    }
}

bool RocketChatAccountSettings::tokenExpired() const
{
    return mExpireToken < QDateTime::currentDateTime().toMSecsSinceEpoch();
}

QByteArray RocketChatAccountSettings::userId() const
{
    return mUserId;
}

void RocketChatAccountSettings::setUserId(const QByteArray &userId)
{
    // Don't use if( m_userID != userID) as we need to Q_EMIT userIDChanged
    mUserId = userId;
    mSetting->setValue(QStringLiteral("userID"), userId);
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
        mSetting->setValue(QStringLiteral("authToken"), authToken);
    }
}

void RocketChatAccountSettings::logout()
{
    mSetting->setValue(QStringLiteral("authToken"), QString());
    mSetting->setValue(QStringLiteral("expireToken"), -1);
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
    mPassword = password;

    auto writeJob = new WritePasswordJob(QStringLiteral("Ruqola"), this);
    connect(writeJob, &Job::finished, this, &RocketChatAccountSettings::slotPasswordWritten);
    writeJob->setKey(mAccountName);
    writeJob->setTextData(mPassword);
    writeJob->start();

    Q_EMIT passwordChanged();
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
        mSetting->setValue(QStringLiteral("username"), mUserName);
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
    mSetting->setValue(QStringLiteral("accountName"), accountName);
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

    mSetting->setValue(QStringLiteral("serverURL"), serverUrl);
    mSetting->sync();
    mServerUrl = serverUrl;
    Q_EMIT serverURLChanged();
}

QString RocketChatAccountSettings::cacheBasePath()
{
    if (mServerUrl.isEmpty()) {
        return {};
    }
    if (mCachePath.isEmpty()) {
        mCachePath = ManagerDataPaths::self()->path(ManagerDataPaths::Cache, mAccountName);
        QDir dir;
        if (!dir.mkpath(mCachePath)) {
            qCWarning(RUQOLA_LOG) << "Impossible to create cache directory" << mCachePath;
        }
    }
    return mCachePath;
}

void RocketChatAccountSettings::removeSettings()
{
    // Delete password
    auto deleteJob = new DeletePasswordJob(QStringLiteral("Ruqola"));
    deleteJob->setKey(mAccountName);
    deleteJob->start();
    QFile f(mSetting->fileName());
    if (!f.remove()) {
        qCWarning(RUQOLA_LOG) << "Impossible to delete config file: " << mSetting->fileName();
    }

    const QString storeCachePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + QLatin1Char('/') + mAccountName + QLatin1Char('/');
    QDir dir(storeCachePath);
    if (!dir.removeRecursively()) {
        qCWarning(RUQOLA_LOG) << "Impossible to delete cache dir: " << storeCachePath;
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
    return d;
}

#include "moc_rocketchataccountsettings.cpp"
