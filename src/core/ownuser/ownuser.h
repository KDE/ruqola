/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "ownuserpreferences.h"
#include "servicepassword.h"
#include "user.h"

class LIBRUQOLACORE_EXPORT OwnUser
{
public:
    OwnUser();
    ~OwnUser();
    void parseOwnUserInfo(const QJsonObject &replyObject);

    [[nodiscard]] QString userId() const;
    void setUserId(const QString &userId);

    [[nodiscard]] QString userName() const;
    void setUserName(const QString &userName);

    [[nodiscard]] QString status() const;
    void setStatus(const QString &status);

    [[nodiscard]] bool operator==(const OwnUser &other) const;

    [[nodiscard]] QString email() const;
    void setEmail(const QString &email);

    [[nodiscard]] QString statusText() const;
    void setStatusText(const QString &statusText);

    [[nodiscard]] QString name() const;
    void setName(const QString &name);

    [[nodiscard]] QString avatarUrl() const;
    void setAvatarUrl(const QString &avatarUrl);

    [[nodiscard]] double utcOffset() const;
    void setUtcOffset(double utcOffset);

    [[nodiscard]] QString statusDefault() const;
    void setStatusDefault(const QString &statusDefault);

    [[nodiscard]] User user() const;

    [[nodiscard]] QString nickName() const;
    void setNickName(const QString &nickName);

    [[nodiscard]] QStringList roles() const;
    void setRoles(const QStringList &roles);

    [[nodiscard]] bool isAdministrator() const;

    [[nodiscard]] ServicePassword servicePassword() const;
    void setServicePassword(const ServicePassword &servicePassword);

    [[nodiscard]] OwnUserPreferences ownUserPreferences() const;
    void setOwnUserPreferences(const OwnUserPreferences &ownUserPreferences);

private:
    QString mUserId;
    QString mUserName;
    QString mStatus;
    QString mEmail;
    QString mStatusText;
    QString mName;
    QString mAvatarUrl;
    QString mStatusDefault;
    QString mNickName;
    QStringList mRoles;
    ServicePassword mServicePassword;
    OwnUserPreferences mOwnUserPreferences;
    double mUtcOffset = 0.0;
};
Q_DECLARE_METATYPE(OwnUser)
Q_DECLARE_TYPEINFO(OwnUser, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const OwnUser &t);
