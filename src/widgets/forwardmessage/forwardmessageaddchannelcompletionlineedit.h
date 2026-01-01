/*
  SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "misc/joinedchannelcompletionlineeditbase.h"
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ForwardMessageAddChannelCompletionLineEdit : public JoinedChannelCompletionLineEditBase
{
    Q_OBJECT
public:
    explicit ForwardMessageAddChannelCompletionLineEdit(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ForwardMessageAddChannelCompletionLineEdit() override;

Q_SIGNALS:
    void fowardToChannel(const JoinedChannelCompletionLineEditBase::JoinedChannelCompletionInfo &channelInfo);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotJoinedChannelFound(const JoinedChannelCompletionLineEditBase::JoinedChannelCompletionInfo &info);
};
