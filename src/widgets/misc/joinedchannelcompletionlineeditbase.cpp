/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "joinedchannelcompletionlineeditbase.h"
#include "common/completionlistview.h"
#include "joinedchannelcompletiondelegate.h"
#include "model/joinedchannelmodel.h"
#include "rocketchataccount.h"
#include "room.h"
#include <KLocalizedString>
#include <QTimer>
#include <chrono>

using namespace std::chrono_literals;

JoinedChannelCompletionLineEditBase::JoinedChannelCompletionLineEditBase(RocketChatAccount *account, QWidget *parent)
    : CompletionLineEdit(parent)
    , mJoinedChannelModel(new JoinedChannelModel(this))
    , mSearchTimer(new QTimer(this))
    , mRocketChatAccount(account)
{
    setPlaceholderText(i18n("Search rooms..."));
    setCompletionModel(mJoinedChannelModel);
    connect(this, &JoinedChannelCompletionLineEditBase::complete, this, &JoinedChannelCompletionLineEditBase::slotComplete);
    connect(mSearchTimer, &QTimer::timeout, this, &JoinedChannelCompletionLineEditBase::slotSearchTimerFired);
    connect(this, &QLineEdit::textChanged, this, &JoinedChannelCompletionLineEditBase::slotSearchTextEdited);

    auto joinedChannelCompletionDelegate = new JoinedChannelCompletionDelegate(mCompletionListView);
    joinedChannelCompletionDelegate->setObjectName(QStringLiteral("joinedChannelCompletionDelegate"));
    joinedChannelCompletionDelegate->setRocketChatAccount(account);
    mCompletionListView->setItemDelegate(joinedChannelCompletionDelegate);
    mCompletionListView->setTextWidget(this);
}

JoinedChannelCompletionLineEditBase::~JoinedChannelCompletionLineEditBase() = default;

void JoinedChannelCompletionLineEditBase::slotSearchTimerFired()
{
    mSearchTimer->stop();
    slotTextChanged(text());
}

void JoinedChannelCompletionLineEditBase::slotSearchTextEdited()
{
    if (mSearchTimer->isActive()) {
        mSearchTimer->stop(); // eventually
    }

    mSearchTimer->setSingleShot(true);
    mSearchTimer->start(300ms);
}

void JoinedChannelCompletionLineEditBase::slotTextChanged(const QString &text)
{
    QList<ChannelUserCompleter> channels;
    if (mRocketChatAccount) {
        if (!text.isEmpty()) {
            const QList<Room *> rooms = mRocketChatAccount->roomModel()->findRoomNameConstains(text);
            if (rooms.isEmpty()) {
                mCompletionListView->hide();
            } else {
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
                    channel.setAvatarInfo(room->avatarInfo());
                    channels.append(std::move(channel));
                }
            }
        } else {
            mCompletionListView->hide();
        }
    }
    mJoinedChannelModel->setRooms(channels);
}

void JoinedChannelCompletionLineEditBase::slotComplete(const QModelIndex &index)
{
    const QString completerName = index.data(JoinedChannelModel::Name).toString();
    const QString roomId = index.data(JoinedChannelModel::ChannelId).toString();
    if (completerName.isEmpty() || roomId.isEmpty()) {
        return;
    }
    JoinedChannelCompletionInfo info;
    info.name = completerName;
    info.channelId = roomId;
    mCompletionListView->hide();
    Q_EMIT joinedChannelFound(std::move(info));
}

#include "moc_joinedchannelcompletionlineeditbase.cpp"
