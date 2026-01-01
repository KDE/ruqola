/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamdeletejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
TeamDeleteJob::TeamDeleteJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

TeamDeleteJob::~TeamDeleteJob() = default;

bool TeamDeleteJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("TeamDeleteJob::start");
    submitPostRequest(json());

    return true;
}

void TeamDeleteJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("TeamDeleteJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT deleteTeamDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("TeamDeleteJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QList<QByteArray> TeamDeleteJob::roomsId() const
{
    return mRoomToDeleteId;
}

void TeamDeleteJob::setRoomsId(const QList<QByteArray> &roomsId)
{
    mRoomToDeleteId = roomsId;
}

QByteArray TeamDeleteJob::teamId() const
{
    return mTeamId;
}

void TeamDeleteJob::setTeamId(const QByteArray &teamId)
{
    mTeamId = teamId;
}

bool TeamDeleteJob::requireHttpAuthentication() const
{
    return true;
}

bool TeamDeleteJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mTeamId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "TeamDeleteJob: mTeamId is empty";
        return false;
    }
    return true;
}

QNetworkRequest TeamDeleteJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::TeamsDelete);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument TeamDeleteJob::json() const
{
    QJsonObject jsonObj;
    if (!mRoomToDeleteId.isEmpty()) {
        QStringList lst;
        for (const QByteArray &b : std::as_const(mRoomToDeleteId)) {
            lst.append(QLatin1StringView(b));
        }
        jsonObj["roomsToRemove"_L1] = QJsonArray::fromStringList(lst);
    }
    jsonObj["teamId"_L1] = QLatin1StringView(mTeamId);
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_teamdeletejob.cpp"
