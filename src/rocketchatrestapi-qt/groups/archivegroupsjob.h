/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librestapi_private_export.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT ArchiveGroupsJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit ArchiveGroupsJob(QObject *parent = nullptr);
    ~ArchiveGroupsJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] bool archive() const;
    void setArchive(bool archive);

Q_SIGNALS:
    void archiveGroupsDone();
    void archiveGroupsError(const QString &);

private:
    Q_DISABLE_COPY(ArchiveGroupsJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    bool mArchive = true;
};
}
