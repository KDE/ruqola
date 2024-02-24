/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

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

private:
    void slotJoinedChannelFound(const JoinedChannelCompletionLineEditBase::JoinedChannelCompletionInfo &info);
};
