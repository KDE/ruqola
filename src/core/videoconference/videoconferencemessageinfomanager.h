/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolacore_export.h"
#include <QObject>
class RocketChatAccount;
class QTimer;
class LIBRUQOLACORE_EXPORT VideoConferenceMessageInfoManager : public QObject
{
    Q_OBJECT
public:
    explicit VideoConferenceMessageInfoManager(RocketChatAccount *account, QObject *parent = nullptr);
    ~VideoConferenceMessageInfoManager() override;
    [[nodiscard]] RocketChatAccount *rocketChatAccount() const;

    void addCallId(const QString &callId);

private:
    LIBRUQOLACORE_NO_EXPORT void slotUpdateMessage();
    LIBRUQOLACORE_NO_EXPORT void updateVideoConferenceInfo(const QString &callId);
    QStringList mCallIdList;
    RocketChatAccount *const mRocketChatAccount;
    QTimer *const mTimer;
};
