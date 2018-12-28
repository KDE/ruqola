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

#include "creategroupsjob.h"

#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
CreateGroupsJob::CreateGroupsJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

CreateGroupsJob::~CreateGroupsJob()
{
}

bool CreateGroupsJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);
    addLoggerInfo("CreateGroupsJob::start: " + baPostData);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    connect(reply, &QNetworkReply::finished, this, &CreateGroupsJob::slotCreateGroupsFinished);
    return true;
}

void CreateGroupsJob::slotCreateGroupsFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("CreateGroupsJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT createGroupsDone();
        } else {
            addLoggerWarning(QByteArrayLiteral("CreateGroupsJob: problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
    }
    deleteLater();
}

QStringList CreateGroupsJob::members() const
{
    return mMembers;
}

void CreateGroupsJob::setMembers(const QStringList &members)
{
    mMembers = members;
}

QString CreateGroupsJob::channelName() const
{
    return mChannelName;
}

void CreateGroupsJob::setChannelName(const QString &channelName)
{
    mChannelName = channelName;
}

bool CreateGroupsJob::readOnly() const
{
    return mReadOnly;
}

void CreateGroupsJob::setReadOnly(bool readOnly)
{
    mReadOnly = readOnly;
}

bool CreateGroupsJob::requireHttpAuthentication() const
{
    return true;
}

bool CreateGroupsJob::canStart() const
{
    if (mChannelName.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "CreateGroupsJob: channelname is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start CreateGroupsJob job";
        return false;
    }
    return true;
}

QJsonDocument CreateGroupsJob::json() const
{
    QJsonObject jsonObj;
    if (!mMembers.isEmpty()) {
        jsonObj[QLatin1String("members")] = QJsonArray::fromStringList(mMembers);
    }
    jsonObj[QLatin1String("name")] = channelName();
    if (mReadOnly) {
        jsonObj[QLatin1String("readOnly")] = true;
    } //Default is false

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest CreateGroupsJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsCreate);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    return request;
}
