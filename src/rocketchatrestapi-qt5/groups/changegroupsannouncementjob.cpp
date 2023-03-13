/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "changegroupsannouncementjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ChangeGroupsAnnouncementJob::ChangeGroupsAnnouncementJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

ChangeGroupsAnnouncementJob::~ChangeGroupsAnnouncementJob() = default;

bool ChangeGroupsAnnouncementJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChangeGroupsAnnouncementJob::start");
    submitPostRequest(json());

    return true;
}

void ChangeGroupsAnnouncementJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("ChangeGroupsDescriptionJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT changeGroupsAnnouncement();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("ChangeGroupsDescriptionJob: problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool ChangeGroupsAnnouncementJob::requireHttpAuthentication() const
{
    return true;
}

bool ChangeGroupsAnnouncementJob::canStart() const
{
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChangeGroupsAnnouncementJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument ChangeGroupsAnnouncementJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj);
    jsonObj[QLatin1String("announcement")] = announcement();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString ChangeGroupsAnnouncementJob::announcement() const
{
    return mAnnouncement;
}

void ChangeGroupsAnnouncementJob::setAnnouncement(const QString &t)
{
    mAnnouncement = t;
}

QNetworkRequest ChangeGroupsAnnouncementJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsSetAnnouncement);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}
