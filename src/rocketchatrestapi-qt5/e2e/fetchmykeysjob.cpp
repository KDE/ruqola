/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "fetchmykeysjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
FetchMyKeysJob::FetchMyKeysJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

FetchMyKeysJob::~FetchMyKeysJob() = default;

bool FetchMyKeysJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

bool FetchMyKeysJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    QNetworkReply *reply = submitGetRequest();
    connect(reply, &QNetworkReply::finished, this, &FetchMyKeysJob::slotFetchMyKeys);
    addStartRestApiInfo("Start FetchMyKeysJob");

    return true;
}

void FetchMyKeysJob::slotFetchMyKeys()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("FetchMyKeysJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT fetchMyKeysDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("FetchMyKeysJob: problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QNetworkRequest FetchMyKeysJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::E2EfetchMyKeys);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    return request;
}

bool FetchMyKeysJob::requireHttpAuthentication() const
{
    return true;
}
