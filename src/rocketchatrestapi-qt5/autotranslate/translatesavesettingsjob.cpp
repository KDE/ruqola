/*
   SPDX-FileCopyrightText: 2019-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "translatesavesettingsjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
TranslateSaveSettingsJob::TranslateSaveSettingsJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

TranslateSaveSettingsJob::~TranslateSaveSettingsJob() = default;

bool TranslateSaveSettingsJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    submitPostRequest(json());
    return true;
}

void TranslateSaveSettingsJob::onPostRequestResponse(const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("TranslateSaveSettingsJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT translateSavesettingsDone();
    } else {
        emitFailedMessage(replyObject);
        addLoggerWarning(QByteArrayLiteral("TranslateSaveSettingsJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

TranslateSaveSettingsJob::SettingType TranslateSaveSettingsJob::type() const
{
    return mType;
}

void TranslateSaveSettingsJob::setType(RocketChatRestApi::TranslateSaveSettingsJob::SettingType type)
{
    mType = type;
}

QString TranslateSaveSettingsJob::language() const
{
    return mLanguage;
}

void TranslateSaveSettingsJob::setLanguage(const QString &language)
{
    mLanguage = language;
}

bool TranslateSaveSettingsJob::autoTranslate() const
{
    return mAutoTranslate;
}

void TranslateSaveSettingsJob::setAutoTranslate(bool autoTranslate)
{
    mAutoTranslate = autoTranslate;
}

QString TranslateSaveSettingsJob::roomId() const
{
    return mRoomId;
}

void TranslateSaveSettingsJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

bool TranslateSaveSettingsJob::requireHttpAuthentication() const
{
    return true;
}

bool TranslateSaveSettingsJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "TranslateSaveSettingsJob: mRoomId is empty";
        return false;
    }
    if (mType == LanguageSetting && mLanguage.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "TranslateSaveSettingsJob: mLanguage is empty";
        return false;
    }
    if (mType == Undefined) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "TranslateSaveSettingsJob: mType is not defined";
        return false;
    }
    return true;
}

QNetworkRequest TranslateSaveSettingsJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::AutoTranslateSaveSettings);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument TranslateSaveSettingsJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = mRoomId;
    switch (mType) {
    case AutoTranslateSetting:
        jsonObj[QLatin1String("field")] = QStringLiteral("autoTranslate");
        jsonObj[QLatin1String("value")] = mAutoTranslate;
        break;
    case LanguageSetting:
        jsonObj[QLatin1String("field")] = QStringLiteral("autoTranslateLanguage");
        jsonObj[QLatin1String("value")] = mLanguage;
        break;
    case Undefined:
        break;
    }
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
