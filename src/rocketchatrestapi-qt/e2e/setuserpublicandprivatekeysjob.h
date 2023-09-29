/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT SetUserPublicAndPrivateKeysJob : public RestApiAbstractJob
{
    Q_OBJECT

    // Since rocketchat 0.70
public:
    explicit SetUserPublicAndPrivateKeysJob(QObject *parent = nullptr);
    ~SetUserPublicAndPrivateKeysJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString rsaPublicKey() const;
    void setRsaPublicKey(const QString &rsaPublicKey);

    [[nodiscard]] QString rsaPrivateKey() const;
    void setRsaPrivateKey(const QString &rsaPrivateKey);

Q_SIGNALS:
    void addKeyToChainDone();

private:
    Q_DISABLE_COPY(SetUserPublicAndPrivateKeysJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mRsaPublicKey;
    QString mRsaPrivateKey;
};
}
