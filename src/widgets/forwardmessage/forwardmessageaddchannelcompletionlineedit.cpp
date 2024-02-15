/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "forwardmessageaddchannelcompletionlineedit.h"
#include "forwardmessagechannelmodel.h"
#include <KLocalizedString>

ForwardMessageAddChannelCompletionLineEdit::ForwardMessageAddChannelCompletionLineEdit(QWidget *parent)
    : CompletionLineEdit(parent)
    , mForwardMessageChannelModel(new ForwardMessageChannelModel(this))
{
    setPlaceholderText(i18n("Search rooms..."));
    setCompletionModel(mForwardMessageChannelModel);
    connect(this, &ForwardMessageAddChannelCompletionLineEdit::complete, this, &ForwardMessageAddChannelCompletionLineEdit::slotComplete);
}

ForwardMessageAddChannelCompletionLineEdit::~ForwardMessageAddChannelCompletionLineEdit() = default;

void ForwardMessageAddChannelCompletionLineEdit::slotComplete(const QModelIndex &index)
{
#if 0
    const QString completerName = index.data(TeamRoomCompleterModel::TeamName).toString();
    const QString roomId = index.data(TeamRoomCompleterModel::TeamId).toString();
    RoomCompletionInfo info;
    info.roomName = completerName;
    info.roomId = roomId;
    mCompletionListView->hide();
    Q_EMIT newRoomName(info);
#endif
    clear();
}

#include "moc_forwardmessageaddchannelcompletionlineedit.cpp"
