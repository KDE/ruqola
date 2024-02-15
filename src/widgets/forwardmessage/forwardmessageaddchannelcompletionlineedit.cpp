/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "forwardmessageaddchannelcompletionlineedit.h"
#include "common/completionlistview.h"
#include "forwardmessagechannelmodel.h"
#include "rocketchataccount.h"
#include "room.h"
#include <KLocalizedString>
#include <QTimer>
#include <chrono>

using namespace std::chrono_literals;

ForwardMessageAddChannelCompletionLineEdit::ForwardMessageAddChannelCompletionLineEdit(RocketChatAccount *account, QWidget *parent)
    : CompletionLineEdit(parent)
    , mForwardMessageChannelModel(new ForwardMessageChannelModel(this))
    , mSearchTimer(new QTimer(this))
    , mRocketChatAccount(account)
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
    mSearchTimer->start(300ms);
}

void ForwardMessageAddChannelCompletionLineEdit::slotTextChanged(const QString &text)
{
    QVector<ChannelUserCompleter> channels;
    if (mRocketChatAccount) {
        if (!text.isEmpty()) {
            const QVector<Room *> rooms = mRocketChatAccount->roomModel()->findRoomNameConstains(text);
            for (const Room *room : rooms) {
                ChannelUserCompleter channel;
                switch (room->channelType()) {
                case Room::RoomType::Channel:
                    channel.setType(ChannelUserCompleter::ChannelUserCompleterType::Room);
                    break;
                case Room::RoomType::Direct:
                    channel.setType(ChannelUserCompleter::ChannelUserCompleterType::DirectChannel);
                    break;
                default:
                    break;
                }
                channel.setName(room->displayFName());
                channel.setIdentifier(room->roomId());
                channel.setChannelIcon();
                channel.setAvatarInfo(room->avatarInfo());
                channels.append(std::move(channel));
            }
        }
    }
    mForwardMessageChannelModel->setRooms(channels);
}

void ForwardMessageAddChannelCompletionLineEdit::slotComplete(const QModelIndex &index)
{
    const QString completerName = index.data(ForwardMessageChannelModel::Name).toString();
    const QString roomId = index.data(ForwardMessageChannelModel::ChannelId).toString();
    ForwardMessageChannelCompletionInfo info;
    info.name = completerName;
    info.channelId = roomId;
    mCompletionListView->hide();
    disconnect(this, &QLineEdit::textChanged, this, &ForwardMessageAddChannelCompletionLineEdit::slotSearchTextEdited);
    Q_EMIT fowardToChannel(std::move(info));
    clear();
    connect(this, &QLineEdit::textChanged, this, &ForwardMessageAddChannelCompletionLineEdit::slotSearchTextEdited);
}

#include "moc_forwardmessageaddchannelcompletionlineedit.cpp"
