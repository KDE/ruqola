/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customsoundslistjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>
using namespace RocketChatRestApi;
CustomSoundsListJob::CustomSoundsListJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

CustomSoundsListJob::~CustomSoundsListJob() = default;

bool CustomSoundsListJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start CustomSoundsJob job";
        deleteLater();
        return false;
    }
    QNetworkReply *reply = submitGetRequest();
    addStartRestApiInfo(QByteArrayLiteral("CustomSoundsJob: Ask custom sounds info"));
    connect(reply, &QNetworkReply::finished, this, &CustomSoundsListJob::slotCustomSoundsDone);

    return true;
}

void CustomSoundsListJob::slotCustomSoundsDone()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("CustomSoundsJob done: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT customSoundsListDone(replyObject); // TODO fix return value!
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("CustomSoundsJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

bool CustomSoundsListJob::requireHttpAuthentication() const
{
    return true;
}

QNetworkRequest CustomSoundsListJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::CustomSoundsList);

    QUrlQuery queryUrl;
    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);
    QNetworkRequest req(url);
    addAuthRawHeader(req);
    return req;
}

bool CustomSoundsListJob::hasQueryParameterSupport() const
{
    return true;
}
