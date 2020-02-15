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

#ifndef ACCOUNTROOMSETTINGS_H
#define ACCOUNTROOMSETTINGS_H

#include <QMap>
#include <QString>
#include "libruqolacore_export.h"

class LIBRUQOLACORE_EXPORT AccountRoomSettings
{
public:
    struct LIBRUQOLACORE_EXPORT PendingTypedInfo {
        QString text;
        QString messageIdBeingEdited;
        int scrollbarPosition = -1;
        Q_REQUIRED_RESULT bool isValid() const
        {
            return !text.isEmpty() || !messageIdBeingEdited.isEmpty() || (scrollbarPosition != -1);
        }
    };
    AccountRoomSettings();
    ~AccountRoomSettings();

    void remove(const QString &roomId);

    void add(const QString &roomId, const PendingTypedInfo &info);

    Q_REQUIRED_RESULT PendingTypedInfo value(const QString &roomId);

    Q_REQUIRED_RESULT bool isEmpty() const;

private:

    QMap<QString /*RoomId*/, PendingTypedInfo> mPendingTypedTexts;
};

#endif // ACCOUNTROOMSETTINGS_H
