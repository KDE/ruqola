/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomsmediaconfirmjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
RoomsMediaConfirmJob::RoomsMediaConfirmJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RoomsMediaConfirmJob::~RoomsMediaConfirmJob() = default;

bool RoomsMediaConfirmJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("RoomsMediaConfirmJob::start");
    submitPostRequest(json());

    return true;
}

void RoomsMediaConfirmJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("RoomsMediaConfirmJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT roomsMediaConfirmDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("RoomsMediaConfirmJob: problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QByteArray RoomsMediaConfirmJob::tmid() const
{
    return mTmid;
}

void RoomsMediaConfirmJob::setTmid(const QByteArray &newTmid)
{
    mTmid = newTmid;
}

QString RoomsMediaConfirmJob::description() const
{
    return mDescription;
}

void RoomsMediaConfirmJob::setDescription(const QString &newDescription)
{
    mDescription = newDescription;
}

QByteArray RoomsMediaConfirmJob::fileId() const
{
    return mFileId;
}

void RoomsMediaConfirmJob::setFileId(const QByteArray &newFileId)
{
    mFileId = newFileId;
}

QByteArray RoomsMediaConfirmJob::roomId() const
{
    return mRoomId;
}

void RoomsMediaConfirmJob::setRoomId(const QByteArray &newRoomId)
{
    mRoomId = newRoomId;
}

bool RoomsMediaConfirmJob::requireHttpAuthentication() const
{
    return true;
}

bool RoomsMediaConfirmJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mFileId.isEmpty() || mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RoomsMediaConfirmJob: mFileId or mRoomId is empty.";
        return false;
    }

    return true;
}

QJsonDocument RoomsMediaConfirmJob::json() const
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

QNetworkRequest RoomsMediaConfirmJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsMediaConfirm,
                                                 RestApiUtil::RestApiUrlExtensionType::V1,
                                                 QStringLiteral("%1/%2").arg(QString::fromLatin1(mRoomId), QString::fromLatin1(mFileId)));
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_roomsmediaconfirmjob.cpp"
