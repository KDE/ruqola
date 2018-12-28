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

#include "channellistjob.h"
#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ChannelListJob::ChannelListJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ChannelListJob::~ChannelListJob()
{
}

bool ChannelListJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start channel list job";
        deleteLater();
        return false;
    }

    QNetworkReply *reply = mNetworkAccessManager->get(request());
    addLoggerInfo(QByteArrayLiteral("ChannelListJob: ask channel list"));
    connect(reply, &QNetworkReply::finished, this, &ChannelListJob::slotListInfo);
    return false;
}

void ChannelListJob::slotListInfo()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();
        addLoggerInfo(QByteArrayLiteral("ChannelListJob: finished: ") + replyJson.toJson(QJsonDocument::Indented));
        //qCDebug(ROCKETCHATQTRESTAPI_LOG) << "RestApiRequest::parseGetAvatar: " << data << " userId "<<userId;
        Q_EMIT channelListDone(replyObject);
    }
    deleteLater();
}

QNetworkRequest ChannelListJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsList);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    return request;
}

bool ChannelListJob::requireHttpAuthentication() const
{
    return true;
}
