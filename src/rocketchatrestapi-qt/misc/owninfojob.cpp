/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

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
    submitGetRequest();

    addStartRestApiInfo(QByteArrayLiteral("OwnInfoJob: Ask info about me"));
    return true;
}

void OwnInfoJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QLatin1String("status")].toString() == QLatin1String("error")) {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("OwnInfoJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    } else {
        addLoggerInfo(QByteArrayLiteral("OwnInfoJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT ownInfoDone(replyObject);
    }
}

QNetworkRequest OwnInfoJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::Me);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);

    return request;
}

#include "moc_owninfojob.cpp"
