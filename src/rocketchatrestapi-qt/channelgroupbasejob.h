/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

#include <QJsonObject>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT ChannelGroupBaseJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit ChannelGroupBaseJob(QObject *parent = nullptr);
    ~ChannelGroupBaseJob() override;
    enum class ChannelGroupInfoType {
        Unknown,
        Identifier,
        Name,
    };
    Q_ENUM(ChannelGroupInfoType)

    struct ChannelGroupInfo {
        QString identifier;
        ChannelGroupBaseJob::ChannelGroupInfoType channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Unknown;
    };

    [[nodiscard]] bool hasIdentifier() const;

    [[nodiscard]] ChannelGroupInfo channelGroupInfo() const;
    void setChannelGroupInfo(const ChannelGroupInfo &channelInfo);

protected:
    void generateJson(QJsonObject &obj) const;
    void addQueryItem(QUrl &url) const;

private:
    ChannelGroupInfo mChannelGroupInfo;
};
}
LIBROCKETCHATRESTAPI_QT_EXPORT QDebug operator<<(QDebug d, const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &t);
