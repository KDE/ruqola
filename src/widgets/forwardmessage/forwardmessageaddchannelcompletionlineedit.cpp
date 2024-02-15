/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "forwardmessageaddchannelcompletionlineedit.h"
#include "forwardmessagechannelmodel.h"
#include <KLocalizedString>
#include <QTimer>
#include <chrono>

using namespace std::chrono_literals;

ForwardMessageAddChannelCompletionLineEdit::ForwardMessageAddChannelCompletionLineEdit(QWidget *parent)
    : CompletionLineEdit(parent)
    , mForwardMessageChannelModel(new ForwardMessageChannelModel(this))
    , mSearchTimer(new QTimer(this))
{
    setPlaceholderText(i18n("Search rooms..."));
    setCompletionModel(mForwardMessageChannelModel);
    connect(this, &ForwardMessageAddChannelCompletionLineEdit::complete, this, &ForwardMessageAddChannelCompletionLineEdit::slotComplete);
    connect(mSearchTimer, &QTimer::timeout, this, &ForwardMessageAddChannelCompletionLineEdit::slotSearchTimerFired);
    connect(this, &QLineEdit::textChanged, this, &ForwardMessageAddChannelCompletionLineEdit::slotSearchTextEdited);
}

ForwardMessageAddChannelCompletionLineEdit::~ForwardMessageAddChannelCompletionLineEdit() = default;

void ForwardMessageAddChannelCompletionLineEdit::slotSearchTimerFired()
{
    mSearchTimer->stop();
    slotTextChanged(text());
}

void ForwardMessageAddChannelCompletionLineEdit::slotSearchTextEdited()
{
    if (mSearchTimer->isActive()) {
        mSearchTimer->stop(); // eventually
    }

    mSearchTimer->setSingleShot(true);
    mSearchTimer->start(500ms);
}

void ForwardMessageAddChannelCompletionLineEdit::slotTextChanged(const QString &text)
{
    // TODO
}

void ForwardMessageAddChannelCompletionLineEdit::slotComplete(const QModelIndex &index)
{
#if 0
    const QString completerName = index.data(ForwardMessageChannelModel::TeamName).toString();
    const QString roomId = index.data(ForwardMessageChannelModel::TeamId).toString();
    RoomCompletionInfo info;
    info.roomName = completerName;
    info.roomId = roomId;
    mCompletionListView->hide();
    Q_EMIT newRoomName(info);
#endif
    clear();
}

#include "moc_forwardmessageaddchannelcompletionlineedit.cpp"
