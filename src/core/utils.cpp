/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "utils.h"

#include "colorsandmessageviewstyle.h"
#include "rocketchataccount.h"
#include "ruqola_debug.h"
#include "ruqolaserverconfig.h"
#include <KLocalizedString>

#include <KColorScheme>
#include <QApplication>
#include <QCryptographicHash>
#include <QDateTime>
#include <QIcon>
#include <QJsonDocument>
#include <QJsonObject>
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
    return QUrl(serverUrl + u"/websocket"_s);
}

QString Utils::extractRoomUserFromUrl(QString url)
{
    url.remove(u"ruqola:/user/"_s);
    url.remove(u"ruqola:/room/"_s);
    return url;
}

QString Utils::formatQuotedRichText(const QuotedRichTextInfo &info)
{
    // Qt's support for borders is limited to tables, so we have to jump through some hoops...
    const auto backgroundColor = ColorsAndMessageViewStyle::self().schemeView().background(KColorScheme::AlternateBackground).color().name();
    const auto borderColor = qApp->palette().link().color().name();
    QString dateTimeInfo;
    if (!info.displayTime.isEmpty()) {
        if (!info.url.isEmpty()) {
            dateTimeInfo = u'\n' + u"<a href='%1'>%2</a>"_s.arg(info.url, info.displayTime);
        } else {
            dateTimeInfo = u'\n' + info.displayTime;
        }
    }
    return u"<table><tr><td style='background-color:%1; padding-left: 5px; border-left: 5px solid %2'>"_s.arg(backgroundColor, borderColor) + info.richText
        + dateTimeInfo + u"</td></tr></table>"_s;
}

QString Utils::presenceStatusToString(User::PresenceStatus status)
{
    switch (status) {
    case User::PresenceStatus::Online:
        return u"online"_s;
    case User::PresenceStatus::Busy:
        return u"busy"_s;
    case User::PresenceStatus::Away:
        return u"away"_s;
    case User::PresenceStatus::Offline:
        return u"offline"_s;
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
        return u"user-online"_s;
    case User::PresenceStatus::Busy:
        return u"user-busy"_s;
    case User::PresenceStatus::Away:
        return u"user-away"_s;
    case User::PresenceStatus::Offline:
        return u"user-offline"_s;
    case User::PresenceStatus::Unknown:
        return u"unknown"_s;
    }
    qCWarning(RUQOLA_LOG) << "Unknown status" << status;
    return u"unknown"_s;
}

QString Utils::iconFromStatus(const QString &status)
{
    if (status == "online"_L1) {
        return u"user-online"_s;
    } else if (status == "busy"_L1) {
        return u"user-busy"_s;
    } else if (status == "away"_L1) {
        return u"user-away"_s;
    } else if (status == "offline"_L1) {
        return u"user-offline"_s;
    } else {
        qCWarning(RUQOLA_LOG) << "Unknown status" << status;
        return u"unknown"_s;
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
    static const QRegularExpression regularUnCheckMark(u"(^|\\n)-\\s\\[\\s\\]\\s"_s);
    static const QRegularExpression regularCheckMark(u"(^|\\n)-\\s\\[x]\\s"_s);
    QString newStr = str;
    newStr = newStr.replace(regularUnCheckMark, u"\\1:white_medium_square: "_s);
    newStr = newStr.replace(regularCheckMark, u"\\1:ballot_box_with_check: "_s);
    return newStr;
}

QString Utils::convertTextWithUrl(const QString &str)
{
    static const QRegularExpression regularExpressionAHref(u"<a href=\"(.*)\">(.*)</a>"_s);
    static const QRegularExpression regularExpressionCustomAHref(u"<a href=\"(.*)\\|(.*)\">(.*)</a>"_s);
    QString newStr;
    bool isRef = false;
    bool isUrl = false;
#if 0
    bool isHasNewRef = false;
#endif
    QString url;
    QString references;
    for (int i = 0; i < str.length(); ++i) {
        const QChar ref = str.at(i);
        if (ref == u'[') {
            if (isRef) {
                isRef = false;
                newStr += u'[' + references + u'[';
                references.clear();
            } else {
                isRef = true;
            }
#if 0
        } else if (isUrl && ref == u']' && isHasNewRef) {
            isUrl = false;
            isRef = false;
            newStr += u"<a href=\'%1'>%2</a>"_s.arg(url, references);
            references.clear();
            url.clear();
#endif
        } else if (isRef && ref == u']') {
            isRef = false;
            if ((i == str.length() - 1) || (str.at(i + 1) != u'(')) {
                if (references.startsWith(u'<')) {
                    newStr += references.replace(regularExpressionCustomAHref, u"<a href=\"\\2\">\\1</a>"_s);
                } else {
                    newStr += u'[' + references + u']';
                }
                references.clear();
            }
        } else if (ref == u'(' && !references.isEmpty()) {
            isUrl = true;
        } else if (isUrl && ref == u')' && !references.isEmpty()) {
            isUrl = false;
            // detect whether the string already contains HTML <a/> tags
            if (url.startsWith(u'<')) {
                newStr += url.replace(regularExpressionAHref, u"<a href=\"\\1\">%1</a>"_s.arg(references));
            } else {
                newStr += u"<a href=\'%1'>%2</a>"_s.arg(url, references);
            }
            references.clear();
            url.clear();
#if 0
        } else if (ref == u'|' && !references.isEmpty()) {
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
        newStr += u'[' + references;
    } else if (isUrl) {
        newStr += u'[' + references + "]("_L1 + url;
#if 0
    } else if (isHasNewRef) {
        if (!url.isEmpty() && !references.isEmpty()) {
            newStr += u"<a href=\'%1'>%2</a>"_s.arg(url, references);
        }
#endif
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
        subFolder = u"/room"_s;
        break;
    case AvatarType::Unknown:
    case AvatarType::User:
    case AvatarType::UserAndRoom:
        break;
    }
    subFolder += u'/' + identifier;
    subFolder += u"?format=png"_s;
    if (!etag.isEmpty()) {
        subFolder += u"&etag=%1"_s.arg(etag);
    }
    subFolder += u"&size=22"_s;
    if (!serverUrl.startsWith(QStringView(u"https://")) && !serverUrl.startsWith(QStringView(u"http://"))) {
        serverUrl.prepend(u"https://"_s);
    }
    return QUrl(serverUrl + u"/avatar"_s + subFolder);
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
        return identifier + u'-' + etag;
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
        newAccountName = u"%1%2"_s.arg(accountName, QString::number(i));
        ++i;
    }
    return newAccountName;
}

bool Utils::userActivity(const QJsonArray &contents)
{
    if (contents.toVariantList().isEmpty()) {
        return false;
    }
    // const QString val = contents.toVariantList().at(1).toString();
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
