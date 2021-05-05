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

#include "creategroupsjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
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
    addStartRestApiInfo("CreateGroupsJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &CreateGroupsJob::slotCreateGroupsFinished);
    return true;
}

void CreateGroupsJob::slotCreateGroupsFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("CreateGroupsJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT createGroupsDone(replyObject);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("CreateGroupsJob: problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

CreateRoomInfo CreateGroupsJob::createGroupsInfo() const
{
    return mCreateGroupInfo;
}

void CreateGroupsJob::setCreateGroupsInfo(const CreateRoomInfo &createGroupsInfo)
{
    mCreateGroupInfo = createGroupsInfo;
}

bool CreateGroupsJob::requireHttpAuthentication() const
{
    return true;
}

bool CreateGroupsJob::canStart() const
{
    if (!mCreateGroupInfo.canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "CreateGroupsJob: channelname is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument CreateGroupsJob::json() const
{
    return mCreateGroupInfo.json();
}

QNetworkRequest CreateGroupsJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsCreate);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}
