/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librestapi_private_export.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT ChangeGroupsEncryptedJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit ChangeGroupsEncryptedJob(QObject *parent = nullptr);
    ~ChangeGroupsEncryptedJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT bool encrypted() const;
    void setEncrypted(bool encrypted);

Q_SIGNALS:
    void changeEncryptedDone();

private:
    Q_DISABLE_COPY(ChangeGroupsEncryptedJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    bool mEncrypted = false;
};
}
