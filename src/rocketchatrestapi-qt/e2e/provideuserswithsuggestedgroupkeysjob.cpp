/*
   SPDX-FileCopyrightText: 2025 Andor Ranogajec <ranogaet@gmail.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "provideuserswithsuggestedgroupkeysjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;

ProvideUsersWithSuggestedGroupKeysJob::ProvideUsersWithSuggestedGroupKeysJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ProvideUsersWithSuggestedGroupKeysJob::~ProvideUsersWithSuggestedGroupKeysJob() = default;

void ProvideUsersWithSuggestedGroupKeysJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

void ProvideUsersWithSuggestedGroupKeysJob::setKeys(const QVector<SuggestedGroupKey> &keys)
{
    mSuggestedGroupKeys = keys;
}

QString ProvideUsersWithSuggestedGroupKeysJob::roomId() const
{
    return mRoomId;
}

QVector<SuggestedGroupKey> ProvideUsersWithSuggestedGroupKeysJob::keys() const
{
    return mSuggestedGroupKeys;
}

bool ProvideUsersWithSuggestedGroupKeysJob::requireHttpAuthentication() const
{
    return true;
}

bool ProvideUsersWithSuggestedGroupKeysJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ProvideUsersWithSuggestedGroupKeysJob::start");
    submitPostRequest(json());
    return true;
}

void ProvideUsersWithSuggestedGroupKeysJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("ProvideUsersWithSuggestedGroupKeysJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT provideUsersWithSuggestedGroupKeysDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("ProvideUsersWithSuggestedGroupKeysJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QNetworkRequest ProvideUsersWithSuggestedGroupKeysJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::E2EProvideUsersWithSuggestedGroupKeys);
    QNetworkRequest req(url);
    addAuthRawHeader(req);
    addRequestAttribute(req);
    return req;
}

QJsonDocument ProvideUsersWithSuggestedGroupKeysJob::json() const
{
    QJsonObject obj;
    obj["rid"_L1] = mRoomId;
    QJsonArray keysArr;
    for (const auto &k : mSuggestedGroupKeys) {
        QJsonObject keyObj;
        keyObj["userId"_L1] = k.userId;
        keyObj["key"_L1] = k.encryptedKey;
        keysArr.append(keyObj);
    }
    obj["keys"_L1] = keysArr;
    return QJsonDocument(obj);
}

bool ProvideUsersWithSuggestedGroupKeysJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoomId.isEmpty() || mSuggestedGroupKeys.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ProvideUsersWithSuggestedGroupKeysJob: roomId or keys is empty";
        return false;
    }
    return true;
}
