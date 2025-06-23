/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomutil.h"
using namespace Qt::Literals::StringLiterals;

#include "ruqolawidgets_debug.h"
QString RoomUtil::generateUserLink(const QString &userName)
{
    return u"ruqola:/user/"_s + userName;
}

QString RoomUtil::generatePermalink(const QString &messageId, const QString &roomId, Room::RoomType channelType)
{
    QString prefix;
    switch (channelType) {
    case Room::RoomType::Channel:
        prefix = u"channel/"_s;
        break;
    case Room::RoomType::Direct:
        prefix = u"direct/"_s;
        break;
    case Room::RoomType::Private:
        prefix = u"group/"_s;
        break;
    case Room::RoomType::Unknown:
        qCWarning(RUQOLAWIDGETS_LOG) << " channel type undefined " << channelType;
        break;
    }
    const QString result = u"%1%2?msg=%3"_s.arg(prefix, roomId, messageId);
    return result;
}
