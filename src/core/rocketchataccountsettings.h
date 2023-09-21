/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
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

    [[nodiscard]] QString userId() const;
    void setUserId(const QString &userId);

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
    [[nodiscard]] QString cacheBasePath();

    [[nodiscard]] QString password() const;
    void setPassword(const QString &password);

    void removeSettings();

    [[nodiscard]] qint64 expireToken() const;
    void setExpireToken(qint64 expireToken);

    [[nodiscard]] bool tokenExpired() const;

    [[nodiscard]] bool accountEnabled() const;
    void setAccountEnabled(bool accountEnabled);

    [[nodiscard]] QString lastSelectedRoom() const;
    void setLastSelectedRoom(const QString &roomId);

    [[nodiscard]] QString displayName() const;
    void setDisplayName(const QString &displayName);

Q_SIGNALS:
    void serverURLChanged();
    void userNameChanged();
    void userIdChanged();
    void accountNameChanged();
    void passwordChanged();
    void twoFactorAuthenticationCodeChanged();
    void enableAccountChanged();
    void displayNameChanged();

private:
    Q_DISABLE_COPY(RocketChatAccountSettings)
    LIBRUQOLACORE_NO_EXPORT void initializeSettings(const QString &accountFileName);
    LIBRUQOLACORE_NO_EXPORT void slotPasswordRead(QKeychain::Job *job);
    LIBRUQOLACORE_NO_EXPORT void slotPasswordWritten(QKeychain::Job *job);

    QString mUserId;
    QString mAuthToken;
    QString mServerUrl;
    QString mAccountName;
    QString mDisplayName;
    QString mCachePath;
    QString mUserName;
    QString mPassword;
    bool mUseLdap = false;
    QString mTwoFactorAuthenticationCode;
    qint64 mExpireToken = -1;
    QSettings *mSetting = nullptr;
    bool mAccountEnabled = true;
};
