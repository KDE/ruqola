/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "changegroupsreadonlyjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ChangeGroupsReadonlyJob::ChangeGroupsReadonlyJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

ChangeGroupsReadonlyJob::~ChangeGroupsReadonlyJob() = default;

bool ChangeGroupsReadonlyJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChangeGroupsReadonlyJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &ChangeGroupsReadonlyJob::slotChangeReadonlyFinished);
    return true;
}

void ChangeGroupsReadonlyJob::slotChangeReadonlyFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("Change read only success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT changeReadonlyDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("Problem when we tried to change read only status: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

bool ChangeGroupsReadonlyJob::readOnly() const
{
    return mReadOnly;
}

void ChangeGroupsReadonlyJob::setReadOnly(bool readOnly)
{
    mReadOnly = readOnly;
}

bool ChangeGroupsReadonlyJob::requireHttpAuthentication() const
{
    return true;
}

bool ChangeGroupsReadonlyJob::canStart() const
{
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChangeGroupsReadonlyJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument ChangeGroupsReadonlyJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj);
    jsonObj[QLatin1String("readOnly")] = readOnly();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest ChangeGroupsReadonlyJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsSetReadOnly);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}
