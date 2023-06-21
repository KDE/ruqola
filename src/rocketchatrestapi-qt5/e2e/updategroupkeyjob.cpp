/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "updategroupkeyjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
UpdateGroupKeyJob::UpdateGroupKeyJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

UpdateGroupKeyJob::~UpdateGroupKeyJob() = default;

bool UpdateGroupKeyJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("UpdateGroupKeyJob::start");
    submitPostRequest(json());

    return true;
}

void UpdateGroupKeyJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("UpdateGroupKeyJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT updateGroupKeyDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("UpdateGroupKeyJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

const UpdateGroupKeyJob::UpdateGroupKeyInfo &UpdateGroupKeyJob::updateGroupInfo() const
{
    return mUpdateGroupInfo;
}

void UpdateGroupKeyJob::setUpdateGroupInfo(const UpdateGroupKeyInfo &newUpdateGroupInfo)
{
    mUpdateGroupInfo = newUpdateGroupInfo;
}

bool UpdateGroupKeyJob::requireHttpAuthentication() const
{
    return true;
}

bool UpdateGroupKeyJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!mUpdateGroupInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << " Group Info is invalid";
        return false;
    }
    return true;
}

QNetworkRequest UpdateGroupKeyJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::E2EupdateGroupKey);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument UpdateGroupKeyJob::json() const
{
    QJsonObject jsonObj;
    // TODO !
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool UpdateGroupKeyJob::UpdateGroupKeyInfo::isValid() const
{
    return !uid.isEmpty() && !roomId.isEmpty() && !key.isEmpty();
}

#include "moc_updategroupkeyjob.cpp"
