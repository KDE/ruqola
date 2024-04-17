/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationdismissuserreportsjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>
using namespace RocketChatRestApi;
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
    addStartRestApiInfo(QByteArrayLiteral("ModerationDismissUserReportsJob: Ask for moderation reportes by users"));
    return true;
}

void ModerationDismissUserReportsJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo(QByteArrayLiteral("ModerationDismissUserReportsJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT moderationDismissReportedUserDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("ModerationDismissUserReportsJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
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
    queryUrl.addQueryItem(QStringLiteral("userId"), QString::fromLatin1(mModerationReportedUserId));
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);

    return request;
}

#include "moc_moderationdismissuserreportsjob.cpp"
