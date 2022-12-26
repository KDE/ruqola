/*
   SPDX-FileCopyrightText: 2017-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "roles/roleinfo.h"
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

    void parseUserRestApi(const QJsonObject &object, const QVector<RoleInfo> &roleInfo);

    Q_REQUIRED_RESULT static QVector<User> parseUsersList(const QJsonObject &object, const QVector<RoleInfo> &roleInfo);
    void parseUser(const QVariantList &list);

    Q_REQUIRED_RESULT QStringList roles() const;
    void setRoles(const QStringList &roles, const QVector<RoleInfo> &roleInfo);

    Q_REQUIRED_RESULT QDateTime createdAt() const;
    void setCreatedAt(const QDateTime &createdAt);

    Q_REQUIRED_RESULT QDateTime lastLogin() const;
    void setLastLogin(const QDateTime &lastLogin);

    Q_REQUIRED_RESULT UserEmailsInfo userEmailsInfo() const;
    void setUserEmailsInfo(const UserEmailsInfo &userEmailsInfo);

    Q_REQUIRED_RESULT QStringList i18nRoles() const;

    Q_REQUIRED_RESULT bool active() const;
    void setActive(bool newActive);

    Q_REQUIRED_RESULT QString generateStatusStr() const;

    Q_REQUIRED_RESULT bool requirePasswordChange() const;
    void setRequirePasswordChange(bool newRequirePasswordChange);

    Q_REQUIRED_RESULT static QString roleI18n(const QString &roleStr, const QVector<RoleInfo> &roleInfo);

    Q_REQUIRED_RESULT QString bio() const;
    void setBio(const QString &newBio);

private:
    UserEmailsInfo mUserEmailsInfo;
    QDateTime mCreatedAt;
    QDateTime mLastLogin;

    PresenceStatus mStatus = PresenceStatus::PresenceOffline;
    QString mUserId;
    QString mName;
    QString mUserName;
    QString mStatusText;
    QString mBio;
    QStringList mRoles;
    QStringList mI18nRoles;
    double mUtcOffset = 0.0;
    bool mActive = true;
    bool mRequirePasswordChange = false;
};
Q_DECLARE_METATYPE(User)
Q_DECLARE_TYPEINFO(User, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const User &t);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const User::UserEmailsInfo &t);
