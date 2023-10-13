/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

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
    const QString filePartInfo = QStringLiteral("form-data; name=\"file\"; filename=\"%1\"")
                                     .arg(mUploadFileInfo.fileName.isEmpty() ? mUploadFileInfo.filenameUrl.fileName() : mUploadFileInfo.fileName);
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
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsUpload, mUploadFileInfo.roomId);
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
        if (replyObject.value(QLatin1String("success")).toBool()) {
            addLoggerInfo(QByteArrayLiteral("UploadFileJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        } else {
            if (reply->error() != QNetworkReply::NoError) {
                if (mReply->error() == QNetworkReply::NetworkSessionFailedError) {
                    qCWarning(ROCKETCHATQTRESTAPI_LOG) << metaObject()->className() << "NetworkSessionFailedError. Connection loss?";
                    return;
                }
                // TODO verify it !
                Q_EMIT failed(mReply->errorString() + QLatin1Char('\n') + errorStr(replyObject));
            } else {
                emitFailedMessage(reply->errorString(), replyObject);
                addLoggerWarning(QByteArrayLiteral("UploadFileJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
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

#include "moc_uploadfilejob.cpp"
