/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "findorcreateinvitejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <KLocalizedString>

#include <QJsonDocument>
#include <QJsonObject>
using namespace RocketChatRestApi;
FindOrCreateInviteJob::FindOrCreateInviteJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

FindOrCreateInviteJob::~FindOrCreateInviteJob() = default;

bool FindOrCreateInviteJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("FindOrCreateInviteJob::start");
    submitPostRequest(json());

    return true;
}

void FindOrCreateInviteJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QLatin1StringView("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("FindOrCreateInviteJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT findOrCreateInviteDone(parseResult(replyObject));
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("FindOrCreateInviteJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

FindOrCreateInviteJob::InviteUsersInfo FindOrCreateInviteJob::parseResult(const QJsonObject &replyObject)
{
    FindOrCreateInviteJob::InviteUsersInfo info;
    info.url = QUrl(replyObject[QLatin1StringView("url")].toString());
    info.userId = replyObject[QLatin1StringView("userId")].toString();
    info.roomId = replyObject[QLatin1StringView("rid")].toString();
    info.expireDateTime = QDateTime::fromString(replyObject[QLatin1StringView("expires")].toString(), Qt::ISODate).toString();
    info.maxUses = replyObject[QLatin1StringView("maxUses")].toInt();
    return info;
}

int FindOrCreateInviteJob::numberOfDays() const
{
    return mNumberOfDays;
}

void FindOrCreateInviteJob::setNumberOfDays(int numberOfDays)
{
    mNumberOfDays = numberOfDays;
}

int FindOrCreateInviteJob::maxUses() const
{
    return mMaxUses;
}

void FindOrCreateInviteJob::setMaxUses(int maxUses)
{
    mMaxUses = maxUses;
}

QString FindOrCreateInviteJob::roomId() const
{
    return mRoomId;
}

void FindOrCreateInviteJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

bool FindOrCreateInviteJob::requireHttpAuthentication() const
{
    return true;
}

bool FindOrCreateInviteJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RoomId is Empty. Impossible to start FindOrCreateInviteJob";
        return false;
    }
    if (mMaxUses < 0) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "MaxUses is negative it's a bug. Impossible to start FindOrCreateInviteJob";
        return false;
    }
    if (mNumberOfDays < 0) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Number of days is negative. Impossible to start FindOrCreateInviteJob";
        return false;
    }
    return true;
}

QNetworkRequest FindOrCreateInviteJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::FindOrCreateInvite);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument FindOrCreateInviteJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1StringView("rid")] = mRoomId;
    jsonObj[QLatin1StringView("days")] = mNumberOfDays;
    jsonObj[QLatin1StringView("maxUses")] = mMaxUses;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString FindOrCreateInviteJob::errorMessage(const QString &str, const QJsonObject &details)
{
    if (str == QLatin1StringView("not_authorized")) {
        return i18n("Generate link is not authorized in this channel.");
    }
    return RestApiAbstractJob::errorMessage(str, details);
}

QDebug operator<<(QDebug d, const FindOrCreateInviteJob::InviteUsersInfo &t)
{
    d << " url: " << t.url;
    d << " roomId: " << t.roomId;
    d << " userId: " << t.userId;
    d << " expireDateTime: " << t.expireDateTime;
    d << " maxUses: " << t.maxUses;
    return d;
}

bool FindOrCreateInviteJob::InviteUsersInfo::operator==(const FindOrCreateInviteJob::InviteUsersInfo &other) const
{
    return other.url == url && other.roomId == roomId && other.userId == userId && other.expireDateTime == expireDateTime && other.maxUses == maxUses;
}

#include "moc_findorcreateinvitejob.cpp"
