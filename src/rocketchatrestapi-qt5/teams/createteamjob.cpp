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

#include "createteamjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
CreateTeamJob::CreateTeamJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

CreateTeamJob::~CreateTeamJob()
{
}

bool CreateTeamJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("CreateTeamJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &CreateTeamJob::slotCreateChannelFinished);
    return true;
}

void CreateTeamJob::slotCreateChannelFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("CreateTeamJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT createChannelDone();
            if (!mPassword.isEmpty()) {
                const QJsonObject channelObj = replyObject[QStringLiteral("channel")].toObject();
                const QString channelId = channelObj[QStringLiteral("_id")].toString();
                if (channelId.isEmpty()) {
                    emitFailedMessage(replyObject, reply);
                    addLoggerWarning(QByteArrayLiteral("CreateTeamJob Impossible to extract channel id: ") + replyJson.toJson(QJsonDocument::Indented));
                } else {
                    Q_EMIT addJoinCodeToChannel(channelId, mPassword);
                }
            }
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("CreateTeamJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QString CreateTeamJob::password() const
{
    return mPassword;
}

void CreateTeamJob::setPassword(const QString &password)
{
    mPassword = password;
}

QString CreateTeamJob::errorMessage(const QString &str, const QJsonObject &detail)
{
    if (str == QLatin1String("error-duplicate-channel-name")) {
        return i18n("A channel with name '%1' exists", detail.value(QStringLiteral("channel_name")).toString());
    }
    return RestApiAbstractJob::errorMessage(str, detail);
}

QStringList CreateTeamJob::members() const
{
    return mMembers;
}

void CreateTeamJob::setMembers(const QStringList &members)
{
    mMembers = members;
}

QString CreateTeamJob::channelName() const
{
    return mChannelName;
}

void CreateTeamJob::setChannelName(const QString &channelName)
{
    mChannelName = channelName;
}

bool CreateTeamJob::readOnly() const
{
    return mReadOnly;
}

void CreateTeamJob::setReadOnly(bool readOnly)
{
    mReadOnly = readOnly;
}

bool CreateTeamJob::requireHttpAuthentication() const
{
    return true;
}

bool CreateTeamJob::canStart() const
{
    if (mChannelName.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "CreateTeamJob: channelname is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument CreateTeamJob::json() const
{
    QJsonObject jsonObj;
    if (!mMembers.isEmpty()) {
        jsonObj[QLatin1String("members")] = QJsonArray::fromStringList(mMembers);
    }
    jsonObj[QLatin1String("name")] = channelName();
    if (mReadOnly) {
        // Default is false
        jsonObj[QLatin1String("readOnly")] = true;
    }

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest CreateTeamJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsCreate);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}
