/*
   Copyright (c) 2019-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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

TranslateSaveSettingsJob::~TranslateSaveSettingsJob()
{
}

bool TranslateSaveSettingsJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &TranslateSaveSettingsJob::slotTranslateSaveSettingsFinished);
    return true;
}

void TranslateSaveSettingsJob::slotTranslateSaveSettingsFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("TranslateSaveSettingsJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT translateSavesettingsDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("TranslateSaveSettingsJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
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
    if (mType == Underfined) {
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
    case Underfined:
        break;
    }
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
