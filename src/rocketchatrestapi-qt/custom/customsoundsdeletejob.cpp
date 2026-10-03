/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customsoundsdeletejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
CustomSoundsDeleteJob::CustomSoundsDeleteJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

CustomSoundsDeleteJob::~CustomSoundsDeleteJob() = default;

bool CustomSoundsDeleteJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("CustomSoundsDeleteJob::start"_ba);
    submitPostRequest(json());

    return true;
}

void CustomSoundsDeleteJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    if (checkResponse("CustomSoundsDeleteJob"_ba, replyErrorString, replyJson)) {
        Q_EMIT userCustomSoundDeletedDone();
    }
}

QByteArray CustomSoundsDeleteJob::customSoundId() const
{
    return mCustomSoundId;
}

void CustomSoundsDeleteJob::setCustomSoundId(const QByteArray &newCustomSoundId)
{
    mCustomSoundId = newCustomSoundId;
}

bool CustomSoundsDeleteJob::requireHttpAuthentication() const
{
    return true;
}

bool CustomSoundsDeleteJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mCustomSoundId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "CustomSoundsDeleteJob: mCustomSoundId is empty.";
        return false;
    }
    return true;
}

QNetworkRequest CustomSoundsDeleteJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::CustomSoundsDelete);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument CustomSoundsDeleteJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["_id"_L1] = QLatin1StringView(mCustomSoundId);

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_customsoundsdeletejob.cpp"
