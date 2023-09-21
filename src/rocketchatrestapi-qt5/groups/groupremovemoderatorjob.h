/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librestapi_private_export.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT GroupRemoveModeratorJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit GroupRemoveModeratorJob(QObject *parent = nullptr);
    ~GroupRemoveModeratorJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString removeUserId() const;
    void setRemoveUserId(const QString &removeUserId);

Q_SIGNALS:
    void removeModeratorDone();

private:
    Q_DISABLE_COPY(GroupRemoveModeratorJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mRemoveUserId;
};
}
