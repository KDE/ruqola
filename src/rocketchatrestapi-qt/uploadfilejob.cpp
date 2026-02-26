/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "uploadfilejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>
#include <QFile>
#include <QHttpMultiPart>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

#include <QMimeDatabase>
#include <QNetworkReply>

using namespace Qt::Literals::StringLiterals;
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
    const QMimeDatabase db;
    const QMimeType mimeType = db.mimeTypeForFile(fileNameAsLocalFile);

    auto multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(mimeType.name()));
    const QString filePartInfo = u"form-data; name=\"file\"; filename=\"%1\""_s.arg(mUploadFileInfo.fileName.isEmpty() ? mUploadFileInfo.filenameUrl.fileName()
                                                                                                                       : mUploadFileInfo.fileName);
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(filePartInfo));

    filePart.setBodyDevice(file);
    file->setParent(multiPart); // we cannot delete the file now, so delete it with the multiPart
    multiPart->append(filePart);

    QHttpPart msgPart;
    msgPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"msg\""_L1));
    msgPart.setBody(mUploadFileInfo.messageText.toUtf8());
    multiPart->append(msgPart);

    if (!mUploadFileInfo.threadMessageId.isEmpty()) {
        QHttpPart msgThreadPart;
        msgThreadPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"tmid\""_L1));
        msgThreadPart.setBody(mUploadFileInfo.threadMessageId);
        multiPart->append(msgThreadPart);
    }

    QHttpPart descriptionPart;
    descriptionPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"description\""_L1));
    descriptionPart.setBody(mUploadFileInfo.description.toUtf8());
    multiPart->append(descriptionPart);
    mReply = networkAccessManager()->post(request(), multiPart);
    connect(mReply, &QNetworkReply::uploadProgress, this, &UploadFileJob::slotUploadProgress);
    connect(mReply, &QNetworkReply::finished, this, &UploadFileJob::slotUploadFinished);
    multiPart->setParent(mReply); // delete the multiPart with the reply
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
    QUrl url;
    if (mUploadFileInfo.rc80Server) {
        url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsMedia,
                                          RestApiUtil::RestApiUrlExtensionType::V1,
                                          QLatin1StringView(mUploadFileInfo.roomId));
    } else {
        url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsUpload,
                                          RestApiUtil::RestApiUrlExtensionType::V1,
                                          QLatin1StringView(mUploadFileInfo.roomId));
    }
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);
    return request;
}

void UploadFileJob::abort()
{
    if (mReply) {
        mReply->abort();
    }
    deleteLater();
}

void UploadFileJob::slotUploadFinished()
{
    auto reply = mReply;
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject.value("success"_L1).toBool()) {
            qDebug() << " replyObject " << replyObject;
            if (mUploadFileInfo.rc80Server) {
                UploadFileJob::ConfirmMediaInfo confirmInfo;
                confirmInfo.parse(replyObject.value("file"_L1).toObject());
                confirmInfo.roomId = mUploadFileInfo.roomId;
                confirmInfo.description = mUploadFileInfo.description;
                confirmInfo.messageText = mUploadFileInfo.messageText;
                confirmInfo.threadMessageId = mUploadFileInfo.threadMessageId;
                Q_EMIT confirmMediaRequested(confirmInfo);
            }
            addLoggerInfo("UploadFileJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        } else {
            if (reply->error() != QNetworkReply::NoError) {
                if (mReply->error() == QNetworkReply::NetworkSessionFailedError) {
                    qCWarning(ROCKETCHATQTRESTAPI_LOG) << metaObject()->className() << "NetworkSessionFailedError. Connection loss?";
                    reply->deleteLater();
                    deleteLater();
                    return;
                }
                // Don't report cancel job when we canceled job
                if (reply->error() != QNetworkReply::OperationCanceledError) {
                    Q_EMIT failed(mReply->errorString() + u'\n' + errorStr(replyObject));
                }
            } else {
                emitFailedMessage(reply->errorString(), replyObject);
                addLoggerWarning("UploadFileJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
            }
        }
        reply->deleteLater();
    }

    Q_EMIT uploadFinished();
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

QDebug operator<<(QDebug d, const RocketChatRestApi::UploadFileJob::UploadFileInfo &t)
{
    d.space() << "roomId:" << t.roomId;
    d.space() << "description:" << t.description;
    d.space() << "messageText:" << t.messageText;
    d.space() << "filenameUrl:" << t.filenameUrl;
    d.space() << "fileName:" << t.fileName;
    d.space() << "threadMessageId:" << t.threadMessageId;
    d.space() << "deleteTemporaryFile:" << t.deleteTemporaryFile;
    return d;
}

QDebug operator<<(QDebug d, const RocketChatRestApi::UploadFileJob::ConfirmMediaInfo &t)
{
    d.space() << "roomId:" << t.roomId;
    d.space() << "fileId:" << t.fileId;
    d.space() << "description:" << t.description;
    d.space() << "messageText:" << t.messageText;
    return d;
}

void UploadFileJob::ConfirmMediaInfo::parse(const QJsonObject &obj)
{
    fileId = obj["_id"_L1].toString().toLatin1();
}

#include "moc_uploadfilejob.cpp"
