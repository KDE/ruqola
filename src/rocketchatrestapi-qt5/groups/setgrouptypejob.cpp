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

#include "setgrouptypejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
SetGroupTypeJob::SetGroupTypeJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

SetGroupTypeJob::~SetGroupTypeJob() = default;

bool SetGroupTypeJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("SetGroupTypeJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &SetGroupTypeJob::slotSetGroupTypeFinished);
    return true;
}

void SetGroupTypeJob::slotSetGroupTypeFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("SetGroupTypeJob: Success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT setGroupTypeDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("SetGroupTypeJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

SetGroupTypeJob::GroupType SetGroupTypeJob::type() const
{
    return mType;
}

void SetGroupTypeJob::setType(GroupType type)
{
    mType = type;
}


bool SetGroupTypeJob::requireHttpAuthentication() const
{
    return true;
}

bool SetGroupTypeJob::canStart() const
{
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SetGroupTypeJob: mRoomId is empty";
        return false;
    }
    if (mType == Unknown) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SetGroupTypeJob: type is not defined";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument SetGroupTypeJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj);
    switch (mType) {
    case Public:
        jsonObj[QLatin1String("type")] = QStringLiteral("c");
        break;
    case Private:
        jsonObj[QLatin1String("type")] = QStringLiteral("p");
        break;
    case Unknown:
        break;
    }

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest SetGroupTypeJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsSetType);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}
