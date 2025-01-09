/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationuserreportsjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>
using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
ModerationUserReportsJob::ModerationUserReportsJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ModerationUserReportsJob::~ModerationUserReportsJob() = default;

bool ModerationUserReportsJob::requireHttpAuthentication() const
{
    return true;
}

bool ModerationUserReportsJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start get get user reported message job";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo(QByteArrayLiteral("ModerationReportJob: get get user reported message starting"));
    return true;
}

void ModerationUserReportsJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo(QByteArrayLiteral("ModerationReportJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT moderationUserReportJobDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("ModerationReportJob: Problem when we tried to getmoderation user report : ")
                         + replyJson.toJson(QJsonDocument::Indented));
    }
}

ModerationUserReportsJob::ModerationUserReportsInfo ModerationUserReportsJob::moderationUserReportsInfo() const
{
    return mModerationUserReportsInfo;
}

void ModerationUserReportsJob::setModerationUserReportsInfo(const ModerationUserReportsInfo &newModerationUserReportsInfo)
{
    mModerationUserReportsInfo = newModerationUserReportsInfo;
}

QNetworkRequest ModerationUserReportsJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ModerationUserReports);

    QUrlQuery queryUrl;
    if (mModerationUserReportsInfo.isValid()) {
        queryUrl.addQueryItem(QStringLiteral("oldest"), mModerationUserReportsInfo.mOldest.toString(Qt::ISODate));
        queryUrl.addQueryItem(QStringLiteral("latest"), mModerationUserReportsInfo.mLatest.toString(Qt::ISODate));
        if (!mModerationUserReportsInfo.mSelector.isEmpty()) {
            queryUrl.addQueryItem(QStringLiteral("selector"), mModerationUserReportsInfo.mSelector);
        }
    }
    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);

    return request;
}

#include "moc_moderationuserreportsjob.cpp"
