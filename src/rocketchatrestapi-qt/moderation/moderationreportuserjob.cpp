/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportuserjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
ModerationReportUserJob::ModerationReportUserJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ModerationReportUserJob::~ModerationReportUserJob() = default;

bool ModerationReportUserJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ModerationReportUserJob::start");
    submitPostRequest(json());

    return true;
}

void ModerationReportUserJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo(QByteArrayLiteral("ModerationReportUserJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT moderationReportUserDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("ModerationReportUserJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString ModerationReportUserJob::description() const
{
    return mDescription;
}

void ModerationReportUserJob::setDescription(const QString &newDescription)
{
    mDescription = newDescription;
}

QByteArray ModerationReportUserJob::reportedUserId() const
{
    return mReportedUserId;
}

void ModerationReportUserJob::setReportedUserId(const QByteArray &newReportedUserId)
{
    mReportedUserId = newReportedUserId;
}

bool ModerationReportUserJob::requireHttpAuthentication() const
{
    return true;
}

bool ModerationReportUserJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mReportedUserId.isEmpty() || mDescription.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ModerationReportUserJob: mReportedUserId is empty or mDescription isEmpty()";
        return false;
    }
    return true;
}

QNetworkRequest ModerationReportUserJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ModerationReportUser);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument ModerationReportUserJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["userId"_L1] = QLatin1StringView(mReportedUserId);
    jsonObj["description"_L1] = mDescription;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_moderationreportuserjob.cpp"
