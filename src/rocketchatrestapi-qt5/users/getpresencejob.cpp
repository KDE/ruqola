/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "getpresencejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
GetPresenceJob::GetPresenceJob(QObject *parent)
    : UserBaseJob(parent)
{
}

GetPresenceJob::~GetPresenceJob() = default;

bool GetPresenceJob::canStart() const
{
    if (!hasUserIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GetPresenceJob: identifier is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

bool GetPresenceJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    QNetworkReply *reply = submitGetRequest();
    connect(reply, &QNetworkReply::finished, this, &GetPresenceJob::slotGetPresenceUserId);
    addStartRestApiInfo("GetPresenceJob ask for presenceUserId: " + mUserInfo.userIdentifier.toLatin1());

    return true;
}

void GetPresenceJob::slotGetPresenceUserId()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("GetPresenceJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT getPresenceDone(replyObject[QStringLiteral("presence")].toString());
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("GetPresenceJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QNetworkRequest GetPresenceJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersGetPresence);
    addQueryUrl(url);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    return request;
}

bool GetPresenceJob::requireHttpAuthentication() const
{
    return true;
}
