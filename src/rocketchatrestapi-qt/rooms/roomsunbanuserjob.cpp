/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomsunbanuserjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
RoomsUnbanUserJob::RoomsUnbanUserJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RoomsUnbanUserJob::~RoomsUnbanUserJob() = default;

bool RoomsUnbanUserJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("RoomsUnbanUserJob::start");
    submitPostRequest(json());

    return true;
}

void RoomsUnbanUserJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("RoomsUnbanUserJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT roomsMediaConfirmDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("RoomsUnbanUserJob: problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QByteArray RoomsUnbanUserJob::tmid() const
{
    return mTmid;
}

void RoomsUnbanUserJob::setTmid(const QByteArray &newTmid)
{
    mTmid = newTmid;
}

QString RoomsUnbanUserJob::description() const
{
    return mDescription;
}

void RoomsUnbanUserJob::setDescription(const QString &newDescription)
{
    mDescription = newDescription;
}

QByteArray RoomsUnbanUserJob::fileId() const
{
    return mFileId;
}

void RoomsUnbanUserJob::setFileId(const QByteArray &newFileId)
{
    mFileId = newFileId;
}

QByteArray RoomsUnbanUserJob::roomId() const
{
    return mRoomId;
}

void RoomsUnbanUserJob::setRoomId(const QByteArray &newRoomId)
{
    mRoomId = newRoomId;
}

bool RoomsUnbanUserJob::requireHttpAuthentication() const
{
    return true;
}

bool RoomsUnbanUserJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mFileId.isEmpty() || mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RoomsUnbanUserJob: mFileId or mRoomId is empty.";
        return false;
    }

    return true;
}

QJsonDocument RoomsUnbanUserJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["description"_L1] = mDescription;
    if (!mTmid.isEmpty()) {
        jsonObj["tmid"_L1] = QString::fromLatin1(mTmid);
    }
    // TODO jsonObj["msg"_L1] = QString::fromLatin1(mFileId);
    const QJsonDocument postData = QJsonDocument(jsonObj);
    // qDebug() << " postData**************** " << postData;
    return postData;
}

QNetworkRequest RoomsUnbanUserJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsMediaConfirm,
                                                 RestApiUtil::RestApiUrlExtensionType::V1,
                                                 QStringLiteral("%1/%2").arg(QString::fromLatin1(mRoomId), QString::fromLatin1(mFileId)));
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_roomsunbanuserjob.cpp"
