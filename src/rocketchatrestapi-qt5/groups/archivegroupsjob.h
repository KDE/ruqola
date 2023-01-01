/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librestapi_private_export.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT ArchiveGroupsJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit ArchiveGroupsJob(QObject *parent = nullptr);
    ~ArchiveGroupsJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT bool archive() const;
    void setArchive(bool archive);

Q_SIGNALS:
    void archiveGroupsDone();
    void archiveGroupsError(const QString &);

private:
    Q_DISABLE_COPY(ArchiveGroupsJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    bool mArchive = true;
};
}
