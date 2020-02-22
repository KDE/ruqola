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

#include "createdmjob.h"

#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
CreateDmJob::CreateDmJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

CreateDmJob::~CreateDmJob()
{
}

bool CreateDmJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);
    addLoggerInfo("CreateDmJob::start: " + baPostData);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    connect(reply, &QNetworkReply::finished, this, &CreateDmJob::slotCreateDmFinished);
    return true;
}

void CreateDmJob::slotCreateDmFinished()
{
    auto *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("Create direct message success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT createDmDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("Create direct message Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QString CreateDmJob::userName() const
{
    return mUserName;
}

void CreateDmJob::setUserName(const QString &userName)
{
    mUserName = userName;
}

bool CreateDmJob::requireHttpAuthentication() const
{
    return true;
}

bool CreateDmJob::canStart() const
{
    if (mUserName.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "CreateDmJob: username is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument CreateDmJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("username")] = mUserName;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest CreateDmJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ImCreate);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}
