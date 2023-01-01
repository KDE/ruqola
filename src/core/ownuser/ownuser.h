/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "ownuserpreferences.h"
#include "servicepassword.h"
#include "user.h"

#include <QJsonObject>
class LIBRUQOLACORE_EXPORT OwnUser
{
public:
    OwnUser();
    ~OwnUser();
    void parseOwnUserInfo(const QJsonObject &replyObject);

    Q_REQUIRED_RESULT QString userId() const;
    void setUserId(const QString &userId);

    Q_REQUIRED_RESULT QString userName() const;
    void setUserName(const QString &userName);

    Q_REQUIRED_RESULT QString status() const;
    void setStatus(const QString &status);

    Q_REQUIRED_RESULT bool operator==(const OwnUser &other) const;

    Q_REQUIRED_RESULT QString email() const;
    void setEmail(const QString &email);

    Q_REQUIRED_RESULT QString statusText() const;
    void setStatusText(const QString &statusText);

    Q_REQUIRED_RESULT QString name() const;
    void setName(const QString &name);

    Q_REQUIRED_RESULT QString avatarUrl() const;
    void setAvatarUrl(const QString &avatarUrl);

    Q_REQUIRED_RESULT double utcOffset() const;
    void setUtcOffset(double utcOffset);

    Q_REQUIRED_RESULT QString statusDefault() const;
    void setStatusDefault(const QString &statusDefault);

    Q_REQUIRED_RESULT User user() const;

    Q_REQUIRED_RESULT QString nickName() const;
    void setNickName(const QString &nickName);

    Q_REQUIRED_RESULT QStringList roles() const;
    void setRoles(const QStringList &roles);

    Q_REQUIRED_RESULT bool isAdministrator() const;

    Q_REQUIRED_RESULT ServicePassword servicePassword() const;
    void setServicePassword(const ServicePassword &servicePassword);

    Q_REQUIRED_RESULT OwnUserPreferences ownUserPreferences() const;
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
Q_DECLARE_TYPEINFO(OwnUser, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const OwnUser &t);
