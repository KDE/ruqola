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

#include "archivegroupsjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ArchiveGroupsJob::ArchiveGroupsJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ArchiveGroupsJob::~ArchiveGroupsJob()
{
}

bool ArchiveGroupsJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ArchiveGroupsJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &ArchiveGroupsJob::slotArchiveGroupsFinished);
    return true;
}

void ArchiveGroupsJob::slotArchiveGroupsFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("ArchiveGroupsJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT archiveGroupsDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("ArchiveGroupsJob: problem: ") + replyJson.toJson(QJsonDocument::Indented));
            // FIXME report error
            Q_EMIT archiveGroupsError(QString());
        }
        reply->deleteLater();
    }
    deleteLater();
}

bool ArchiveGroupsJob::archive() const
{
    return mArchive;
}

void ArchiveGroupsJob::setArchive(bool archive)
{
    mArchive = archive;
}

bool ArchiveGroupsJob::requireHttpAuthentication() const
{
    return true;
}

bool ArchiveGroupsJob::canStart() const
{
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ArchiveGroupsJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument ArchiveGroupsJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = roomId();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString ArchiveGroupsJob::roomId() const
{
    return mRoomId;
}

void ArchiveGroupsJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QNetworkRequest ArchiveGroupsJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(mArchive ? RestApiUtil::RestApiUrlType::GroupsArchive : RestApiUtil::RestApiUrlType::GroupsUnarchive);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}
