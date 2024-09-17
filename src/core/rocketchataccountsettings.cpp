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

    mServerUrl = mSetting->value(QLatin1StringView("serverURL"), QLatin1StringView("open.rocket.chat")).toString();
    mUserName = mSetting->value(QLatin1StringView("username")).toString();
    mUserId = mSetting->value(QLatin1StringView("userID")).toByteArray();
    mAuthToken = mSetting->value(QLatin1StringView("authToken")).toString();
    mExpireToken = mSetting->value(QLatin1StringView("expireToken")).toLongLong();
    mAccountName = mSetting->value(QLatin1StringView("accountName")).toString();
    mActivities = mSetting->value(QLatin1StringView("activities")).toStringList();
    mUseLdap = mSetting->value(QLatin1StringView("useLdap")).toBool();
    mAccountEnabled = mSetting->value(QLatin1StringView("enabled"), true).toBool();
    mActivityEnabled = mSetting->value(QLatin1StringView("ActivityEnabled"), false).toBool();
    mDisplayName = mSetting->value(QLatin1StringView("displayName")).toString();
    mLastCheckedPreviewUrlCacheDate = mSetting->value(QLatin1StringView("lastCheckedPreviewUrlDate")).toDate();
    mAuthMethodType = mSetting->value(QLatin1StringView("authenticationMethodType"), AuthenticationManager::AuthMethodType::Password)
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

bool RocketChatAccountSettings::activityEnabled() const
{
    return mActivityEnabled;
}

void RocketChatAccountSettings::setActivityEnabled(bool newActivityEnabled)
{
    if (mActivityEnabled == newActivityEnabled) {
        return;
    }

    mSetting->setValue(QStringLiteral("ActivityEnabled"), newActivityEnabled);
    mSetting->sync();
    mActivityEnabled = newActivityEnabled;
    Q_EMIT activitiesChanged();
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

    mSetting->setValue(QStringLiteral("activities"), newActivities);
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
        mSetting->setValue(QLatin1StringView("authenticationMethodType"), mAuthMethodType);
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
        mSetting->setValue(QLatin1StringView("lastCheckedPreviewUrlDate"), mLastCheckedPreviewUrlCacheDate);
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
        mSetting->setValue(QLatin1StringView("displayName"), mDisplayName);
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
        mSetting->setValue(QLatin1StringView("enabled"), mAccountEnabled);
        mSetting->sync();
        Q_EMIT enableAccountChanged();
    }
}

QByteArray RocketChatAccountSettings::lastSelectedRoom() const
{
    return mSetting->value(QLatin1StringView("SelectedRoom")).toByteArray();
}

void RocketChatAccountSettings::setLastSelectedRoom(const QByteArray &roomId)
{
    if (!roomId.isEmpty()) {
        mSetting->setValue(QLatin1StringView("SelectedRoom"), roomId);
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
        mSetting->setValue(QLatin1StringView("expireToken"), mExpireToken);
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
    mSetting->setValue(QLatin1StringView("userID"), userId);
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
        mSetting->setValue(QLatin1StringView("authToken"), authToken);
    }
}

void RocketChatAccountSettings::logout()
{
    mSetting->setValue(QLatin1StringView("authToken"), QString());
    mSetting->setValue(QLatin1StringView("expireToken"), -1);
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
        mSetting->setValue(QLatin1StringView("username"), mUserName);
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
    mSetting->setValue(QLatin1StringView("accountName"), accountName);
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

    mSetting->setValue(QLatin1StringView("serverURL"), serverUrl);
    mSetting->sync();
    mServerUrl = serverUrl;
    Q_EMIT serverURLChanged();
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
    return d;
}

#include "moc_rocketchataccountsettings.cpp"
