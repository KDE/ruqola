/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "utils.h"

#include "colorsandmessageviewstyle.h"
#include "rocketchataccount.h"
#include "ruqola_debug.h"
#include <KLocalizedString>

#include <KColorScheme>
#include <QCoreApplication>
#include <QCryptographicHash>
#include <QDateTime>
#include <QIcon>
#include <QJsonDocument>
#include <QRegularExpression>

#include <TextEmoticonsCore/EmoticonUnicodeUtils>

using namespace Qt::Literals::StringLiterals;
QUrl Utils::generateServerUrl(const QString &url)
{
    if (url.isEmpty()) {
        return {};
    }
    QString serverUrl = url.trimmed();
    if (serverUrl.startsWith("https://"_L1)) {
        serverUrl.replace("https://"_L1, "wss://"_L1);
    } else if (serverUrl.startsWith("http://"_L1)) {
        serverUrl.replace("http://"_L1, "ws://"_L1);
    } else {
        serverUrl = "wss://"_L1 + serverUrl;
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
    const auto backgroundColor = ColorsAndMessageViewStyle::self().schemeView().background(KColorScheme::AlternateBackground).color().name();
    const auto borderColor = ColorsAndMessageViewStyle::self().schemeView().foreground(KColorScheme::LinkText).color().name();
    QString dateTimeInfo;
    if (!info.displayTime.isEmpty()) {
        if (!info.url.isEmpty()) {
            dateTimeInfo = QLatin1Char('\n') + QStringLiteral("<a href='%1'>%2</a>").arg(info.url, info.displayTime);
        } else {
            dateTimeInfo = QLatin1Char('\n') + info.displayTime;
        }
    }
    return QStringLiteral("<table><tr><td style='background-color:%1; padding-left: 5px; border-left: 5px solid %2'>").arg(backgroundColor, borderColor)
        + info.richText + dateTimeInfo + QStringLiteral("</td></tr></table>");
}

QString Utils::presenceStatusToString(User::PresenceStatus status)
{
    switch (status) {
    case User::PresenceStatus::Online:
        return QStringLiteral("online");
    case User::PresenceStatus::Busy:
        return QStringLiteral("busy");
    case User::PresenceStatus::Away:
        return QStringLiteral("away");
    case User::PresenceStatus::Offline:
        return QStringLiteral("offline");
    case User::PresenceStatus::Unknown:
        return {};
    }

    return {};
}

QString Utils::i18nFromPresenceStatus(User::PresenceStatus status)
{
    switch (status) {
    case User::PresenceStatus::Online:
        return i18n("Online");
    case User::PresenceStatus::Busy:
        return i18n("Busy");
    case User::PresenceStatus::Away:
        return i18n("Away");
    case User::PresenceStatus::Offline:
        return i18n("Offline");
    case User::PresenceStatus::Unknown:
        return {};
    }
    return {};
}

QString Utils::iconFromPresenceStatus(User::PresenceStatus status)
{
    switch (status) {
    case User::PresenceStatus::Online:
        return QStringLiteral("user-online");
    case User::PresenceStatus::Busy:
        return QStringLiteral("user-busy");
    case User::PresenceStatus::Away:
        return QStringLiteral("user-away");
    case User::PresenceStatus::Offline:
        return QStringLiteral("user-offline");
    case User::PresenceStatus::Unknown:
        return QStringLiteral("unknown");
    }
    qCWarning(RUQOLA_LOG) << "Unknown status" << status;
    return QStringLiteral("unknown");
}

QString Utils::iconFromStatus(const QString &status)
{
    if (status == "online"_L1) {
        return QStringLiteral("user-online");
    } else if (status == "busy"_L1) {
        return QStringLiteral("user-busy");
    } else if (status == "away"_L1) {
        return QStringLiteral("user-away");
    } else if (status == "offline"_L1) {
        return QStringLiteral("user-offline");
    } else {
        qCWarning(RUQOLA_LOG) << "Unknown status" << status;
        return QStringLiteral("unknown");
    }
}

User::PresenceStatus Utils::presenceStatusFromString(const QString &status)
{
    if (status == "online"_L1) {
        return User::PresenceStatus::Online;
    } else if (status == "busy"_L1) {
        return User::PresenceStatus::Busy;
    } else if (status == "away"_L1) {
        return User::PresenceStatus::Away;
    } else if (status == "offline"_L1) {
        return User::PresenceStatus::Offline;
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
    return o.value(key).toObject().value("$date"_L1).toDouble(-1);
}

qint64 Utils::parseIsoDate(const QString &key, const QJsonObject &o)
{
    if (o.contains(key)) {
        return QDateTime::fromString(o.value(key).toString(), Qt::ISODate).toMSecsSinceEpoch();
    } else {
        return -1;
    }
}

QString Utils::convertTextWithCheckMark(const QString &str)
{
    static const QRegularExpression regularUnCheckMark(QStringLiteral("(^|\\n)-\\s\\[\\s\\]\\s"));
    static const QRegularExpression regularCheckMark(QStringLiteral("(^|\\n)-\\s\\[x]\\s"));
    QString newStr = str;
    newStr = newStr.replace(regularUnCheckMark, QStringLiteral("\\1:white_medium_square: "));
    newStr = newStr.replace(regularCheckMark, QStringLiteral("\\1:ballot_box_with_check: "));
    return newStr;
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
    for (int i = 0; i < str.length(); ++i) {
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
            if ((i == str.length() - 1) || (str.at(i + 1) != QLatin1Char('('))) {
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
        newStr += QLatin1Char('[') + references + "]("_L1 + url;
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

QUrl Utils::AvatarInfo::avatarUrl(const QString &serverRcUrl) const
{
    if (serverRcUrl.isEmpty()) {
        return {};
    }
    QString serverUrl = serverRcUrl;
    QString subFolder;
    switch (avatarType) {
    case AvatarType::Room:
        subFolder = QStringLiteral("/room");
        break;
    case AvatarType::Unknown:
    case AvatarType::User:
    case AvatarType::UserAndRoom:
        break;
    }
    subFolder += QLatin1Char('/') + identifier;
    subFolder += QStringLiteral("?format=png");
    if (!etag.isEmpty()) {
        subFolder += QStringLiteral("&etag=%1").arg(etag);
    }
    subFolder += QStringLiteral("&size=22");
    if (!serverUrl.startsWith(QStringView(u"https://")) && !serverUrl.startsWith(QStringView(u"http://"))) {
        serverUrl.prepend(QStringLiteral("https://"));
    }
    return QUrl(serverUrl + QStringLiteral("/avatar") + subFolder);
}

QDebug operator<<(QDebug d, const Utils::AvatarInfo &t)
{
    d.space() << "etag " << t.etag;
    d.space() << "identifier " << t.identifier;
    d.space() << "avatarType " << static_cast<int>(t.avatarType);
    return d;
}

QString Utils::emojiFontName()
{
    return TextEmoticonsCore::EmoticonUnicodeUtils::emojiFontName();
}

QString Utils::displaytextFromPresenceStatus(User::PresenceStatus status)
{
    switch (status) {
    case User::PresenceStatus::Online:
        return i18n("Online");
    case User::PresenceStatus::Busy:
        return i18n("Busy");
    case User::PresenceStatus::Away:
        return i18n("Away");
    case User::PresenceStatus::Offline:
        return i18n("Offline");
    case User::PresenceStatus::Unknown:
        return {};
    }
    return {};
}

QString Utils::AvatarInfo::generateAvatarIdentifier() const
{
    if (etag.isEmpty()) {
        return identifier;
    } else {
        return identifier + QLatin1Char('-') + etag;
    }
}

QIcon Utils::iconFromAccount(RocketChatAccount *account)
{
    QIcon icon;
    const QString iconFaviconUrl{account->attachmentUrlFromLocalCache(account->ruqolaServerConfig()->faviconUrl().url).toLocalFile()};
    if (!iconFaviconUrl.isEmpty()) {
        const QIcon iconFavicon{iconFaviconUrl};
        if (!iconFavicon.isNull()) {
            icon = std::move(iconFavicon);
        }
    }
    return icon;
}

bool Utils::validUser(const QString &userName)
{
    return (userName != "here"_L1 && userName != "all"_L1);
}

QString Utils::createUniqueAccountName(const QStringList &list, const QString &accountName)
{
    QString newAccountName = accountName;
    int i = 1;
    while (list.contains(newAccountName)) {
        newAccountName = QStringLiteral("%1%2").arg(accountName, QString::number(i));
        ++i;
    }
    return newAccountName;
}

QString Utils::findExecutable(const QString &exec)
{
#ifdef Q_OS_WIN
    const QString executableName = exec + QStringLiteral(".exe");
    QString path = QStandardPaths::findExecutable(executableName, {QCoreApplication::applicationDirPath()});
    if (path.isEmpty()) {
        path = QStandardPaths::findExecutable(executableName);
    }
#else
    const QString path = QStandardPaths::findExecutable(exec);
#endif
    return path;
}

bool Utils::executableFound(const QString &exec)
{
    return !Utils::findExecutable(exec).isEmpty();
}

bool Utils::userActivity(const QJsonArray &contents)
{
    if (contents.toVariantList().isEmpty()) {
        return false;
    }
    const QString val = contents.toVariantList().at(1).toString();
    // qDebug() << " val ************ " << val << " contents.toVariantList().at(1 " << contents.toVariantList().at(1);
    bool status = false;
    if (contents.toVariantList().at(1).toBool()) {
        // qDebug() << " TYPING *************************************************";
        status = true;
    } else if (!contents.toVariantList().at(1).toList().isEmpty()) {
        if (contents.toVariantList().at(1).toList().at(0).toString() == "user-typing"_L1) {
            // qDebug() << " FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF typing user";
            status = true;
        }
    }
    return status;
}
