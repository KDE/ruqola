/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportsbyusersjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ModerationReportsByUsersJob::ModerationReportsByUsersJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ModerationReportsByUsersJob::~ModerationReportsByUsersJob() = default;

bool ModerationReportsByUsersJob::requireHttpAuthentication() const
{
    return true;
}

bool ModerationReportsByUsersJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ModerationReportsByUsersJob";
        deleteLater();
        return false;
    }
    submitGetRequest();
    addStartRestApiInfo(QByteArrayLiteral("ModerationReportsByUsersJob: Ask for moderation message"));
    return true;
}

void ModerationReportsByUsersJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("ModerationReportsByUsersJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT moderationReportByUserDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("ModerationReportsByUsersJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QNetworkRequest ModerationReportsByUsersJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ModerationReportsByUsers);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);

    return request;
}

#include "moc_moderationreportsbyusersjob.cpp"
