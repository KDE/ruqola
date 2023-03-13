/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "archivegroupsjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ArchiveGroupsJob::ArchiveGroupsJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

ArchiveGroupsJob::~ArchiveGroupsJob() = default;

bool ArchiveGroupsJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ArchiveGroupsJob::start");
    submitPostRequest(json());

    return true;
}

void ArchiveGroupsJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("ArchiveGroupsJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT archiveGroupsDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("ArchiveGroupsJob: problem: ") + replyJson.toJson(QJsonDocument::Indented));
        // FIXME report error
        Q_EMIT archiveGroupsError(QString());
    }
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
    if (!hasIdentifier()) {
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
    generateJson(jsonObj);

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest ArchiveGroupsJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(mArchive ? RestApiUtil::RestApiUrlType::GroupsArchive : RestApiUtil::RestApiUrlType::GroupsUnarchive);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}
