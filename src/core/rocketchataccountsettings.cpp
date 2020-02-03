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

#include "rocketchataccountsettings.h"
#include "managerdatapaths.h"
#include "ruqola_debug.h"
#include <config-ruqola.h>

#include <QDateTime>
#include <QFile>
#include <QSettings>
#include <QStandardPaths>

#if HAVE_QT5KEYCHAIN
#include <qt5keychain/keychain.h>
using namespace QKeychain;
#endif

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

void RocketChatAccountSettings::initializeSettings(const QString &accountFileName)
{
    delete mSetting;
    mSetting = new QSettings(accountFileName, QSettings::IniFormat);
    qCDebug(RUQOLA_LOG) << "accountFileName "<<accountFileName;

    mServerUrl = mSetting->value(QStringLiteral("serverURL"), QStringLiteral("open.rocket.chat")).toString();
    mUserName = mSetting->value(QStringLiteral("username")).toString();
    mUserId = mSetting->value(QStringLiteral("userID")).toString();
    mAuthToken = mSetting->value(QStringLiteral("authToken")).toString();
    mExpireToken = mSetting->value(QStringLiteral("expireToken")).toLongLong();
    mAccountName = mSetting->value(QStringLiteral("accountName")).toString();
    mShowUnreadOnTop = mSetting->value(QStringLiteral("showunreadontop")).toBool();
    mAccountEnabled = mSetting->value(QStringLiteral("enabled"), true).toBool();
#if HAVE_QT5KEYCHAIN
    auto readJob = new ReadPasswordJob(QStringLiteral("Ruqola"), this);
    connect(readJob, &Job::finished, this, &RocketChatAccountSettings::slotPasswordRead);
    readJob->setKey(mAccountName);
    readJob->start();
#endif
}

void RocketChatAccountSettings::slotPasswordRead(QKeychain::Job *baseJob)
{
#if HAVE_QT5KEYCHAIN
    ReadPasswordJob *job = qobject_cast<ReadPasswordJob *>(baseJob);
    Q_ASSERT(job);
    if (!job->error()) {
        mPassword = job->textData();
        qCDebug(RUQOLA_LOG) << "OK, we have the password now";
        Q_EMIT passwordChanged();
    }
#else
    Q_UNUSED(baseJob);
#endif
}

void RocketChatAccountSettings::slotPasswordWritten(QKeychain::Job *baseJob)
{
#if HAVE_QT5KEYCHAIN
    if (baseJob->error()) {
        qCWarning(RUQOLA_LOG) << "Error writing password using QKeychain:" << baseJob->errorString();
    }
#endif
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

qint64 RocketChatAccountSettings::expireToken() const
{
    return mExpireToken;
}

void RocketChatAccountSettings::setExpireToken(const qint64 &expireToken)
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

bool RocketChatAccountSettings::showUnreadOnTop() const
{
    return mShowUnreadOnTop;
}

bool RocketChatAccountSettings::setShowUnreadOnTop(bool showUnreadOnTop)
{
    if (mShowUnreadOnTop != showUnreadOnTop) {
        mShowUnreadOnTop = showUnreadOnTop;
        mSetting->setValue(QStringLiteral("showunreadontop"), mShowUnreadOnTop);
        mSetting->sync();
        return true;
    }
    return false;
}

QString RocketChatAccountSettings::userId() const
{
    return mUserId;
}

void RocketChatAccountSettings::setUserId(const QString &userId)
{
    //Don't use if( m_userID != userID) as we need to Q_EMIT userIDChanged
    mUserId = userId;
    mSetting->setValue(QStringLiteral("userID"), userId);
    mSetting->sync();
    Q_EMIT userIDChanged();
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
    mPassword.clear();
    mExpireToken = -1;
}

QString RocketChatAccountSettings::password() const
{
    return mPassword;
}

void RocketChatAccountSettings::setPassword(const QString &password)
{
    mPassword = password;

#if HAVE_QT5KEYCHAIN
    auto writeJob = new WritePasswordJob(QStringLiteral("Ruqola"), this);
    connect(writeJob, &Job::finished, this, &RocketChatAccountSettings::slotPasswordWritten);
    writeJob->setKey(mAccountName);
    writeJob->setTextData(mPassword);
    writeJob->start();
#endif

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
        return QString();
    }
    if (mCachePath.isEmpty()) {
        mCachePath = ManagerDataPaths::self()->path(ManagerDataPaths::Cache, mAccountName);
    }
    return mCachePath;
}

void RocketChatAccountSettings::removeSettings()
{
    //Delete password
#if HAVE_QT5KEYCHAIN
    auto deleteJob = new DeletePasswordJob(QStringLiteral("Ruqola"));
    deleteJob->setKey(mAccountName);
    deleteJob->setAutoDelete(true);
    deleteJob->start();
#endif
    QFile f(mSetting->fileName());
    if (!f.remove()) {
        qCWarning(RUQOLA_LOG) << "Impossible to delete config file: " << mSetting->fileName();
    }
}
