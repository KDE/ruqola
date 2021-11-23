/*
   Copyright (c) 2018-2021 Laurent Montel <montel@kde.org>

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
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>
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

UploadFileJob::~UploadFileJob() = default;

bool UploadFileJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mUploadFileInfo.roomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "UploadFileJob : RoomId not defined";
        return false;
    }
    if (!mUploadFileInfo.filenameUrl.isLocalFile()) {
        // shouldn't be too hard, just use KIO::storedGet
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Uploading remote files is not supported yet";
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
    const QString fileNameAsLocalFile = mUploadFileInfo.filenameUrl.toLocalFile();
    auto file = new QFile(fileNameAsLocalFile);
    if (!file->open(QIODevice::ReadOnly)) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << " Impossible to open filename " << mUploadFileInfo.filenameUrl;
        Q_EMIT failed(i18n("File not found \'%1\'", fileNameAsLocalFile));
        delete file;
        deleteLater();
        return false;
    }
    QMimeDatabase db;
    const QMimeType mimeType = db.mimeTypeForFile(fileNameAsLocalFile);

    auto multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(mimeType.name()));
    const QString filePartInfo = QStringLiteral("form-data; name=\"file\"; filename=\"%1\"").arg(mUploadFileInfo.filenameUrl.fileName());
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(filePartInfo));

    filePart.setBodyDevice(file);
    file->setParent(multiPart); // we cannot delete the file now, so delete it with the multiPart
    multiPart->append(filePart);

    QHttpPart msgPart;
    msgPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QLatin1String("form-data; name=\"msg\"")));
    msgPart.setBody(mUploadFileInfo.messageText.toUtf8());
    multiPart->append(msgPart);

    if (!mUploadFileInfo.threadMessageId.isEmpty()) {
        QHttpPart msgPart;
        msgPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QLatin1String("form-data; name=\"tmid\"")));
        msgPart.setBody(mUploadFileInfo.threadMessageId.toUtf8());
        multiPart->append(msgPart);
    }

    QHttpPart descriptionPart;
    descriptionPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QLatin1String("form-data; name=\"description\"")));
    descriptionPart.setBody(mUploadFileInfo.description.toUtf8());
    multiPart->append(descriptionPart);
    QNetworkReply *reply = networkAccessManager()->post(request(), multiPart);
    connect(reply, &QNetworkReply::uploadProgress, this, &UploadFileJob::slotUploadProgress);
    connect(reply, &QNetworkReply::finished, this, &UploadFileJob::slotUploadFinished);
    multiPart->setParent(reply); // delete the multiPart with the reply
    // TODO signal error ?
    addStartRestApiInfo("UploadFileJob::start");
    return true;
}

void UploadFileJob::slotUploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    UploadFileJob::UploadStatusInfo info;
    info.bytesSent = bytesSent;
    info.bytesTotal = bytesTotal;
    info.fileName = mUploadFileInfo.filenameUrl.fileName();
    Q_EMIT uploadProgress(info);
}

QNetworkRequest UploadFileJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsUpload, mUploadFileInfo.roomId);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);

    return request;
}

void UploadFileJob::slotUploadFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject.value(QLatin1String("success")).toBool()) {
            addLoggerInfo(QByteArrayLiteral("UploadFileJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("UploadFileJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

UploadFileJob::UploadFileInfo UploadFileJob::uploadFileInfo() const
{
    return mUploadFileInfo;
}

void UploadFileJob::setUploadFileInfo(const UploadFileInfo &uploadFileInfo)
{
    mUploadFileInfo = uploadFileInfo;
}

bool UploadFileJob::requireHttpAuthentication() const
{
    return true;
}

bool UploadFileJob::UploadFileInfo::isValid() const
{
    return !roomId.isEmpty() && !filenameUrl.isEmpty();
}
