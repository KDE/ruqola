/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

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
        [[nodiscard]] bool operator==(const UserEmailsInfo &other) const
        {
            return (email == other.email) && (verified == other.verified);
        }

        [[nodiscard]] bool isValid() const
        {
            return !email.isEmpty();
        }
    };

    User();
    ~User();

    [[nodiscard]] QString name() const;
    void setName(const QString &name);

    [[nodiscard]] QString userId() const;
    void setUserId(const QString &userId);

    [[nodiscard]] User::PresenceStatus status() const;
    void setStatus(User::PresenceStatus status);

    void parseUser(const QJsonObject &json);
    [[nodiscard]] QString iconFromStatus() const;

    [[nodiscard]] bool operator==(const User &other) const;
    [[nodiscard]] bool operator!=(const User &other) const;

    [[nodiscard]] QString userName() const;
    void setUserName(const QString &userName);

    [[nodiscard]] bool isValid() const;

    [[nodiscard]] double utcOffset() const;
    void setUtcOffset(double utcOffset);

    [[nodiscard]] QString statusText() const;
    void setStatusText(const QString &statusText);

    void parseUserRestApi(const QJsonObject &object, const QList<RoleInfo> &roleInfo);

    [[nodiscard]] static QList<User> parseUsersList(const QJsonObject &object, const QList<RoleInfo> &roleInfo);
    void parseUser(const QVariantList &list);

    [[nodiscard]] QStringList roles() const;
    void setRoles(const QStringList &roles, const QList<RoleInfo> &roleInfo);

    [[nodiscard]] QDateTime createdAt() const;
    void setCreatedAt(const QDateTime &createdAt);

    [[nodiscard]] QDateTime lastLogin() const;
    void setLastLogin(const QDateTime &lastLogin);

    [[nodiscard]] UserEmailsInfo userEmailsInfo() const;
    void setUserEmailsInfo(const UserEmailsInfo &userEmailsInfo);

    [[nodiscard]] QStringList i18nRoles() const;

    [[nodiscard]] bool active() const;
    void setActive(bool newActive);

    [[nodiscard]] QString generateStatusStr() const;

    [[nodiscard]] bool requirePasswordChange() const;
    void setRequirePasswordChange(bool newRequirePasswordChange);

    [[nodiscard]] static QString roleI18n(const QString &roleStr, const QList<RoleInfo> &roleInfo);

    [[nodiscard]] QString bio() const;
    void setBio(const QString &newBio);

    static QJsonObject serialize(const User &user);
    static User deserialize(const QJsonObject &o);

    [[nodiscard]] QString nickName() const;
    void setNickName(const QString &newNickName);

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
    QString mNickName;
    QStringList mRoles;
    QStringList mI18nRoles;
    double mUtcOffset = 0.0;
    bool mActive = true;
    bool mRequirePasswordChange = false;
};
Q_DECLARE_METATYPE(User)
Q_DECLARE_TYPEINFO(User, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const User &t);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const User::UserEmailsInfo &t);
