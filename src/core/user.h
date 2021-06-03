/*
   Copyright (c) 2017-2021 Laurent Montel <montel@kde.org>

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

#pragma once

#include "libruqolacore_export.h"
#include <QDateTime>
#include <QDebug>
#include <QString>
class LIBRUQOLACORE_EXPORT User
{
    Q_GADGET
public:
    enum class PresenceStatus {
        PresenceOnline,
        PresenceBusy,
        PresenceAway,
        PresenceOffline,
        Unknown,
    };
    Q_ENUM(PresenceStatus)

    struct LIBRUQOLACORE_EXPORT UserEmailsInfo {
        QString email;
        bool verified = false;
        Q_REQUIRED_RESULT bool operator==(const UserEmailsInfo &other) const
        {
            return (email == other.email) && (verified == other.verified);
        }

        Q_REQUIRED_RESULT bool isValid() const
        {
            return !email.isEmpty();
        }
    };

    User();
    ~User();

    Q_REQUIRED_RESULT QString name() const;
    void setName(const QString &name);

    Q_REQUIRED_RESULT QString userId() const;
    void setUserId(const QString &userId);

    Q_REQUIRED_RESULT User::PresenceStatus status() const;
    void setStatus(User::PresenceStatus status);

    void parseUser(const QJsonObject &json);
    Q_REQUIRED_RESULT QString iconFromStatus() const;

    Q_REQUIRED_RESULT bool operator==(const User &other) const;
    Q_REQUIRED_RESULT bool operator!=(const User &other) const;

    Q_REQUIRED_RESULT QString userName() const;
    void setUserName(const QString &userName);

    Q_REQUIRED_RESULT bool isValid() const;

    Q_REQUIRED_RESULT double utcOffset() const;
    void setUtcOffset(double utcOffset);

    Q_REQUIRED_RESULT QString statusText() const;
    void setStatusText(const QString &statusText);

    void parseUserRestApi(const QJsonObject &object);

    static Q_REQUIRED_RESULT QVector<User> parseUsersList(const QJsonObject &object);
    void parseUser(const QVariantList &list);

    Q_REQUIRED_RESULT QStringList roles() const;
    void setRoles(const QStringList &roles);

    Q_REQUIRED_RESULT QDateTime createdAt() const;
    void setCreatedAt(const QDateTime &createdAt);

    Q_REQUIRED_RESULT QDateTime lastLogin() const;
    void setLastLogin(const QDateTime &lastLogin);

    Q_REQUIRED_RESULT UserEmailsInfo userEmailsInfo() const;
    void setUserEmailsInfo(const UserEmailsInfo &userEmailsInfo);

    Q_REQUIRED_RESULT QStringList i18nRoles() const;

private:
    UserEmailsInfo mUserEmailsInfo;
    QDateTime mCreatedAt;
    QDateTime mLastLogin;

    PresenceStatus mStatus = PresenceStatus::PresenceOffline;
    QString mUserId;
    QString mName;
    QString mUserName;
    QString mStatusText;
    QStringList mRoles;
    QStringList mI18nRoles;
    double mUtcOffset = 0.0;
};
Q_DECLARE_METATYPE(User)
Q_DECLARE_TYPEINFO(User, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const User &t);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const User::UserEmailsInfo &t);

