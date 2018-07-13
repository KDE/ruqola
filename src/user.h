/*
   Copyright (c) 2017-2018 Montel Laurent <montel@kde.org>

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

#ifndef USER_H
#define USER_H

#include <QString>
#include <QDebug>
#include "libruqola_private_export.h"
class LIBRUQOLACORE_TESTS_EXPORT User
{
    Q_GADGET
public:

    enum class PresenceStatus {
        PresenceOnline,
        PresenceBusy,
        PresenceAway,
        PresenceOffline,
        Unknown
    };
    Q_ENUM(PresenceStatus)

    User();
    ~User();

    Q_REQUIRED_RESULT QString name() const;
    void setName(const QString &name);

    Q_REQUIRED_RESULT QString userId() const;
    void setUserId(const QString &userId);

    Q_REQUIRED_RESULT QString status() const;
    void setStatus(const QString &status);

    void parseUser(const QJsonObject &json);
    Q_REQUIRED_RESULT Q_INVOKABLE QString iconFromStatus() const;

    Q_REQUIRED_RESULT bool operator ==(const User &other) const;
    Q_REQUIRED_RESULT bool operator !=(const User &other) const;

    User &operator=(const User &other);
    Q_REQUIRED_RESULT QString userName() const;
    void setUserName(const QString &userName);

    Q_REQUIRED_RESULT bool isValid() const;

    Q_REQUIRED_RESULT int utcOffset() const;
    void setUtcOffset(int utcOffset);

private:
    QString mStatus = QStringLiteral("offline");
    QString mUserId;
    QString mName;
    QString mUserName;
    int mUtcOffset = 0;
};
Q_DECLARE_METATYPE(User)
Q_DECLARE_TYPEINFO(User, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator <<(QDebug d, const User &t);

#endif // USER_H
