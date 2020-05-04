/*
   Copyright (c) 2017-2020 Laurent Montel <montel@kde.org>

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
#ifndef UTILS_H
#define UTILS_H

#include "libruqola_private_export.h"
#include "user.h"
#include <QUrl>

namespace Utils {
struct NotificationInfo {
    QString message;
    QString title;
    QString sender;
    QString roomId;
    QString type;
};

Q_REQUIRED_RESULT LIBRUQOLACORE_TESTS_EXPORT QUrl generateServerUrl(const QString &url);
Q_REQUIRED_RESULT LIBRUQOLACORE_TESTS_EXPORT QString markdownToRichText(const QString &markDown);
Q_REQUIRED_RESULT LIBRUQOLACORE_TESTS_EXPORT QString presenceStatusToString(User::PresenceStatus status);
Q_REQUIRED_RESULT LIBRUQOLACORE_TESTS_EXPORT User::PresenceStatus presenceStatusFromString(const QString &status);
Q_REQUIRED_RESULT LIBRUQOLACORE_TESTS_EXPORT QString generateRichText(const QString &markDown, const QString &username);
Q_REQUIRED_RESULT LIBRUQOLACORE_EXPORT QString formatQuotedRichText(const QString &richText);
Q_REQUIRED_RESULT LIBRUQOLACORE_TESTS_EXPORT QString extractRoomUserFromUrl(QString url);
LIBRUQOLACORE_TESTS_EXPORT NotificationInfo parseNotification(const QJsonArray &contents);
Q_REQUIRED_RESULT LIBRUQOLACORE_TESTS_EXPORT QString userIdFromDirectChannel(const QString &rid, const QString &userId);
Q_REQUIRED_RESULT LIBRUQOLACORE_TESTS_EXPORT qint64 parseDate(const QString &key, const QJsonObject &o);
Q_REQUIRED_RESULT LIBRUQOLACORE_TESTS_EXPORT qint64 parseIsoDate(const QString &key, const QJsonObject &o);
Q_REQUIRED_RESULT LIBRUQOLACORE_TESTS_EXPORT QString iconFromStatus(const QString &status);

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
}

#endif // UTILS_H
