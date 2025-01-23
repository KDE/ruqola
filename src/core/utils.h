/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqola_private_export.h"
#include "user.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QUrl>

class RocketChatAccount;

namespace Utils
{
struct LIBRUQOLACORE_EXPORT QuotedRichTextInfo {
    QString richText;
    QString url;
    QString displayTime;
};

[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT QUrl generateServerUrl(const QString &url);
[[nodiscard]] LIBRUQOLACORE_EXPORT QString presenceStatusToString(User::PresenceStatus status);
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT User::PresenceStatus presenceStatusFromString(const QString &status);
[[nodiscard]] LIBRUQOLACORE_EXPORT QString i18nFromPresenceStatus(User::PresenceStatus status);
[[nodiscard]] LIBRUQOLACORE_EXPORT QString formatQuotedRichText(const QuotedRichTextInfo &info);
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT QString extractRoomUserFromUrl(QString url);
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT QString userIdFromDirectChannel(const QString &rid, const QString &userId);
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT qint64 parseDate(const QString &key, const QJsonObject &o);
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT qint64 parseIsoDate(const QString &key, const QJsonObject &o);
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT QString iconFromStatus(const QString &status);
[[nodiscard]] LIBRUQOLACORE_EXPORT QString iconFromPresenceStatus(User::PresenceStatus status);
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT QJsonObject strToJsonObject(const QString &jsonString);
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT QJsonArray strToJsonArray(const QString &jsonString);
[[nodiscard]] LIBRUQOLACORE_EXPORT QByteArray convertSha256Password(const QString &pwd);
[[nodiscard]] LIBRUQOLACORE_EXPORT QString emojiFontName();
[[nodiscard]] LIBRUQOLACORE_EXPORT QString displaytextFromPresenceStatus(User::PresenceStatus status);
[[nodiscard]] LIBRUQOLACORE_EXPORT QString convertTextWithCheckMark(const QString &str);
[[nodiscard]] LIBRUQOLACORE_EXPORT bool validUser(const QString &userName);
[[nodiscard]] LIBRUQOLACORE_EXPORT QString createUniqueAccountName(const QStringList &list, const QString &accountName);
[[nodiscard]] LIBRUQOLACORE_EXPORT QString findExecutable(const QString &exec);
[[nodiscard]] LIBRUQOLACORE_EXPORT bool executableFound(const QString &exec);
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT bool userActivity(const QJsonArray &contents);

/**
 * @brief Convert []() style Markdown URLS with proper HTML tags
 *
 * Also supports strings that already contain HTML links.
 *
 * Examples:
 *   "[NAME](<a href=\"LINK\">...</a>)" => "<a href="LINK">NAME</a>"
 *   "[NAME](LINK)"                   => "<a href="LINK">NAME</a>"
 */
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT QString convertTextWithUrl(const QString &str);

enum class AvatarType : uint8_t {
    Unknown,
    Room,
    User,
    UserAndRoom,
};

struct LIBRUQOLACORE_EXPORT AvatarInfo {
    [[nodiscard]] bool isValid() const
    {
        return (avatarType != AvatarType::Unknown) && !identifier.isEmpty();
    }

    [[nodiscard]] bool operator==(const AvatarInfo &other) const
    {
        return etag == other.etag && identifier == other.identifier && avatarType == other.avatarType;
    }

    [[nodiscard]] QString generateAvatarIdentifier() const;
    QString etag;
    QString identifier;
    AvatarType avatarType = AvatarType::Unknown;
};

[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT QUrl avatarUrl(const QString &url, const AvatarInfo &avatarInfo);
[[nodiscard]] LIBRUQOLACORE_EXPORT QIcon iconFromAccount(RocketChatAccount *account);
}
Q_DECLARE_METATYPE(Utils::AvatarInfo)
Q_DECLARE_TYPEINFO(Utils::AvatarInfo, Q_RELOCATABLE_TYPE);
Q_DECLARE_TYPEINFO(Utils::QuotedRichTextInfo, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Utils::AvatarInfo &t);
