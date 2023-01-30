/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "utils.h"
#include "colors.h"
#include "ruqola_debug.h"
#include <KLocalizedString>

#include <KColorScheme>
#include <QCryptographicHash>
#include <QDateTime>
#include <QJsonDocument>
#include <QRegularExpression>

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

QString Utils::formatQuotedRichText(const QuotedRichTextInfo &info)
{
    // Qt's support for borders is limited to tables, so we have to jump through some hoops...
    const auto backgroundColor = Colors::self().schemeView().background(KColorScheme::AlternateBackground).color().name();
    const auto borderColor = Colors::self().schemeView().foreground(KColorScheme::LinkText).color().name();
    QString dateTimeInfo;
    if (!info.displayTime.isEmpty()) {
        dateTimeInfo = QLatin1Char('\n') + info.displayTime;
    }
    return QStringLiteral("<table><tr><td style='background-color:%1; padding-left: 5px; border-left: 5px solid %2'>").arg(backgroundColor, borderColor)
        + info.richText + dateTimeInfo + QStringLiteral("</td></tr></table>");
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

QString Utils::iconFromPresenceStatus(User::PresenceStatus status)
{
    switch (status) {
    case User::PresenceStatus::PresenceOnline:
        return QStringLiteral("user-online");
    case User::PresenceStatus::PresenceBusy:
        return QStringLiteral("user-busy");
    case User::PresenceStatus::PresenceAway:
        return QStringLiteral("user-away");
    case User::PresenceStatus::PresenceOffline:
        return QStringLiteral("user-offline");
    case User::PresenceStatus::Unknown:
        return QStringLiteral("unknown");
    }
    qCWarning(RUQOLA_LOG) << "Unknown status" << status;
    return QStringLiteral("unknown");
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
        qCWarning(RUQOLA_LOG) << "Unknown status" << status;
        return QStringLiteral("unknown");
    }
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
    static const QRegularExpression regularExpressionCustomAHref(QStringLiteral("<a href=\"(.*)\\|(.*)\">(.*)</a>"));
    QString newStr;
    bool isRef = false;
    bool isUrl = false;
    bool isHasNewRef = false;
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
#if 0
        } else if (isUrl && ref == QLatin1Char(']') && isHasNewRef) {
            isUrl = false;
            isRef = false;
            newStr += QStringLiteral("<a href=\'%1'>%2</a>").arg(url, references);
            references.clear();
            url.clear();
#endif
        } else if (isRef && ref == QLatin1Char(']')) {
            isRef = false;
            if ((i == str.count() - 1) || (str.at(i + 1) != QLatin1Char('('))) {
                if (references.startsWith(QLatin1Char('<'))) {
                    newStr += references.replace(regularExpressionCustomAHref, QStringLiteral("<a href=\"\\2\">\\1</a>"));
                } else {
                    newStr += QLatin1Char('[') + references + QLatin1Char(']');
                }
                references.clear();
            }
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
#if 0
        } else if (ref == QLatin1Char('|') && !references.isEmpty()) {
            isUrl = true;
            isRef = false;
            isHasNewRef = true;
#endif
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
    } else if (isHasNewRef) {
        if (!url.isEmpty() && !references.isEmpty()) {
            newStr += QStringLiteral("<a href=\'%1'>%2</a>").arg(url, references);
        }
    }
    return newStr;
}

QJsonObject Utils::strToJsonObject(const QString &jsonString)
{
    QJsonParseError jsonParseError;
    const auto doc = QJsonDocument::fromJson(jsonString.toLatin1(), &jsonParseError);

    if (jsonParseError.error != QJsonParseError::NoError) {
        qCWarning(RUQOLA_LOG).nospace() << Q_FUNC_INFO << " Couldn't parse a valid JSON from argument: " << jsonString
                                        << "\n JSON parse error: " << jsonParseError.errorString();
        return {};
    }

    if (!doc.isObject()) {
        qCWarning(RUQOLA_LOG) << Q_FUNC_INFO << "The JSON string argument is not a JSON object." << jsonString;
        return {};
    }

    return doc.object();
}

QJsonArray Utils::strToJsonArray(const QString &jsonString)
{
    QJsonParseError jsonParseError;
    const auto doc = QJsonDocument::fromJson(jsonString.toLatin1(), &jsonParseError);

    if (jsonParseError.error != QJsonParseError::NoError) {
        qCWarning(RUQOLA_LOG).nospace() << Q_FUNC_INFO << " Couldn't parse a valid JSON from argument: " << jsonString
                                        << "\n JSON parse error: " << jsonParseError.errorString();
        return {};
    }

    if (!doc.isArray()) {
        qCWarning(RUQOLA_LOG) << Q_FUNC_INFO << "The JSON string argument is not a JSON array." << jsonString;
        return {};
    }

    return doc.array();
}

QByteArray Utils::convertSha256Password(const QString &pwd)
{
    const QByteArray sha256pw = QCryptographicHash::hash(pwd.toUtf8(), QCryptographicHash::Sha256).toHex();
    return sha256pw;
}

QUrl Utils::avatarUrl(const QString &serverRcUrl, const AvatarInfo &avatarInfo)
{
    if (serverRcUrl.isEmpty()) {
        return {};
    }
    QString serverUrl = serverRcUrl;
    QString subFolder;
    switch (avatarInfo.avatarType) {
    case AvatarType::Room:
        subFolder = QStringLiteral("/room");
        break;
    case AvatarType::Unknown:
    case AvatarType::User:
        break;
    }
    subFolder += QLatin1Char('/') + avatarInfo.identifier;
    subFolder += QStringLiteral("?format=png");
    if (!avatarInfo.etag.isEmpty()) {
        subFolder += QStringLiteral("&etag=%1").arg(avatarInfo.etag);
    }
    subFolder += QStringLiteral("&size=22");
    if (!serverUrl.startsWith(QStringView(u"https://")) && !serverUrl.startsWith(QStringView(u"http://"))) {
        serverUrl.prepend(QStringLiteral("https://"));
    }
    return QUrl(serverUrl + QStringLiteral("/avatar") + subFolder);
}

QDebug operator<<(QDebug d, const Utils::AvatarInfo &t)
{
    d << "etag " << t.etag;
    d << "identifier " << t.identifier;
    d << "avatarType " << static_cast<int>(t.avatarType);
    return d;
}

QString Utils::emojiFontName()
{
#ifdef Q_OS_WIN
    return QStringLiteral("Segoe UI Emoji");
#else
    return QStringLiteral("NotoColorEmoji");
#endif
}

QString Utils::displaytextFromPresenceStatus(User::PresenceStatus status)
{
    switch (status) {
    case User::PresenceStatus::PresenceOnline:
        return i18n("Online");
    case User::PresenceStatus::PresenceBusy:
        return i18n("Busy");
    case User::PresenceStatus::PresenceAway:
        return i18n("Away");
    case User::PresenceStatus::PresenceOffline:
        return i18n("Offline");
    case User::PresenceStatus::Unknown:
        return {};
    }
    return {};
}
