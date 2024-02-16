/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
