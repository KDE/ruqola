/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

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
    submitGetRequest();
    addStartRestApiInfo(QByteArrayLiteral("CustomSoundsJob: Ask custom sounds info"));

    return true;
}

void CustomSoundsListJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("CustomSoundsJob done: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT customSoundsListDone(replyObject); // TODO fix return value!
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("CustomSoundsJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
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
