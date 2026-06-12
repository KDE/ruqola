/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customsoundsgetonejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

#include <QUrlQuery>
using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
CustomSoundsGetOneJob::CustomSoundsGetOneJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

CustomSoundsGetOneJob::~CustomSoundsGetOneJob() = default;

bool CustomSoundsGetOneJob::requireHttpAuthentication() const
{
    return true;
}

bool CustomSoundsGetOneJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mSoundId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "CustomSoundsGetOneJob: SoundId is empty";
        return false;
    }
    return true;
}

bool CustomSoundsGetOneJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start CustomSoundsGetOneJob job";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo("CustomSoundsGetOneJob: Ask if room name already exists"_ba);
    return true;
}

void CustomSoundsGetOneJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("CustomSoundsGetOneJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT soundInfoDone(replyObject["subscription"_L1].toObject());
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("CustomSoundsGetOneJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QByteArray CustomSoundsGetOneJob::soundId() const
{
    return mSoundId;
}

void CustomSoundsGetOneJob::setSoundId(const QByteArray &newSoundId)
{
    mSoundId = newSoundId;
}

QNetworkRequest CustomSoundsGetOneJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::CustomSoundsGetOne);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(u"_id"_s, QString::fromLatin1(mSoundId));
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_customsoundsgetonejob.cpp"
