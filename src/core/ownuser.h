/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#ifndef OWNUSER_H
#define OWNUSER_H

#include "libruqolacore_export.h"
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

    Q_REQUIRED_RESULT bool operator ==(const OwnUser &other) const;

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

private:
    //TODO add service password support
    QString mUserId;
    QString mUserName;
    QString mStatus;
    QString mEmail;
    QString mStatusText;
    QString mName;
    QString mAvatarUrl;
    QString mStatusDefault;
    QString mNickName;
    double mUtcOffset = 0.0;
};
Q_DECLARE_METATYPE(OwnUser)
Q_DECLARE_TYPEINFO(OwnUser, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator <<(QDebug d, const OwnUser &t);
#endif // OWNUSER_H
