/*
   SPDX-FileCopyrightText: 2018-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "downloadfilejob.h"
#include "rocketchatqtrestapi_debug.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QNetworkReply>

using namespace RocketChatRestApi;

DownloadFileJob::DownloadFileJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

DownloadFileJob::~DownloadFileJob() = default;

bool DownloadFileJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }

    mReply = networkAccessManager()->get(request());
    addStartRestApiInfo("DownloadFileJob: url:" + mUrl.toEncoded() + " mimetype " + mMimeType.toLatin1() + " saveAs " + mLocalFileUrl.toEncoded());
    connect(mReply.data(), &QNetworkReply::finished, this, &DownloadFileJob::slotDownloadDone);
    return true;
}

void DownloadFileJob::slotDownloadDone()
{
    auto reply = mReply;
    if (reply) {
        const QByteArray data = reply->readAll();
        const int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (status == 200) {
            addLoggerInfo("DownloadFileJob::slotDownloadDone finished");

            // Split between image/video/audio
            const QString newFilePath = mLocalFileUrl.toLocalFile();
            QFileInfo(newFilePath).absoluteDir().mkpath(QStringLiteral("."));
            QFile file(newFilePath);
            if (file.open(QIODevice::ReadWrite)) {
                file.write(data);
                file.close();
            } else {
                qCWarning(ROCKETCHATQTRESTAPI_LOG) << " Error !" << file.errorString();
            }
            Q_EMIT downloadFileDone(reply->url(), mLocalFileUrl);
        } else {
            // FIXME
            // emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("DownloadFileJob problem data: [") + data + "] :END");
        }
        reply->deleteLater();
    }
    deleteLater();
}

QNetworkRequest DownloadFileJob::request() const
{
    QNetworkRequest req(mUrl);
    addAuthRawHeader(req);
    req.setHeader(QNetworkRequest::ContentTypeHeader, mMimeType);
    req.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    req.setAttribute(QNetworkRequest::Http2AllowedAttribute, true);
    return req;
}

QUrl DownloadFileJob::localFileUrl() const
{
    return mLocalFileUrl;
}

void DownloadFileJob::setLocalFileUrl(const QUrl &localFileUrl)
{
    mLocalFileUrl = localFileUrl;
}

bool DownloadFileJob::requireHttpAuthentication() const
{
    return true;
}

bool DownloadFileJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!mUrl.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "DownloadFileJob: url is not valid: " << mUrl.toString();
        return false;
    }
    return true;
}

QUrl DownloadFileJob::url() const
{
    return mUrl;
}

void DownloadFileJob::setUrl(const QUrl &url)
{
    mUrl = url;
}

QString DownloadFileJob::mimeType() const
{
    return mMimeType;
}

void DownloadFileJob::setMimeType(const QString &mimeType)
{
    mMimeType = mimeType;
}
