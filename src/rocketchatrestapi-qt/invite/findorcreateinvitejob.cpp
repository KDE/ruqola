/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "findorcreateinvitejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <KLocalizedString>

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
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

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("FindOrCreateInviteJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT findOrCreateInviteDone(parseResult(replyObject));
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("FindOrCreateInviteJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

FindOrCreateInviteJob::InviteUsersInfo FindOrCreateInviteJob::parseResult(const QJsonObject &replyObject)
{
    FindOrCreateInviteJob::InviteUsersInfo info;
    info.url = QUrl(replyObject["url"_L1].toString());
    info.userId = replyObject["userId"_L1].toString();
    info.roomId = replyObject["rid"_L1].toString();
    info.expireDateTime = QDateTime::fromString(replyObject["expires"_L1].toString(), Qt::ISODate).toString();
    info.maxUses = replyObject["maxUses"_L1].toInt();
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

QByteArray FindOrCreateInviteJob::roomId() const
{
    return mRoomId;
}

void FindOrCreateInviteJob::setRoomId(const QByteArray &roomId)
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
    jsonObj["rid"_L1] = QLatin1StringView(mRoomId);
    jsonObj["days"_L1] = mNumberOfDays;
    jsonObj["maxUses"_L1] = mMaxUses;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString FindOrCreateInviteJob::errorMessage(const QString &str, const QJsonObject &details)
{
    if (str == "not_authorized"_L1) {
        return i18n("Generate link is not authorized in this channel.");
    }
    return RestApiAbstractJob::errorMessage(str, details);
}

QDebug operator<<(QDebug d, const FindOrCreateInviteJob::InviteUsersInfo &t)
{
    d.space() << "url: " << t.url;
    d.space() << "roomId: " << t.roomId;
    d.space() << "userId: " << t.userId;
    d.space() << "expireDateTime: " << t.expireDateTime;
    d.space() << "maxUses: " << t.maxUses;
    return d;
}

bool FindOrCreateInviteJob::InviteUsersInfo::operator==(const FindOrCreateInviteJob::InviteUsersInfo &other) const
{
    return other.url == url && other.roomId == roomId && other.userId == userId && other.expireDateTime == expireDateTime && other.maxUses == maxUses;
}

#include "moc_findorcreateinvitejob.cpp"
