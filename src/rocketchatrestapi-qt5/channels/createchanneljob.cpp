/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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

#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
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
            addLoggerInfo(QByteArrayLiteral("CreateChannelJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT createChannelDone();
            if (!mPassword.isEmpty()) {
                const QJsonObject channelObj = replyObject[QStringLiteral("channel")].toObject();
                const QString channelId = channelObj[QStringLiteral("_id")].toString();
                if (channelId.isEmpty()) {
                    emitFailedMessage(replyObject);
                    addLoggerWarning(QByteArrayLiteral("CreateChannelJob Impossible to extract channel id: ") + replyJson.toJson(QJsonDocument::Indented));
                } else {
                    Q_EMIT addJoinCodeToChannel(channelId, mPassword);
                }
            }
        } else {
            emitFailedMessage(replyObject);
            addLoggerWarning(QByteArrayLiteral("CreateChannelJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QString CreateChannelJob::password() const
{
    return mPassword;
}

void CreateChannelJob::setPassword(const QString &password)
{
    mPassword = password;
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
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "CreateChannelJob: channelname is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start CreateChannelJob job";
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
    addRequestAttribute(request);
    return request;
}
