/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
struct LIBRUQOLACORE_EXPORT QuotedRichTextInfo {
    QString richText;
    QString url;
    QString displayTime;
};

Q_REQUIRED_RESULT LIBRUQOLACORE_TESTS_EXPORT QUrl generateServerUrl(const QString &url);
Q_REQUIRED_RESULT LIBRUQOLACORE_EXPORT QString presenceStatusToString(User::PresenceStatus status);
Q_REQUIRED_RESULT LIBRUQOLACORE_TESTS_EXPORT User::PresenceStatus presenceStatusFromString(const QString &status);
Q_REQUIRED_RESULT LIBRUQOLACORE_EXPORT QString formatQuotedRichText(const QuotedRichTextInfo &info);
Q_REQUIRED_RESULT LIBRUQOLACORE_TESTS_EXPORT QString extractRoomUserFromUrl(QString url);
Q_REQUIRED_RESULT LIBRUQOLACORE_TESTS_EXPORT QString userIdFromDirectChannel(const QString &rid, const QString &userId);
Q_REQUIRED_RESULT LIBRUQOLACORE_TESTS_EXPORT qint64 parseDate(const QString &key, const QJsonObject &o);
Q_REQUIRED_RESULT LIBRUQOLACORE_TESTS_EXPORT qint64 parseIsoDate(const QString &key, const QJsonObject &o);
Q_REQUIRED_RESULT LIBRUQOLACORE_TESTS_EXPORT QString iconFromStatus(const QString &status);
Q_REQUIRED_RESULT LIBRUQOLACORE_EXPORT QString iconFromPresenceStatus(User::PresenceStatus status);
Q_REQUIRED_RESULT LIBRUQOLACORE_TESTS_EXPORT QJsonObject strToJsonObject(const QString &jsonString);
Q_REQUIRED_RESULT LIBRUQOLACORE_TESTS_EXPORT QJsonArray strToJsonArray(const QString &jsonString);
Q_REQUIRED_RESULT LIBRUQOLACORE_EXPORT QByteArray convertSha256Password(const QString &pwd);
Q_REQUIRED_RESULT LIBRUQOLACORE_EXPORT QString emojiFontName();
Q_REQUIRED_RESULT LIBRUQOLACORE_EXPORT QString displaytextFromPresenceStatus(User::PresenceStatus status);

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

struct LIBRUQOLACORE_EXPORT AvatarInfo {
    Q_REQUIRED_RESULT bool isValid() const
    {
        return (avatarType != AvatarType::Unknown) && !identifier.isEmpty();
    }

    Q_REQUIRED_RESULT bool operator==(const AvatarInfo &other) const
    {
        return etag == other.etag && identifier == other.identifier && avatarType == other.avatarType;
    }

    Q_REQUIRED_RESULT QString generateAvatarIdentifier() const;
    QString etag;
    QString identifier;
    AvatarType avatarType = AvatarType::Unknown;
};

Q_REQUIRED_RESULT LIBRUQOLACORE_TESTS_EXPORT QUrl avatarUrl(const QString &url, const AvatarInfo &avatarInfo);
}
Q_DECLARE_METATYPE(Utils::AvatarInfo)
Q_DECLARE_TYPEINFO(Utils::AvatarInfo, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Utils::AvatarInfo &t);
