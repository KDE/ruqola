/*
   Copyright (c) 2017-2021 Laurent Montel <montel@kde.org>

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
#pragma once

#include "libruqola_private_export.h"
#include "user.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QPixmap>
#include <QUrl>

namespace Utils
{
struct NotificationInfo {
    QString message;
    QString title;
    QString senderId;
    QString senderName;
    QString senderUserName;
    QString roomName;
    QString channelType;
    QPixmap pixmap;
    // Pixmap can be null.
    Q_REQUIRED_RESULT bool isValid() const
    {
        return !senderId.isEmpty() && !message.isEmpty() && !channelType.isEmpty();
    }
};

Q_REQUIRED_RESULT LIBRUQOLACORE_TESTS_EXPORT QUrl generateServerUrl(const QString &url);
Q_REQUIRED_RESULT LIBRUQOLACORE_EXPORT QString presenceStatusToString(User::PresenceStatus status);
Q_REQUIRED_RESULT LIBRUQOLACORE_TESTS_EXPORT User::PresenceStatus presenceStatusFromString(const QString &status);
Q_REQUIRED_RESULT LIBRUQOLACORE_EXPORT QString formatQuotedRichText(const QString &richText);
Q_REQUIRED_RESULT LIBRUQOLACORE_TESTS_EXPORT QString extractRoomUserFromUrl(QString url);
LIBRUQOLACORE_TESTS_EXPORT NotificationInfo parseNotification(const QJsonArray &contents);
Q_REQUIRED_RESULT LIBRUQOLACORE_TESTS_EXPORT QString userIdFromDirectChannel(const QString &rid, const QString &userId);
Q_REQUIRED_RESULT LIBRUQOLACORE_TESTS_EXPORT qint64 parseDate(const QString &key, const QJsonObject &o);
Q_REQUIRED_RESULT LIBRUQOLACORE_TESTS_EXPORT qint64 parseIsoDate(const QString &key, const QJsonObject &o);
Q_REQUIRED_RESULT LIBRUQOLACORE_TESTS_EXPORT QString iconFromStatus(const QString &status);
Q_REQUIRED_RESULT LIBRUQOLACORE_TESTS_EXPORT QJsonObject strToJsonObject(const QString &jsonString);
Q_REQUIRED_RESULT LIBRUQOLACORE_TESTS_EXPORT QJsonArray strToJsonArray(const QString &jsonString);
Q_REQUIRED_RESULT LIBRUQOLACORE_EXPORT QByteArray convertSha256Password(const QString &pwd);

/**
 * @brief Convert []() style Markdown URLS with proper HTML tags
 *
 * Also supports strings that already contain HTML links.
 *
 * Examples:
 *   "[NAME](<a href=\"LINK\">...</a>)" => "<a href="LINK">NAME</a>"
 *   "[NAME](LINK)"                   => "<a href="LINK">NAME</a>"
 */
Q_REQUIRED_RESULT LIBRUQOLACORE_TESTS_EXPORT QString convertTextWithUrl(const QString &str);

enum class AvatarType {
    Unknown,
    Room,
    User,
};

struct AvatarInfo {
    Q_REQUIRED_RESULT bool isValid() const
    {
        return (avatarType != AvatarType::Unknown) && !identifier.isEmpty();
    }

    Q_REQUIRED_RESULT bool operator==(const AvatarInfo &other) const
    {
        return etag == other.etag && identifier == other.identifier && avatarType == other.avatarType;
    }

    QString etag;
    QString identifier;
    AvatarType avatarType = AvatarType::Unknown;
};

Q_REQUIRED_RESULT LIBRUQOLACORE_TESTS_EXPORT QUrl avatarUrl(const QString &url, const AvatarInfo &avatarInfo);
}
Q_DECLARE_TYPEINFO(Utils::NotificationInfo, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(Utils::AvatarInfo)
Q_DECLARE_TYPEINFO(Utils::AvatarInfo, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Utils::NotificationInfo &t);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Utils::AvatarInfo &t);
