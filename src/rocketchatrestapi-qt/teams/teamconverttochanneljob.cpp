/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamconverttochanneljob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
TeamConvertToChannelJob::TeamConvertToChannelJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

TeamConvertToChannelJob::~TeamConvertToChannelJob() = default;

bool TeamConvertToChannelJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("TeamConvertToChannelJob::start"_ba);
    submitPostRequest(json());

    return true;
}

void TeamConvertToChannelJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    if (checkResponse("TeamConvertToChannelJob"_ba, replyErrorString, replyJson)) {
        Q_EMIT teamConvertToChannelDone();
    }
}

const QList<QByteArray> &TeamConvertToChannelJob::roomsToRemove() const
{
    return mRoomsToRemove;
}

void TeamConvertToChannelJob::setRoomsToRemove(const QList<QByteArray> &newRoomsToRemove)
{
    mRoomsToRemove = newRoomsToRemove;
}

QByteArray TeamConvertToChannelJob::teamId() const
{
    return mTeamId;
}

void TeamConvertToChannelJob::setTeamId(const QByteArray &teamId)
{
    mTeamId = teamId;
}

bool TeamConvertToChannelJob::requireHttpAuthentication() const
{
    return true;
}

bool TeamConvertToChannelJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mTeamId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "TeamConvertToChannelJob: mTeamId is empty";
        return false;
    }
    return true;
}

QNetworkRequest TeamConvertToChannelJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::TeamsConvertToChannel);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument TeamConvertToChannelJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["teamId"_L1] = QLatin1StringView(mTeamId);
    if (!mRoomsToRemove.isEmpty()) {
        QStringList lst;
        for (const QByteArray &b : std::as_const(mRoomsToRemove)) {
            lst.append(QLatin1StringView(b));
        }
        jsonObj["roomsToRemove"_L1] = QJsonArray::fromStringList(lst);
    }
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_teamconverttochanneljob.cpp"
