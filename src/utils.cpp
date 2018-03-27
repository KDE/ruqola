/*
   Copyright (c) 2017-2018 Montel Laurent <montel@kde.org>

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

#include "utils.h"
#include "ruqola_debug.h"
#include <QJsonObject>
#include <QJsonArray>
#include <KTextToHTML>
#include <qregularexpression.h>

QUrl Utils::generateServerUrl(const QString &url)
{
    if (url.isEmpty()) {
        return {};
    }
    QString serverUrl = url;
    if (serverUrl.startsWith(QLatin1String("https://"))) {
        serverUrl.replace(QLatin1String("https://"), QLatin1String("wss://"));
    } else if (serverUrl.startsWith(QLatin1String("http://"))) {
        serverUrl.replace(QLatin1String("http://"), QLatin1String("ws://"));
    } else {
        serverUrl = QStringLiteral("wss://") + serverUrl;
    }
    return QUrl(serverUrl + QStringLiteral("/websocket"));
}

QString Utils::extractRoomUserFromUrl(QString url)
{
    url.remove(QStringLiteral("ruqola:/user/"));
    url.remove(QStringLiteral("ruqola:/room/"));
    return url;
}

QString Utils::markdownToRichText(const QString &markDown)
{
    //qCDebug(RUQOLA_LOG) << "BEFORE markdownToRichText "<<markDown;
    QString str = markDown;

    const KTextToHTML::Options convertFlags = KTextToHTML::PreserveSpaces | KTextToHTML::HighlightText | KTextToHTML::ReplaceSmileys;
    str = KTextToHTML::convertToHtml(str, convertFlags);
    //Bug 391520 I don't remember why I removed <br /> need to investigate
    //str.remove(QStringLiteral("<br />"));
    //qCDebug(RUQOLA_LOG) << "markdownToRichText "<<str;
    return str;
}

QString Utils::generateRichText(const QString &str, const QMap<QString, QString> &mentions)
{
    //Not using mentions for the moment.
    Q_UNUSED(mentions)
    QString newStr = Utils::markdownToRichText(str);
    static const QRegularExpression regularExpressionUser(QStringLiteral("(^|\\s+)@([\\w._-]+)"));
    QRegularExpressionMatchIterator userIterator = regularExpressionUser.globalMatch(newStr);
    while (userIterator.hasNext()) {
        const QRegularExpressionMatch match = userIterator.next();
        const QString word = match.captured(2);
        //Highlight only if it's yours
        //newStr.replace(QLatin1Char('@') + word, QStringLiteral("<a href=\'ruqola:/user/%1\' style=\"color:#FFFFFF;background-color:#0000FF;\">@%1</a>").arg(word));
        newStr.replace(QLatin1Char('@') + word, QStringLiteral("<a href=\'ruqola:/user/%1\'>@%1</a>").arg(word));
    }

    static const QRegularExpression regularExpressionRoom(QStringLiteral("(^|\\s+)#([\\w._-]+)"));
    QRegularExpressionMatchIterator roomIterator = regularExpressionRoom.globalMatch(newStr);
    while (roomIterator.hasNext()) {
        const QRegularExpressionMatch match = roomIterator.next();
        const QString word = match.captured(2);
        newStr.replace(QLatin1Char('#') + word, QStringLiteral("<a href=\'ruqola:/room/%1\'>#%1</a>").arg(word));
    }
    return newStr;
}

QString Utils::presenceStatusToString(User::PresenceStatus status)
{
    switch (status) {
    case User::PresenceStatus::PresenceOnline:
        return QStringLiteral("online");
    case User::PresenceStatus::PresenceBusy:
        return QStringLiteral("busy");
    case User::PresenceStatus::PresenceAway:
        return QStringLiteral("away");
    case User::PresenceStatus::PresenceOffline:
        return QStringLiteral("offline");
    case User::PresenceStatus::Unknown:
        return {};
    }
    Q_UNREACHABLE();
    return {};
}

User::PresenceStatus Utils::presenceStatusFromString(const QString &status)
{
    if (status == QStringLiteral("online")) {
        return User::PresenceStatus::PresenceOnline;
    } else if (status == QStringLiteral("busy")) {
        return User::PresenceStatus::PresenceBusy;
    } else if (status == QStringLiteral("away")) {
        return User::PresenceStatus::PresenceAway;
    } else if (status == QStringLiteral("offline")) {
        return User::PresenceStatus::PresenceOffline;
    } else {
        qCDebug(RUQOLA_LOG) << "Problem with status " << status;
        return User::PresenceStatus::Unknown;
    }
}

void Utils::parseNotification(const QJsonArray &contents, QString &message, QString &title, QString &sender)
{
    QJsonObject obj = contents.at(0).toObject();
    message = obj[QStringLiteral("text")].toString();
    title = obj[QStringLiteral("title")].toString();
    obj = obj.value(QLatin1String("payload")).toObject();
    if (!obj.isEmpty()) {
        obj = obj.value(QLatin1String("sender")).toObject();
        if (!obj.isEmpty()) {
            sender = obj.value(QLatin1String("_id")).toString();
        } else {
            qCDebug(RUQOLA_LOG) << "Problem with notication json: missing sender";
        }
    } else {
        qCDebug(RUQOLA_LOG) << "Problem with notication json: missing payload";
    }
}

QString Utils::userIdFromDirectChannel(const QString &rid, const QString &userId)
{
    QString newUserId = rid;
    newUserId.remove(userId);
    return newUserId;
}

qint64 Utils::parseDate(const QString &key, const QJsonObject &o)
{
    return o.value(key).toObject().value(QLatin1String("$date")).toDouble(-1);
}
