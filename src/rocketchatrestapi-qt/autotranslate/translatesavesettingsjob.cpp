/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "translatesavesettingsjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
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

void TranslateSaveSettingsJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("TranslateSaveSettingsJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT translateSavesettingsDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("TranslateSaveSettingsJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
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
    if (mType == SettingType::LanguageSetting && mLanguage.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "TranslateSaveSettingsJob: mLanguage is empty";
        return false;
    }
    if (mType == SettingType::Undefined) {
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
    jsonObj["roomId"_L1] = mRoomId;
    switch (mType) {
    case SettingType::AutoTranslateSetting:
        jsonObj["field"_L1] = u"autoTranslate"_s;
        jsonObj["value"_L1] = mAutoTranslate;
        break;
    case SettingType::LanguageSetting:
        jsonObj["field"_L1] = u"autoTranslateLanguage"_s;
        jsonObj["value"_L1] = mLanguage;
        break;
    case SettingType::Undefined:
        break;
    }
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_translatesavesettingsjob.cpp"
