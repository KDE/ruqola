/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "changegroupsdescriptionjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ChangeGroupsDescriptionJob::ChangeGroupsDescriptionJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

ChangeGroupsDescriptionJob::~ChangeGroupsDescriptionJob() = default;

bool ChangeGroupsDescriptionJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChangeGroupsDescriptionJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &ChangeGroupsDescriptionJob::slotChangeDescriptionFinished);
    return true;
}

void ChangeGroupsDescriptionJob::slotChangeDescriptionFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("ChangeGroupsDescriptionJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT changeDescriptionDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("ChangeGroupsDescriptionJob: problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QString ChangeGroupsDescriptionJob::description() const
{
    return mDescription;
}

void ChangeGroupsDescriptionJob::setDescription(const QString &description)
{
    mDescription = description;
}

bool ChangeGroupsDescriptionJob::requireHttpAuthentication() const
{
    return true;
}

bool ChangeGroupsDescriptionJob::canStart() const
{
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChangeGroupsDescriptionJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument ChangeGroupsDescriptionJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj), jsonObj[QLatin1String("description")] = description();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest ChangeGroupsDescriptionJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsSetDescription);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}
