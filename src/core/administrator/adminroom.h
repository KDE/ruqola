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


#ifndef ROOMADMIN_H
#define ROOMADMIN_H

#include "libruqolacore_export.h"
#include <QString>
#include <QDebug>
#include <QJsonObject>

class LIBRUQOLACORE_EXPORT AdminRoom
{
public:
    AdminRoom();
    ~AdminRoom();
    Q_REQUIRED_RESULT bool operator ==(const AdminRoom &other) const;
    void parseAdminRoom(const QJsonObject &object);

    Q_REQUIRED_RESULT bool defaultRoom() const;
    void setDefaultRoom(bool defaultRoom);

    Q_REQUIRED_RESULT int usersCount() const;
    void setUsersCount(int usersCount);

    Q_REQUIRED_RESULT int messageCount() const;
    void setMessageCount(int messageCount);

    Q_REQUIRED_RESULT QString channelType() const;
    void setChannelType(const QString &channelType);

    Q_REQUIRED_RESULT QString identifier() const;
    void setIdentifier(const QString &identifier);

    Q_REQUIRED_RESULT QString topic() const;
    void setTopic(const QString &topic);

    Q_REQUIRED_RESULT bool readOnly() const;
    void setReadOnly(bool readOnly);

private:
    QString mTopic;
    QString mIdentifier;
    QString mChannelType;
    int mMessageCount = -1;
    int mUsersCount = -1;
    bool mDefaultRoom = false;
    bool mReadOnly = false;
};
Q_DECLARE_METATYPE(AdminRoom)
Q_DECLARE_TYPEINFO(AdminRoom, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator <<(QDebug d, const AdminRoom &t);

#endif // ROOMADMIN_H
