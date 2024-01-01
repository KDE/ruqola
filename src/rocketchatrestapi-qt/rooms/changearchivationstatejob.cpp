/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "changearchivationstatejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ChangeArchivationStateJob::ChangeArchivationStateJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ChangeArchivationStateJob::~ChangeArchivationStateJob() = default;

bool ChangeArchivationStateJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChangeArchivationStateJob::start");
    submitPostRequest(json());

    return true;
}

void ChangeArchivationStateJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("ChangeArchivationStateJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT changeArchivationStateDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("ChangeArchivationStateJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString ChangeArchivationStateJob::roomId() const
{
    return mRoomId;
}

void ChangeArchivationStateJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

bool ChangeArchivationStateJob::archive() const
{
    return mArchive;
}

void ChangeArchivationStateJob::setArchive(bool archive)
{
    mArchive = archive;
}

bool ChangeArchivationStateJob::requireHttpAuthentication() const
{
    return true;
}

bool ChangeArchivationStateJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RoomId is Empty. Impossible to start ChangeArchivationStateJob";
        return false;
    }
    return true;
}

QNetworkRequest ChangeArchivationStateJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsChangeArchivactionState);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument ChangeArchivationStateJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("rid")] = mRoomId;
    jsonObj[QLatin1String("action")] = mArchive ? QStringLiteral("archive") : QStringLiteral("unarchive");

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_changearchivationstatejob.cpp"
