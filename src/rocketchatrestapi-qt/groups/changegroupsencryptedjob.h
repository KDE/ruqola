/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librestapi_private_export.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT ChangeGroupsEncryptedJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit ChangeGroupsEncryptedJob(QObject *parent = nullptr);
    ~ChangeGroupsEncryptedJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] bool encrypted() const;
    void setEncrypted(bool encrypted);

Q_SIGNALS:
    void changeEncryptedDone();

private:
    Q_DISABLE_COPY(ChangeGroupsEncryptedJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    bool mEncrypted = false;
};
}
