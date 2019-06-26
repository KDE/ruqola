/*
   Copyright (c) 2017-2019 Montel Laurent <montel@kde.org>

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

#ifndef ROCKETCHATACCOUNTSETTINGS_H
#define ROCKETCHATACCOUNTSETTINGS_H

#include <QString>
#include <QObject>
#include "libruqola_private_export.h"

namespace QKeychain {
class Job;
}

class QSettings;
class LIBRUQOLACORE_EXPORT RocketChatAccountSettings : public QObject
{
    Q_OBJECT
public:
    explicit RocketChatAccountSettings(const QString &accountFileName = QString(), QObject *parent = nullptr);
    ~RocketChatAccountSettings();
    Q_REQUIRED_RESULT QString userId() const;
    void setUserId(const QString &userId);

    Q_REQUIRED_RESULT QString authToken() const;
    void setAuthToken(const QString &authToken);

    Q_REQUIRED_RESULT QString serverUrl() const;
    void setServerUrl(const QString &serverUrl);

    Q_REQUIRED_RESULT QString accountName() const;
    void setAccountName(const QString &accountName);

    void logout();
    Q_REQUIRED_RESULT QString cacheBasePath();

    Q_REQUIRED_RESULT QString userName() const;
    void setUserName(const QString &userName);

    Q_REQUIRED_RESULT QString password() const;
    void setPassword(const QString &password);

    Q_REQUIRED_RESULT QString code() const;
    void setCode(const QString &code);

    void removeSettings();

    Q_REQUIRED_RESULT bool showUnreadOnTop() const;
    bool setShowUnreadOnTop(bool showUnreadOnTop);

Q_SIGNALS:
    void serverURLChanged();
    void userNameChanged();
    void userIDChanged();
    void accountNameChanged();
    void passwordChanged();
    void codeChanged();

private:
    Q_DISABLE_COPY(RocketChatAccountSettings)
    void initializeSettings(const QString &accountFileName);
    void slotPasswordRead(QKeychain::Job *job);
    void slotPasswordWritten(QKeychain::Job *job);

    QString mUserId;
    QString mAuthToken;
    QString mServerUrl;
    QString mAccountName;
    QString mCachePath;
    QString mUserName;
    QString mPassword;
    QString mCode;
    QSettings *mSetting = nullptr;
    bool mShowUnreadOnTop = false;
};

#endif // ROCKETCHATACCOUNTSETTINGS_H
