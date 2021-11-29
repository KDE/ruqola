/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT SetUserPublicAndPrivateKeysJob : public RestApiAbstractJob
{
    Q_OBJECT

    // Since rocketchat 0.70
public:
    explicit SetUserPublicAndPrivateKeysJob(QObject *parent = nullptr);
    ~SetUserPublicAndPrivateKeysJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString rsaPublicKey() const;
    void setRsaPublicKey(const QString &rsaPublicKey);

    Q_REQUIRED_RESULT QString rsaPrivateKey() const;
    void setRsaPrivateKey(const QString &rsaPrivateKey);

Q_SIGNALS:
    void addKeyToChainDone();

private:
    Q_DISABLE_COPY(SetUserPublicAndPrivateKeysJob)
    void slotAddKeyToChainFinished();
    QString mRsaPublicKey;
    QString mRsaPrivateKey;
};
}
