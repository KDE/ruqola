/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "privateinfojob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
PrivateInfoJob::PrivateInfoJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

PrivateInfoJob::~PrivateInfoJob() = default;

bool PrivateInfoJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start PrivateInfoJob job";
        deleteLater();
        return false;
    }
    QNetworkReply *reply = submitGetRequest();
    addStartRestApiInfo(QByteArrayLiteral("PrivateInfoJob: Ask private info"));
    connect(reply, &QNetworkReply::finished, this, &PrivateInfoJob::slotPrivateInfoDone);

    return true;
}

void PrivateInfoJob::slotPrivateInfoDone()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("PrivateInfoJob done: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT privateInfoDone(replyObject);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("PrivateInfoJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

bool PrivateInfoJob::requireHttpAuthentication() const
{
    return true;
}

QNetworkRequest PrivateInfoJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::Settings);
    QNetworkRequest req(url);
    addAuthRawHeader(req);
    return req;
}
