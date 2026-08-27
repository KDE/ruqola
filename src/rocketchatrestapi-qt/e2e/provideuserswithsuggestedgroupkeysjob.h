/*
   SPDX-FileCopyrightText: 2025 Andor Ranogajec <ranogaet@gmail.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
#include <QString>
#include <QVector>

namespace RocketChatRestApi
{

/**
 * A key of the room from before it was re-keyed, re-encrypted for the member we hand it to. Without
 * them that member can read what was written since the last key change only.
 */
struct SuggestedOldGroupKey {
    QString keyId;
    QString encryptedKey;
    // ISO-8601 date: the server feeds it to new Date() before storing it.
    QString timeStamp;
};

/**
 * QString userId;
 *
 *QString encryptedKey;
 */
struct SuggestedGroupKey {
    QString userId;
    QString encryptedKey; // base64 ????
    QVector<SuggestedOldGroupKey> oldKeys;
};

class LIBROCKETCHATRESTAPI_QT_EXPORT ProvideUsersWithSuggestedGroupKeysJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit ProvideUsersWithSuggestedGroupKeysJob(QObject *parent = nullptr);
    ~ProvideUsersWithSuggestedGroupKeysJob() override;

    void setRoomId(const QString &roomId);
    void setKeys(const QVector<SuggestedGroupKey> &keys);

    [[nodiscard]] QString roomId() const;
    [[nodiscard]] QVector<SuggestedGroupKey> keys() const;
    [[nodiscard]] QNetworkRequest request() const override;
    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] bool requireHttpAuthentication() const override;

protected:
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;

Q_SIGNALS:
    void provideUsersWithSuggestedGroupKeysDone(const QJsonObject &result);

private:
    QString mRoomId;
    QVector<SuggestedGroupKey> mSuggestedGroupKeys;
};
}
