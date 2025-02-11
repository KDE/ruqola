/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "acceptsuggestedgroupkeyjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"

#include <QJsonDocument>
#include <QJsonObject>
using namespace RocketChatRestApi;
AcceptSuggestedGroupKeyJob::AcceptSuggestedGroupKeyJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

AcceptSuggestedGroupKeyJob::~AcceptSuggestedGroupKeyJob() = default;

bool AcceptSuggestedGroupKeyJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("AcceptSuggestedGroupKeyJob::start");
    submitPostRequest(json());
    return true;
}

void AcceptSuggestedGroupKeyJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo(QByteArrayLiteral("AcceptSuggestedGroupKeyJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT resetE2eKeyDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("AcceptSuggestedGroupKeyJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool AcceptSuggestedGroupKeyJob::requireHttpAuthentication() const
{
    return true;
}

QNetworkRequest AcceptSuggestedGroupKeyJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::E2EResetOwnE2EKey);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument AcceptSuggestedGroupKeyJob::json() const
{
    QJsonObject jsonObj;
    // TODO
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_acceptsuggestedgroupkeyjob.cpp"
