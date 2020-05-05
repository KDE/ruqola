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

#include "utils.h"
#include "ruqola_debug.h"
#include <QJsonObject>
#include <QDateTime>
#include <QJsonArray>
#include <KTextToHTML>
#include <QRegularExpression>
#include <KColorScheme>

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
        serverUrl = QLatin1String("wss://") + serverUrl;
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
    if (markDown.isEmpty()) {
        return QString();
    }

    //qCDebug(RUQOLA_LOG) << "BEFORE markdownToRichText "<<markDown;
    QString str = markDown;

    //TODO remove replaceSmileys when we will use unicode emoticons
    const KTextToHTML::Options convertFlags = KTextToHTML::PreserveSpaces | KTextToHTML::HighlightText | KTextToHTML::ReplaceSmileys | KTextToHTML::ConvertPhoneNumbers;
    str = KTextToHTML::convertToHtml(str, convertFlags);

    // substitute "[example.com](<a href="...">...</a>)" style urls
    str = Utils::convertTextWithUrl(str);

    //Bug 391520 I don't remember why I removed <br /> need to investigate
    //str.remove(QStringLiteral("<br />"));
    //qCDebug(RUQOLA_LOG) << "markdownToRichText "<<str;
    return str;
}

QString Utils::generateRichText(const QString &str, const QString &username)
{
    QString newStr = Utils::markdownToRichText(str);
    static const QRegularExpression regularExpressionUser(QStringLiteral("(^|\\s+)@([\\w._-]+)"));
    QRegularExpressionMatchIterator userIterator = regularExpressionUser.globalMatch(newStr);

    KColorScheme colorScheme;
    const auto userMentionForegroundColor = colorScheme.foreground(KColorScheme::ActiveText).color().name();
    const auto userMentionBackgroundColor = colorScheme.background(KColorScheme::ActiveBackground).color().name();
    while (userIterator.hasNext()) {
        const QRegularExpressionMatch match = userIterator.next();
        const QStringRef word = match.capturedRef(2);
        //Highlight only if it's yours
        if (word == username) {
            newStr.replace(QLatin1Char('@') + word,
                           QStringLiteral("<a href=\'ruqola:/user/%1\' style=\"color:%2;background-color:%3;\">@%1</a>")
                           .arg(word.toString(), userMentionForegroundColor, userMentionBackgroundColor));
        } else {
            newStr.replace(QLatin1Char('@') + word, QStringLiteral("<a href=\'ruqola:/user/%1\'>@%1</a>").arg(word));
        }
    }

    static const QRegularExpression regularExpressionRoom(QStringLiteral("(^|\\s+)#([\\w._-]+)"));
    QRegularExpressionMatchIterator roomIterator = regularExpressionRoom.globalMatch(newStr);
    while (roomIterator.hasNext()) {
        const QRegularExpressionMatch match = roomIterator.next();
        const QStringRef word = match.capturedRef(2);
        newStr.replace(QLatin1Char('#') + word, QStringLiteral("<a href=\'ruqola:/room/%1\'>#%1</a>").arg(word));
    }

    /// match unescaped `...` regions, i.e. properly match `...\`...`
    /// and make the inner region non-greedy, to have two code blocks for
    /// lines like this: `foo` asdf `bar`
    static const QRegularExpression regularExpressionCode(QStringLiteral("((?<!\\\\)`.*?(?<!\\\\)`)"));
    newStr.replace(regularExpressionCode, QStringLiteral("<code>\\1</code>"));

    return newStr;
}

QString Utils::formatQuotedRichText(const QString &richText)
{
    // Qt's support for borders is limited to tables, so we have to jump through some hoops...
    KColorScheme scheme;
    const auto backgroundColor = scheme.background(KColorScheme::AlternateBackground).color().name();
    const auto borderColor = scheme.foreground(KColorScheme::LinkText).color().name();
    return QStringLiteral("<table><tr><td style='background-color:%1; padding-left: 5px; border-left: 5px solid %2'>").arg(backgroundColor, borderColor)
           + richText
           + QStringLiteral("</td></tr></table>");
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

    return {};
}

QString Utils::iconFromStatus(const QString &status)
{
    if (status == QLatin1String("online")) {
        return QStringLiteral("user-online");
    } else if (status == QLatin1String("busy")) {
        return QStringLiteral("user-busy");
    } else if (status == QLatin1String("away")) {
        return QStringLiteral("user-away");
    } else if (status == QLatin1String("offline")) {
        return QStringLiteral("user-offline");
    } else {
        qCDebug(RUQOLA_LOG) << "Problem with status " << status;
        return QStringLiteral("unknown");
    }

    qCDebug(RUQOLA_LOG) << " QString User::iconFromStatus() const" << status;
    return {};
}

User::PresenceStatus Utils::presenceStatusFromString(const QString &status)
{
    if (status == QLatin1String("online")) {
        return User::PresenceStatus::PresenceOnline;
    } else if (status == QLatin1String("busy")) {
        return User::PresenceStatus::PresenceBusy;
    } else if (status == QLatin1String("away")) {
        return User::PresenceStatus::PresenceAway;
    } else if (status == QLatin1String("offline")) {
        return User::PresenceStatus::PresenceOffline;
    } else {
        qCDebug(RUQOLA_LOG) << "Problem with status " << status;
        return User::PresenceStatus::Unknown;
    }
}

Utils::NotificationInfo Utils::parseNotification(const QJsonArray &contents)
{
    Utils::NotificationInfo info;
    QJsonObject obj = contents.at(0).toObject();
    info.message = obj[QStringLiteral("text")].toString();
    info.title = obj[QStringLiteral("title")].toString();
    obj = obj.value(QLatin1String("payload")).toObject();
    if (!obj.isEmpty()) {
        obj = obj.value(QLatin1String("sender")).toObject();
        if (!obj.isEmpty()) {
            info.sender = obj.value(QLatin1String("_id")).toString();
        } else {
            qCDebug(RUQOLA_LOG) << "Problem with notification json: missing sender";
        }
        info.roomName = obj.value(QLatin1String("name")).toString();
        info.type = obj.value(QLatin1String("type")).toString();
    } else {
        qCDebug(RUQOLA_LOG) << "Problem with notification json: missing payload";
    }
    qCDebug(RUQOLA_LOG) << "info " << info;
    return info;
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

qint64 Utils::parseIsoDate(const QString &key, const QJsonObject &o)
{
    if (o.contains(key)) {
        return QDateTime::fromString(o.value(key).toString(), Qt::ISODate).toMSecsSinceEpoch();
    } else {
        return -1;
    }
}

QString Utils::convertTextWithUrl(const QString &str)
{
    static const QRegularExpression regularExpressionAHref(QStringLiteral("<a href=\"(.*)\">(.*)</a>"));

    QString newStr;
    bool isRef = false;
    bool isUrl = false;
//    bool isHasNewRef = false;
    QString url;
    QString references;
    for (int i = 0; i < str.count(); ++i) {
        const QChar ref = str.at(i);
        if (ref == QLatin1Char('[')) {
            if (isRef) {
                isRef = false;
                newStr += QLatin1Char('[') + references + QLatin1Char('[');
                references.clear();
            } else {
                isRef = true;
            }
        } else if (isRef && ref == QLatin1Char(']')) {
            isRef = false;
            if ((i == str.count() - 1) || (str.at(i + 1) != QLatin1Char('('))) {
                newStr += QLatin1Char('[') + references + QLatin1Char(']');
                references.clear();
            }
//        } else if (ref == QLatin1Char('|')) {
//            isUrl = false;
//            isRef = true;
//            isHasNewRef = true;
//        } else if (ref == QLatin1Char('<')) {
//            isUrl = true;
//        } else if (ref == QLatin1Char('>') && isHasNewRef) {
//            isUrl = false;
//            isRef = false;
//            isHasNewRef = false;
//            if (url.startsWith(QLatin1Char('<'))) {
//                newStr += url.replace(regularExpressionAHref, QStringLiteral("<a href=\"\\1\">%1</a>").arg(references));
//            } else {
//                newStr += QStringLiteral("<a href=\'%1'>%2</a>").arg(url, references);
//            }
//            references.clear();
//            url.clear();
        } else if (ref == QLatin1Char('(') && !references.isEmpty()) {
            isUrl = true;
        } else if (isUrl && ref == QLatin1Char(')') && !references.isEmpty()) {
            isUrl = false;
            // detect whether the string already contains HTML <a/> tags
            if (url.startsWith(QLatin1Char('<'))) {
                newStr += url.replace(regularExpressionAHref, QStringLiteral("<a href=\"\\1\">%1</a>").arg(references));
            } else {
                newStr += QStringLiteral("<a href=\'%1'>%2</a>").arg(url, references);
            }
            references.clear();
            url.clear();
        } else {
            if (isRef) {
                references += ref;
            } else if (isUrl) {
                url += ref;
            } else {
                newStr += ref;
            }
        }
    }
    if (isRef) {
        newStr += QLatin1Char('[') + references;
    } else if (isUrl) {
        newStr += QLatin1Char('[') + references + QLatin1String("](") + url;
    }
    return newStr;
}

QDebug operator <<(QDebug d, const Utils::NotificationInfo &t)
{
    d << " message " << t.message;
    d << " title " << t.title;
    d << " sender " << t.sender;
    d << " roomId " << t.roomName;
    d << " type " << t.type;
    d << " pixmap is null ? " << t.pixmap.isNull();
    return d;
}
