/*
  SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once

#include "misc/joinedchannelcompletionlineeditbase.h"

class ExploreDatabaseLineEdit : public JoinedChannelCompletionLineEditBase
{
    Q_OBJECT
public:
    explicit ExploreDatabaseLineEdit(RocketChatAccount *account, QWidget *parent);
    ~ExploreDatabaseLineEdit() override;

    [[nodiscard]] QByteArray channelId() const;
    void setChannelId(const QByteArray &newChannelId);

private:
    void slotJoinedChannelFound(const JoinedChannelCompletionLineEditBase::JoinedChannelCompletionInfo &info);

    QByteArray mChannelId;
};
