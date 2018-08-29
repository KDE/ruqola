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

#include "createchanneljob.h"

#include "restapi_debug.h"
#include "restapimethod.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

CreateChannelJob::CreateChannelJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

CreateChannelJob::~CreateChannelJob()
{
}

bool CreateChannelJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);
    addLoggerInfo("CreateChannelJob::start: " + baPostData);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    connect(reply, &QNetworkReply::finished, this, &CreateChannelJob::slotCreateChannelFinished);
    return true;
}

void CreateChannelJob::slotCreateChannelFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            qCDebug(RESTAPI_LOG) << "Create channel success: " << data;
            Q_EMIT createChannelDone();
        } else {
            qCWarning(RESTAPI_LOG) <<" Problem when we tried to create channel" << data;
        }
    }
    deleteLater();
}

QStringList CreateChannelJob::members() const
{
    return mMembers;
}

void CreateChannelJob::setMembers(const QStringList &members)
{
    mMembers = members;
}

QString CreateChannelJob::channelName() const
{
    return mChannelName;
}

void CreateChannelJob::setChannelName(const QString &channelName)
{
    mChannelName = channelName;
}

bool CreateChannelJob::readOnly() const
{
    return mReadOnly;
}

void CreateChannelJob::setReadOnly(bool readOnly)
{
    mReadOnly = readOnly;
}

bool CreateChannelJob::requireHttpAuthentication() const
{
    return true;
}

bool CreateChannelJob::canStart() const
{
    if (mChannelName.isEmpty()) {
        qCWarning(RESTAPI_LOG) << "CreateChannelJob: channelname is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(RESTAPI_LOG) << "Impossible to start CreateChannelJob job";
        return false;
    }
    return true;
}

QJsonDocument CreateChannelJob::json() const
{
    QJsonObject jsonObj;
    if (!mMembers.isEmpty()) {
        jsonObj[QLatin1String("members")] = QJsonArray::fromStringList(mMembers);
    }
    jsonObj[QLatin1String("name")] = channelName();
    if (mReadOnly) {
        //Default is false
        jsonObj[QLatin1String("readOnly")] = true;
    }

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest CreateChannelJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsCreate);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    return request;
}
