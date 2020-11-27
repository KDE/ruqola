/*
   Copyright (c) 2019-2020 Laurent Montel <montel@kde.org>

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

#include "opendmjob.h"

#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
OpenDmJob::OpenDmJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

OpenDmJob::~OpenDmJob()
{
}

bool OpenDmJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("OpenDmJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &OpenDmJob::slotOpenDmFinished);
    return true;
}

void OpenDmJob::slotOpenDmFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("Create direct message success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT openDmDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("Create direct message Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QString OpenDmJob::directUserId() const
{
    return mDirectUserId;
}

void OpenDmJob::setDirectUserId(const QString &userId)
{
    mDirectUserId = userId;
}

bool OpenDmJob::requireHttpAuthentication() const
{
    return true;
}

bool OpenDmJob::canStart() const
{
    if (mDirectUserId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "OpenDmJob: mDirectUserId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument OpenDmJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("userId")] = mDirectUserId;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest OpenDmJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ImOpen);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}
