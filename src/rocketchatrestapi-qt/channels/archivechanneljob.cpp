/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "archivechanneljob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
ArchiveChannelJob::ArchiveChannelJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ArchiveChannelJob::~ArchiveChannelJob() = default;

bool ArchiveChannelJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ArchiveChannelJob::start"_ba);
    submitPostRequest(json());
    return true;
}

void ArchiveChannelJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    if (checkResponse("ArchiveChannelJob"_ba, replyErrorString, replyJson)) {
        Q_EMIT archiveChannelDone();
    }
}

bool ArchiveChannelJob::archive() const
{
    return mArchive;
}

void ArchiveChannelJob::setArchive(bool archive)
{
    mArchive = archive;
}

bool ArchiveChannelJob::requireHttpAuthentication() const
{
    return true;
}

bool ArchiveChannelJob::canStart() const
{
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ArchiveChannelJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument ArchiveChannelJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["roomId"_L1] = roomId();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString ArchiveChannelJob::roomId() const
{
    return mRoomId;
}

void ArchiveChannelJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QNetworkRequest ArchiveChannelJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(mArchive ? RestApiUtil::RestApiUrlType::ChannelsArchive : RestApiUtil::RestApiUrlType::ChannelsUnarchive);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_archivechanneljob.cpp"
