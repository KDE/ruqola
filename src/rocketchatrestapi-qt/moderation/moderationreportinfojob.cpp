/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportinfojob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>
using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
ModerationReportInfoJob::ModerationReportInfoJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ModerationReportInfoJob::~ModerationReportInfoJob() = default;

bool ModerationReportInfoJob::requireHttpAuthentication() const
{
    return true;
}

bool ModerationReportInfoJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start get report info job";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo("ModerationReportJob: get report info starting"_ba);
    return true;
}

void ModerationReportInfoJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("ModerationReportJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT moderationReportInfoDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("ModerationReportJob: Problem when we tried to get report info : "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString ModerationReportInfoJob::reportId() const
{
    return mReportId;
}

void ModerationReportInfoJob::setReportId(const QString &newReportId)
{
    mReportId = newReportId;
}

QNetworkRequest ModerationReportInfoJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ModerationReportInfo);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(u"reportId"_s, mReportId);
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);

    return request;
}

bool ModerationReportInfoJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mReportId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "mReportId is empty";
        return false;
    }
    return true;
}

#include "moc_moderationreportinfojob.cpp"
