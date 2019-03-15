/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "uploadfilejob.h"
#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"
#include <QFile>
#include <QHttpMultiPart>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMimeDatabase>
#include <QNetworkReply>
using namespace RocketChatRestApi;
UploadFileJob::UploadFileJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

UploadFileJob::~UploadFileJob()
{
}

bool UploadFileJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start upload file job";
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "UploadFileJob : RoomId not defined";
        return false;
    }
    return true;
}

bool UploadFileJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    QFile *file = new QFile(mFilenameUrl.path());
    if (!file->open(QIODevice::ReadOnly)) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << " Impossible to open filename " << mFilenameUrl;
        delete file;
        deleteLater();
        return false;
    }
    QMimeDatabase db;
    const QMimeType mimeType = db.mimeTypeForFile(mFilenameUrl.path());

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(mimeType.name()));
    const QString filePartInfo = QStringLiteral("form-data; name=\"file\"; filename=\"%1\"").arg(mFilenameUrl.fileName());
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(filePartInfo));

    filePart.setBodyDevice(file);
    file->setParent(multiPart); // we cannot delete the file now, so delete it with the multiPart
    multiPart->append(filePart);

    QHttpPart msgPart;
    msgPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QLatin1String("form-data; name=\"msg\"")));
    msgPart.setBody(mMessageText.toUtf8());
    multiPart->append(msgPart);

    QHttpPart descriptionPart;
    descriptionPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QLatin1String("form-data; name=\"description\"")));
    descriptionPart.setBody(mDescription.toUtf8());
    multiPart->append(descriptionPart);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), multiPart);
    connect(reply, &QNetworkReply::uploadProgress, this, &UploadFileJob::uploadProgress);
    connect(reply, &QNetworkReply::finished, this, &UploadFileJob::slotUploadFinished);
    multiPart->setParent(reply); // delete the multiPart with the reply
    //TODO signal error ?

    return true;
}

QNetworkRequest UploadFileJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsUpload, mRoomId);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    return request;
}

void UploadFileJob::slotUploadFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject.value(QLatin1String("success")).toBool()) {
            addLoggerInfo(QByteArrayLiteral("UploadFileJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        } else {
            emitFailedMessage(replyObject);
            addLoggerWarning(QByteArrayLiteral("UploadFileJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QString UploadFileJob::roomId() const
{
    return mRoomId;
}

void UploadFileJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QString UploadFileJob::description() const
{
    return mDescription;
}

void UploadFileJob::setDescription(const QString &description)
{
    mDescription = description;
}

QString UploadFileJob::messageText() const
{
    return mMessageText;
}

void UploadFileJob::setMessageText(const QString &messageText)
{
    mMessageText = messageText;
}

QUrl UploadFileJob::filenameUrl() const
{
    return mFilenameUrl;
}

void UploadFileJob::setFilenameUrl(const QUrl &filenameUrl)
{
    mFilenameUrl = filenameUrl;
}

bool UploadFileJob::requireHttpAuthentication() const
{
    return true;
}
