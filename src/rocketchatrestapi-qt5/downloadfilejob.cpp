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

#include "downloadfilejob.h"
#include "restapimethod.h"
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

DownloadFileJob::~DownloadFileJob()
{
}

bool DownloadFileJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }

    QNetworkReply *reply = submitGetRequest();
    addStartRestApiInfo("ChannelListJob: url:" + mUrl.toEncoded() + " mimetype " + mMimeType.toLatin1() + " saveAs " + mLocalFileUrl.toEncoded());
    connect(reply, &QNetworkReply::finished, this, &DownloadFileJob::slotDownloadDone);
    return true;
}

void DownloadFileJob::slotDownloadDone()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
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
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    req.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
#else
    req.setAttribute(QNetworkRequest::Http2AllowedAttribute, true);
#endif
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
