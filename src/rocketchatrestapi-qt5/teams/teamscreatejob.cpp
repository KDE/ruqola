/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#include "teamscreatejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
TeamsCreateJob::TeamsCreateJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

TeamsCreateJob::~TeamsCreateJob()
{
}

bool TeamsCreateJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("TeamsCreateJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &TeamsCreateJob::slotTeamCreateFinished);
    return true;
}

void TeamsCreateJob::slotTeamCreateFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("TeamsCreateJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT teamCreateDone();
            if (!mTeamsCreateJobInfo.password.isEmpty()) {
                const QJsonObject channelObj = replyObject[QStringLiteral("channel")].toObject();
                const QString channelId = channelObj[QStringLiteral("_id")].toString();
                if (channelId.isEmpty()) {
                    emitFailedMessage(replyObject, reply);
                    addLoggerWarning(QByteArrayLiteral("TeamsCreateJob Impossible to extract channel id: ") + replyJson.toJson(QJsonDocument::Indented));
                } else {
                    Q_EMIT addJoinCodeToChannel(channelId, mTeamsCreateJobInfo.password);
                }
            }
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("TeamsCreateJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

CreateRoomInfo TeamsCreateJob::teamsCreateJobInfo() const
{
    return mTeamsCreateJobInfo;
}

void TeamsCreateJob::setTeamsCreateJobInfo(const CreateRoomInfo &teamsCreateJobInfo)
{
    mTeamsCreateJobInfo = teamsCreateJobInfo;
}

bool TeamsCreateJob::requireHttpAuthentication() const
{
    return true;
}

bool TeamsCreateJob::canStart() const
{
    if (mTeamsCreateJobInfo.name.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "TeamsCreateJob: channelname is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument TeamsCreateJob::json() const
{
    return mTeamsCreateJobInfo.json();
}

QNetworkRequest TeamsCreateJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::TeamsCreate);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}
