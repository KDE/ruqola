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
#include <QNetworkRequest>

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
    addStartRestApiInfo("ProvideUsersWithSuggestedGroupKeysJob::start"_ba);
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
    QJsonObject usersSuggestedGroupKeys;
    QJsonArray keysArr;
    for (const auto &k : mSuggestedGroupKeys) {
        QJsonObject keyObj;
        keyObj["_id"_L1] = k.userId;
        keyObj["key"_L1] = k.encryptedKey;
        // The endpoint refuses unknown members, so only send "oldKeys" when there is one to send.
        if (!k.oldKeys.isEmpty()) {
            QJsonArray oldKeysArr;
            for (const auto &oldKey : k.oldKeys) {
                QJsonObject oldKeyObj;
                oldKeyObj["e2eKeyId"_L1] = oldKey.keyId;
                oldKeyObj["E2EKey"_L1] = oldKey.encryptedKey;
                oldKeyObj["ts"_L1] = oldKey.timeStamp;
                oldKeysArr.append(std::move(oldKeyObj));
            }
            keyObj["oldKeys"_L1] = std::move(oldKeysArr);
        }
        keysArr.append(std::move(keyObj));
    }
    usersSuggestedGroupKeys.insert(mRoomId, keysArr);
    obj["usersSuggestedGroupKeys"_L1] = usersSuggestedGroupKeys;
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

#include "moc_provideuserswithsuggestedgroupkeysjob.cpp"
