/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomsbanuserjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
RoomsBanUserJob::RoomsBanUserJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RoomsBanUserJob::~RoomsBanUserJob() = default;

bool RoomsBanUserJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("RoomsBanUserJob::start");
    submitPostRequest(json());

    return true;
}

void RoomsBanUserJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("RoomsBanUserJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT roomsMediaConfirmDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("RoomsBanUserJob: problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QByteArray RoomsBanUserJob::tmid() const
{
    return mTmid;
}

void RoomsBanUserJob::setTmid(const QByteArray &newTmid)
{
    mTmid = newTmid;
}

QString RoomsBanUserJob::description() const
{
    return mDescription;
}

void RoomsBanUserJob::setDescription(const QString &newDescription)
{
    mDescription = newDescription;
}

QByteArray RoomsBanUserJob::fileId() const
{
    return mFileId;
}

void RoomsBanUserJob::setFileId(const QByteArray &newFileId)
{
    mFileId = newFileId;
}

QByteArray RoomsBanUserJob::roomId() const
{
    return mRoomId;
}

void RoomsBanUserJob::setRoomId(const QByteArray &newRoomId)
{
    mRoomId = newRoomId;
}

bool RoomsBanUserJob::requireHttpAuthentication() const
{
    return true;
}

bool RoomsBanUserJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mFileId.isEmpty() || mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RoomsBanUserJob: mFileId or mRoomId is empty.";
        return false;
    }

    return true;
}

QJsonDocument RoomsBanUserJob::json() const
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

QNetworkRequest RoomsBanUserJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsMediaConfirm,
                                                 RestApiUtil::RestApiUrlExtensionType::V1,
                                                 QStringLiteral("%1/%2").arg(QString::fromLatin1(mRoomId), QString::fromLatin1(mFileId)));
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_roomsbanuserjob.cpp"
