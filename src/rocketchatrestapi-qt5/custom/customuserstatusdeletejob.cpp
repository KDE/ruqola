/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#include "customuserstatusdeletejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
CustomUserStatusDeleteJob::CustomUserStatusDeleteJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

CustomUserStatusDeleteJob::~CustomUserStatusDeleteJob()
{
}

bool CustomUserStatusDeleteJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("CustomUserStatusDeleteJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &CustomUserStatusDeleteJob::slotUserStatusDelete);
    return true;
}

void CustomUserStatusDeleteJob::slotUserStatusDelete()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("CustomUserStatusDeleteJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT userStatusDeletedDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("CustomUserStatusDeleteJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QString CustomUserStatusDeleteJob::customUserStatusId() const
{
    return mCustomUserStatusId;
}

void CustomUserStatusDeleteJob::setCustomUserStatusId(const QString &customUserStatusId)
{
    mCustomUserStatusId = customUserStatusId;
}

bool CustomUserStatusDeleteJob::requireHttpAuthentication() const
{
    return true;
}

bool CustomUserStatusDeleteJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mCustomUserStatusId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "CustomUserStatusDeleteJob: mCustomUserStatusId is empty.";
        return false;
    }
    return true;
}

QNetworkRequest CustomUserStatusDeleteJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::CustomUserStatusDelete);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument CustomUserStatusDeleteJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QStringLiteral("customUserStatusId")] = mCustomUserStatusId;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
