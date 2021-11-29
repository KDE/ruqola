/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "customuserstatuslistjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>
using namespace RocketChatRestApi;
CustomUserStatusListJob::CustomUserStatusListJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

CustomUserStatusListJob::~CustomUserStatusListJob() = default;

bool CustomUserStatusListJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start CustomUserStatusJobList job";
        deleteLater();
        return false;
    }
    QNetworkReply *reply = submitGetRequest();
    addStartRestApiInfo(QByteArrayLiteral("CustomUserStatusJob: Ask custom user status info"));
    connect(reply, &QNetworkReply::finished, this, &CustomUserStatusListJob::slotCustomUserStatusDone);

    return true;
}

void CustomUserStatusListJob::slotCustomUserStatusDone()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("CustomUserStatusJob done: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT customUserStatusDone(replyObject);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("CustomUserStatusJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

bool CustomUserStatusListJob::requireHttpAuthentication() const
{
    return true;
}

QNetworkRequest CustomUserStatusListJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::CustomUserStatusList);
    QUrlQuery queryUrl;
    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);
    QNetworkRequest req(url);
    addAuthRawHeader(req);
    return req;
}

bool CustomUserStatusListJob::hasQueryParameterSupport() const
{
    return true;
}
