/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationdismissuserreportsjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

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

bool ModerationDismissUserReportsJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mModerationReportedUserId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ModerationDismissUserReportsJob: mModerationReportedUserId is empty";
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
    addStartRestApiInfo("ModerationDismissUserReportsJob::start"_ba);
    submitPostRequest(json());
    return true;
}

void ModerationDismissUserReportsJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    if (const auto replyObject = checkResponse("ModerationDismissUserReportsJob"_ba, replyErrorString, replyJson)) {
        Q_EMIT moderationDismissReportedUserDone(*replyObject);
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
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ModerationDismissUserReports);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument ModerationDismissUserReportsJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["userId"_L1] = QLatin1StringView(mModerationReportedUserId);
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_moderationdismissuserreportsjob.cpp"
