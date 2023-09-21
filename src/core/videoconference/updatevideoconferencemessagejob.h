/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include "videoconferenceinfo.h"
#include <QObject>
class RocketChatAccount;
class LIBRUQOLACORE_EXPORT UpdateVideoConferenceMessageJob : public QObject
{
    Q_OBJECT
public:
    explicit UpdateVideoConferenceMessageJob(QObject *parent = nullptr);
    ~UpdateVideoConferenceMessageJob() override;

    [[nodiscard]] bool canStart() const;
    void start();

    [[nodiscard]] VideoConferenceInfo videoConferenceInfo() const;
    void setVideoConferenceInfo(const VideoConferenceInfo &newVideoConferenceInfo);

    [[nodiscard]] RocketChatAccount *rocketChatAccount() const;
    void setRocketChatAccount(RocketChatAccount *newRocketChatAccount);

private:
    VideoConferenceInfo mVideoConferenceInfo;
    RocketChatAccount *mRocketChatAccount = nullptr;
};
