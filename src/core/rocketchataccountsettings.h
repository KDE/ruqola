/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "authenticationmanager.h"
#include "libruqolacore_export.h"
#include <QDate>
#include <QObject>
#include <QString>

namespace QKeychain
{
class Job;
}

class QSettings;
class LIBRUQOLACORE_EXPORT RocketChatAccountSettings : public QObject
{
    Q_OBJECT
public:
    explicit RocketChatAccountSettings(const QString &accountFileName = QString(), QObject *parent = nullptr);
    ~RocketChatAccountSettings() override;

    [[nodiscard]] bool isValid() const;

    [[nodiscard]] QByteArray userId() const;
    void setUserId(const QByteArray &userId);

    [[nodiscard]] QString authToken() const;
    void setAuthToken(const QString &authToken);

    // account info
    [[nodiscard]] QString serverUrl() const;
    void setServerUrl(const QString &serverUrl);

    [[nodiscard]] QString accountName() const;
    void setAccountName(const QString &accountName);

    [[nodiscard]] QString userName() const;
    void setUserName(const QString &userName);

    [[nodiscard]] QString twoFactorAuthenticationCode() const;
    void setTwoFactorAuthenticationCode(const QString &twoFactorAuthenticationCode);

    void logout();

    [[nodiscard]] QString password() const;
    void setPassword(const QString &password);

    void removeSettings();

    [[nodiscard]] qint64 expireToken() const;
    void setExpireToken(qint64 expireToken);

    [[nodiscard]] bool tokenExpired() const;

    [[nodiscard]] bool accountEnabled() const;
    void setAccountEnabled(bool accountEnabled);

    [[nodiscard]] QByteArray lastSelectedRoom() const;
    void setLastSelectedRoom(const QByteArray &roomId);

    [[nodiscard]] QString displayName() const;
    void setDisplayName(const QString &displayName);

    [[nodiscard]] QDate lastCheckedPreviewUrlCacheDate() const;
    void setLastCheckedPreviewUrlCacheDate(const QDate &newLastCheckedPreviewUrlCacheDate);

    [[nodiscard]] AuthenticationManager::AuthMethodType authMethodType() const;
    void setAuthMethodType(AuthenticationManager::AuthMethodType newAuthMethodType);

    [[nodiscard]] QStringList activities() const;
    void setActivities(const QStringList &newActivities);

    [[nodiscard]] bool activityEnabled() const;
    void setActivityEnabled(bool newActivityEnabled);

    [[nodiscard]] int cacheVersion() const;
    void setCacheVersion(int newVersion);

    [[nodiscard]] bool keySaved() const;
    void setKeySaved(bool newKeySaved);

    [[nodiscard]] QString inviteToken() const;
    void setInviteToken(const QString &newInviteToken);

Q_SIGNALS:
    void serverURLChanged();
    void userNameChanged();
    void userIdChanged();
    void accountNameChanged();
    void passwordAvailable();
    void twoFactorAuthenticationCodeChanged();
    void enableAccountChanged();
    void activitiesChanged();

private:
    LIBRUQOLACORE_NO_EXPORT void initializeSettings(const QString &accountFileName);
    LIBRUQOLACORE_NO_EXPORT void slotPasswordRead(QKeychain::Job *job);
    LIBRUQOLACORE_NO_EXPORT void slotPasswordWritten(QKeychain::Job *job);

    AuthenticationManager::AuthMethodType mAuthMethodType = AuthenticationManager::AuthMethodType::Unknown; // By default
    QByteArray mUserId;
    QString mAuthToken;
    QString mServerUrl;
    QString mAccountName;
    QString mDisplayName;
    QString mUserName;
    QString mPassword;
    QString mInviteToken;
    QStringList mActivities;
    QDate mLastCheckedPreviewUrlCacheDate;
    QString mTwoFactorAuthenticationCode;
    qint64 mExpireToken = -1;
    QSettings *mSetting = nullptr;
    bool mKeySaved = false;
    bool mUseLdap = false;
    bool mAccountEnabled = true;
    bool mActivityEnabled = false;
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const RocketChatAccountSettings &t);
