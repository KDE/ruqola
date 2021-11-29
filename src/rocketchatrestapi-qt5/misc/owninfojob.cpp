/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "owninfojob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
OwnInfoJob::OwnInfoJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

OwnInfoJob::~OwnInfoJob() = default;

bool OwnInfoJob::requireHttpAuthentication() const
{
    return true;
}

bool OwnInfoJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start owninfo job";
        deleteLater();
        return false;
    }
    QNetworkReply *reply = submitGetRequest();
    connect(reply, &QNetworkReply::finished, this, &OwnInfoJob::slotOwnInfoFinished);
    addStartRestApiInfo(QByteArrayLiteral("OwnInfoJob: Ask info about me"));
    return true;
}

void OwnInfoJob::slotOwnInfoFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        // TODO check success !
        addLoggerInfo(QByteArrayLiteral("OwnInfoJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT ownInfoDone(replyObject);
        reply->deleteLater();
    }
    deleteLater();
}

QNetworkRequest OwnInfoJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::Me);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);

    return request;
}
