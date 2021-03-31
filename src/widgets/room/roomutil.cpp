/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#include "roomutil.h"
#include "ruqolawidgets_debug.h"
QString RoomUtil::generateUserLink(const QString &userName)
{
    return QStringLiteral("ruqola:/user/") + userName;
}

QString RoomUtil::generatePermalink(const QString &messageId, const QString &roomId, Room::RoomType channelType)
{
    QString prefix;
    switch (channelType) {
    case Room::RoomType::Channel:
        prefix = QStringLiteral("channel/");
        break;
    case Room::RoomType::Direct:
        prefix = QStringLiteral("direct/");
        break;
    case Room::RoomType::Private:
        prefix = QStringLiteral("group/");
        break;
    case Room::RoomType::Unknown:
        qCWarning(RUQOLAWIDGETS_LOG) << " channel type undefined " << channelType;
        break;
    }
    const QString result = QStringLiteral("%1%2?msg=%3").arg(prefix, roomId, messageId);
    return result;
}
