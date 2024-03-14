/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librocketchatrestapi-qt_export.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT GroupAddModeratorJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit GroupAddModeratorJob(QObject *parent = nullptr);
    ~GroupAddModeratorJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString addModeratorUserId() const;
    void setAddModeratorUserId(const QString &addModeratorUserId);

Q_SIGNALS:
    void addModeratorDone();

private:
    Q_DISABLE_COPY(GroupAddModeratorJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mAddModeratorUserId;
};
}
