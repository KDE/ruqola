/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportsbyusersjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

#include <QUrlQuery>
using namespace Qt::Literals::StringLiterals;
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

bool ModerationReportsByUsersJob::hasQueryParameterSupport() const
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
    addStartRestApiInfo("ModerationReportsByUsersJob: Ask for moderation reportes by users"_ba);
    return true;
}

void ModerationReportsByUsersJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("ModerationReportsByUsersJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT moderationReportByUserDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("ModerationReportsByUsersJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

ModerationReportsByUsersJob::ModerationReportsByUsersInfo ModerationReportsByUsersJob::moderationReportsByUsersInfo() const
{
    return mModerationReportsByUsersInfo;
}

void ModerationReportsByUsersJob::setModerationReportsByUsersInfo(const ModerationReportsByUsersInfo &newModerationReportsByUsersInfo)
{
    mModerationReportsByUsersInfo = newModerationReportsByUsersInfo;
}

QNetworkRequest ModerationReportsByUsersJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ModerationReportsByUsers);

    QUrlQuery queryUrl;
    if (mModerationReportsByUsersInfo.isValid()) {
        queryUrl.addQueryItem(u"oldest"_s, mModerationReportsByUsersInfo.mOldest.toString(Qt::ISODate));
        queryUrl.addQueryItem(u"latest"_s, mModerationReportsByUsersInfo.mLatest.toString(Qt::ISODate));
        if (!mModerationReportsByUsersInfo.mSelector.isEmpty()) {
            queryUrl.addQueryItem(u"selector"_s, mModerationReportsByUsersInfo.mSelector);
        }
    }
    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);

    return request;
}

#include "moc_moderationreportsbyusersjob.cpp"
