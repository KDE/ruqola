/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
    submitGetRequest();
    addStartRestApiInfo(QByteArrayLiteral("PrivateInfoJob: Ask private info"));

    return true;
}

void PrivateInfoJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("PrivateInfoJob done: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT privateInfoDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("PrivateInfoJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
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

#include "moc_privateinfojob.cpp"
