/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationdismissuserreportsjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>
using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
ModerationDismissUserReportsJob::ModerationDismissUserReportsJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ModerationDismissUserReportsJob::~ModerationDismissUserReportsJob() = default;

bool ModerationDismissUserReportsJob::requireHttpAuthentication() const
{
    return true;
}

bool ModerationDismissUserReportsJob::hasQueryParameterSupport() const
{
    return true;
}

bool ModerationDismissUserReportsJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mModerationReportedUserId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "mModerationUserId is empty";
        return false;
    }
    return true;
}

bool ModerationDismissUserReportsJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ModerationDismissUserReportsJob";
        deleteLater();
        return false;
    }
    submitGetRequest();
    addStartRestApiInfo("ModerationDismissUserReportsJob: Ask for moderation reportes by users"_ba);
    return true;
}

void ModerationDismissUserReportsJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("ModerationDismissUserReportsJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT moderationDismissReportedUserDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("ModerationDismissUserReportsJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QByteArray ModerationDismissUserReportsJob::moderationReportedUserId() const
{
    return mModerationReportedUserId;
}

void ModerationDismissUserReportsJob::setModerationReportedUserId(const QByteArray &newModerationReportedUserId)
{
    mModerationReportedUserId = newModerationReportedUserId;
}

QNetworkRequest ModerationDismissUserReportsJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ModerationDismissUserReports);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(u"userId"_s, QString::fromLatin1(mModerationReportedUserId));
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);

    return request;
}

#include "moc_moderationdismissuserreportsjob.cpp"
