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
class LIBRUQOLAWIDGETS_TESTS_EXPORT ForwardMessageAddChannelCompletionLineEdit : public CompletionLineEdit
{
    Q_OBJECT
public:
    struct ForwardMessageChannelCompletionInfo {
        QString name;
        QString channelId;
    };
    explicit ForwardMessageAddChannelCompletionLineEdit(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ForwardMessageAddChannelCompletionLineEdit() override;

Q_SIGNALS:
    void fowardToChannel(const ForwardMessageAddChannelCompletionLineEdit::ForwardMessageChannelCompletionInfo &channelInfo);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotComplete(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSearchTimerFired();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSearchTextEdited();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTextChanged(const QString &text);
    JoinedChannelModel *const mForwardMessageChannelModel;
    QTimer *const mSearchTimer;
    RocketChatAccount *const mRocketChatAccount;
};
Q_DECLARE_TYPEINFO(ForwardMessageAddChannelCompletionLineEdit::ForwardMessageChannelCompletionInfo, Q_RELOCATABLE_TYPE);
