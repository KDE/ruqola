/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

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
#include "restapi_debug.h"
#include "restapimethod.h"
#include <QNetworkReply>

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

    QNetworkReply *reply = mNetworkAccessManager->get(request());
    addLoggerInfo("ChannelListJob: url:"
                  +mUrl.toEncoded()
                  +" mimetype " + mMimeType.toLatin1()
                  +" saveAs " + mLocalFileUrl.toEncoded()
                  +" store in cache " + (mStoreInCache ? "true" : "false"));
    connect(reply, &QNetworkReply::finished, this, &DownloadFileJob::slotDownloadDone);
    return true;
}

void DownloadFileJob::slotDownloadDone()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (status == 200) {
            addLoggerInfo("DownloadFileJob::slotDownloadDone finished");
            Q_EMIT downloadFileDone(data, reply->url(), mStoreInCache, mLocalFileUrl);
        } else {
            qCWarning(RESTAPI_LOG) << "Unable to download " << reply->url();
        }
    }
    deleteLater();
}

QNetworkRequest DownloadFileJob::request() const
{
    QNetworkRequest req(mUrl);
    addAuthRawHeader(req);
    req.setHeader(QNetworkRequest::ContentTypeHeader, mMimeType);
    req.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    req.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    return req;
}

bool DownloadFileJob::storeInCache() const
{
    return mStoreInCache;
}

void DownloadFileJob::setStoreInCache(bool storeInCache)
{
    mStoreInCache = storeInCache;
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
        qCWarning(RESTAPI_LOG) << "Impossible to start DownloadFileJob";
        return false;
    }
    if (!mUrl.isValid()) {
        qCWarning(RESTAPI_LOG) << "DownloadFileJob: url is not valid";
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
