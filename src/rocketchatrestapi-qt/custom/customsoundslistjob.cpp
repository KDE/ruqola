/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customsoundslistjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

#include <QUrlQuery>
using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
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
    submitGetRequest();
    addStartRestApiInfo("CustomSoundsJob: Ask custom sounds info"_ba);

    return true;
}

void CustomSoundsListJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("CustomSoundsJob done: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT customSoundsListDone(replyObject); // TODO fix return value!
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("CustomSoundsJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
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

#include "moc_customsoundslistjob.cpp"
