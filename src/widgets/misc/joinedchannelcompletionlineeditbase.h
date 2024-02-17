/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "common/completionlineedit.h"
#include "libruqolawidgets_private_export.h"
class JoinedChannelModel;
class QTimer;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT JoinedChannelCompletionLineEditBase : public CompletionLineEdit
{
    Q_OBJECT
public:
    struct JoinedChannelCompletionInfo {
        QString name;
        QString channelId;
    };
    explicit JoinedChannelCompletionLineEditBase(RocketChatAccount *account, QWidget *parent = nullptr);
    ~JoinedChannelCompletionLineEditBase() override;

Q_SIGNALS:
    void joinedChannelFound(const JoinedChannelCompletionLineEditBase::JoinedChannelCompletionInfo &channelInfo);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotComplete(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSearchTimerFired();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSearchTextEdited();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTextChanged(const QString &text);
    JoinedChannelModel *const mJoinedChannelModel;
    QTimer *const mSearchTimer;
    RocketChatAccount *const mRocketChatAccount;
};
Q_DECLARE_TYPEINFO(JoinedChannelCompletionLineEditBase::JoinedChannelCompletionInfo, Q_RELOCATABLE_TYPE);
