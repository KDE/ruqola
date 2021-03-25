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

#include "changearchivationstatejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ChangeArchivationStateJob::ChangeArchivationStateJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ChangeArchivationStateJob::~ChangeArchivationStateJob()
{
}

bool ChangeArchivationStateJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChangeArchivationStateJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &ChangeArchivationStateJob::slotChangeArchivationStateFinished);
    return true;
}

void ChangeArchivationStateJob::slotChangeArchivationStateFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("ChangeArchivationStateJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT changeArchivationStateDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("ChangeArchivationStateJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QString ChangeArchivationStateJob::roomId() const
{
    return mRoomId;
}

void ChangeArchivationStateJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

bool ChangeArchivationStateJob::archive() const
{
    return mArchive;
}

void ChangeArchivationStateJob::setArchive(bool archive)
{
    mArchive = archive;
}

bool ChangeArchivationStateJob::requireHttpAuthentication() const
{
    return true;
}

bool ChangeArchivationStateJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RoomId is Empty. Impossible to start ChangeArchivationStateJob";
        return false;
    }
    return true;
}

QNetworkRequest ChangeArchivationStateJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsChangeArchivactionState);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument ChangeArchivationStateJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("rid")] = mRoomId;
    jsonObj[QLatin1String("action")] = mArchive ? QStringLiteral("archive") : QStringLiteral("unarchive");

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
