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

#include "findorcreateinvitejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
FindOrCreateInviteJob::FindOrCreateInviteJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

FindOrCreateInviteJob::~FindOrCreateInviteJob()
{
}

bool FindOrCreateInviteJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);
    addLoggerInfo("FindOrCreateInviteJob::start: " + baPostData);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    connect(reply, &QNetworkReply::finished, this, &FindOrCreateInviteJob::slotFindOrCreateInviteFinished);
    return true;
}

void FindOrCreateInviteJob::slotFindOrCreateInviteFinished()
{
    auto *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("FindOrCreateInviteJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            //TODO parse element
            qDebug()<< "replyObject " << replyObject;
            Q_EMIT findOrCreateInviteDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("FindOrCreateInviteJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

int FindOrCreateInviteJob::numberOfDays() const
{
    return mNumberOfDays;
}

void FindOrCreateInviteJob::setNumberOfDays(int numberOfDays)
{
    mNumberOfDays = numberOfDays;
}

int FindOrCreateInviteJob::maxUses() const
{
    return mMaxUses;
}

void FindOrCreateInviteJob::setMaxUses(int maxUses)
{
    mMaxUses = maxUses;
}

QString FindOrCreateInviteJob::roomId() const
{
    return mRoomId;
}

void FindOrCreateInviteJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

bool FindOrCreateInviteJob::requireHttpAuthentication() const
{
    return true;
}

bool FindOrCreateInviteJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RoomId is Empty. Impossible to start FindOrCreateInviteJob";
        return false;
    }
    if (mMaxUses < 0) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "MaxUses is negative it's a bug. Impossible to start FindOrCreateInviteJob";
        return false;
    }
    if (mNumberOfDays < 0) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Number of days is negative. Impossible to start FindOrCreateInviteJob";
        return false;
    }
    return true;
}

QNetworkRequest FindOrCreateInviteJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::FindOrCreateInvite);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument FindOrCreateInviteJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("rid")] = mRoomId;
    jsonObj[QLatin1String("days")] = mNumberOfDays;
    jsonObj[QLatin1String("maxUses")] = mMaxUses;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
