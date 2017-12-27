/*
   Copyright (c) 2017 Montel Laurent <montel@kde.org>

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

#include "restapiutil.h"

QString RestApiUtil::adaptUrl(const QString &url)
{
    if (url.isEmpty()) {
        return url;
    }
    //Avoid to add more https:// :)
    if (url.startsWith(QLatin1String("https://")) || url.startsWith(QLatin1String("http://"))) {
        return url;
    } else {
        //Default to https
        return QStringLiteral("https://") + url;
    }
}

QString RestApiUtil::apiUri()
{
    return QStringLiteral("/api/v1");
}

QString RestApiUtil::restUrl(RestApiUtil::RestApiUrlType type)
{
    switch (type) {
    case RestApiUtil::RestApiUrlType::Login:
        return QStringLiteral("login");
    case RestApiUtil::RestApiUrlType::Logout:
        return QStringLiteral("logout");
    case RestApiUtil::RestApiUrlType::Me:
        return QStringLiteral("me");
    case RestApiUtil::RestApiUrlType::GetAvatar:
        return QStringLiteral("users.getAvatar");
    }
#if 0
    //Channel
    return QStringLiteral("channels.addAll");
    return QStringLiteral("channels.addModerator");
    return QStringLiteral("channels.addOwner");
    return QStringLiteral("channels.archive");
    return QStringLiteral("channels.cleanHistory");
    return QStringLiteral("channels.close");
    return QStringLiteral("channels.create");
    return QStringLiteral("channels.getIntegrations");
    return QStringLiteral("channels.history");
    return QStringLiteral("channels.info");
    return QStringLiteral("channels.invite");
    return QStringLiteral("channels.kick");
    return QStringLiteral("channels.leave");
    return QStringLiteral("channels.list");
    return QStringLiteral("channels.list.joined");
    return QStringLiteral("channels.open");
    return QStringLiteral("channels.removeModerator");
    return QStringLiteral("channels.removeOwner");
    return QStringLiteral("channels.rename");
    return QStringLiteral("channels.setDescription");
    return QStringLiteral("channels.setJoinCode");
    return QStringLiteral("channels.setPurpose");
    return QStringLiteral("channels.setReadOnly");
    return QStringLiteral("channels.setTopic");
    return QStringLiteral("channels.setType");
    return QStringLiteral("channels.unarchive");
    //Chat
    return QStringLiteral("chat.delete");
    return QStringLiteral("chat.getMessage");
    return QStringLiteral("chat.pinMessage");
    return QStringLiteral("chat.postMessage");
    return QStringLiteral("chat.react");
    return QStringLiteral("chat.starMessage");
    return QStringLiteral("chat.unPinMessage");
    return QStringLiteral("chat.unStarMessage");
    return QStringLiteral("chat.update");
    //Group
    return QStringLiteral("groups.addAll");
    return QStringLiteral("groups.addModerator");
    return QStringLiteral("groups.addOwner");
    return QStringLiteral("groups.archive");
    return QStringLiteral("groups.close");
    return QStringLiteral("groups.create");
    return QStringLiteral("groups.getIntegrations");
    return QStringLiteral("groups.history");
    return QStringLiteral("groups.info");
    return QStringLiteral("groups.invite");
    return QStringLiteral("groups.kick");
    return QStringLiteral("groups.leave");
    return QStringLiteral("groups.list");
    return QStringLiteral("groups.open");
    return QStringLiteral("groups.removeModerator");
    return QStringLiteral("groups.removeOwner");
    return QStringLiteral("groups.rename");
    return QStringLiteral("groups.setDescription");
    return QStringLiteral("groups.setPurpose");
    return QStringLiteral("groups.setReadOnly");
    return QStringLiteral("groups.setTopic");
    return QStringLiteral("groups.setType");
    return QStringLiteral("groups.unarchive");
    //Users
    return QStringLiteral("users.create");
    return QStringLiteral("users.createToken");

    return QStringLiteral("users.getPresence");
    return QStringLiteral("users.info");
    return QStringLiteral("users.list");
    return QStringLiteral("users.register");
    return QStringLiteral("users.resetAvatar");
    return QStringLiteral("users.setAvatar");
    return QStringLiteral("users.update");
#endif
    return {};
}
