/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "channelsearchnamelineedit.h"
#include "common/completionlistview.h"
#include "connection.h"
#include "misc/directoryjob.h"
#include "misc/lineeditcatchreturnkey.h"
#include "model/channelcompleterfilterproxymodel.h"
#include "model/channelcompletermodel.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include <QJsonObject>

ChannelSearchNameLineEdit::ChannelSearchNameLineEdit(RocketChatAccount *account, QWidget *parent)
    : CompletionLineEdit(parent)
    , mChannelCompleterFilterProxyModel(new ChannelCompleterFilterProxyModel(this))
    , mChannelCompleterModel(new ChannelCompleterModel(this))
    , mRocketChatAccount(account)
{
    new LineEditCatchReturnKey(this, this);
    mChannelCompleterFilterProxyModel->setSourceModel(mChannelCompleterModel);
    connect(this, &QLineEdit::textChanged, this, &ChannelSearchNameLineEdit::slotTextChanged);
    setCompletionModel(mChannelCompleterFilterProxyModel);
    connect(this, &ChannelSearchNameLineEdit::complete, this, &ChannelSearchNameLineEdit::slotComplete);
}

ChannelSearchNameLineEdit::~ChannelSearchNameLineEdit() = default;

void ChannelSearchNameLineEdit::slotTextChanged(const QString &text)
{
    if (!text.trimmed().isEmpty()) {
        auto job = new RocketChatRestApi::DirectoryJob(this);
        RocketChatRestApi::DirectoryJob::DirectoryInfo info;
        info.pattern = text;
        info.searchType = RocketChatRestApi::DirectoryJob::Rooms;
        job->setDirectoryInfo(info);
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::DirectoryJob::directoryDone, this, &ChannelSearchNameLineEdit::slotSearchDone);
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start searchRoomUser job";
        }
    } else {
        mChannelCompleterModel->clear();
    }
}

void ChannelSearchNameLineEdit::slotSearchDone(const QJsonObject &obj)
{
    Channel c;
    QVector<Channel> channelList;
    const QJsonArray rooms = obj.value(QLatin1String("result")).toArray();
    const auto roomsSize(rooms.size());
    channelList.reserve(roomsSize);
    for (auto i = 0; i < roomsSize; i++) {
        const QJsonObject o = rooms.at(i).toObject();
        Channel channel;
        channel.parseChannel(o, Channel::ChannelType::Room);
        // Verify that it's valid
        channelList.append(std::move(channel));
    }
    mChannelCompleterModel->insertChannels(channelList);
}

void ChannelSearchNameLineEdit::slotComplete(const QModelIndex &index)
{
    const QString completerName = index.data(ChannelCompleterModel::RoomName).toString();
    const QString roomId = index.data(ChannelCompleterModel::ChannelId).toString();
    ChannelCompletionInfo info;
    info.channelName = completerName;
    info.channelId = roomId;
    mCompletionListView->hide();
    disconnect(this, &QLineEdit::textChanged, this, &ChannelSearchNameLineEdit::slotTextChanged);
    Q_EMIT newRoomName(info);
    clear();
    connect(this, &QLineEdit::textChanged, this, &ChannelSearchNameLineEdit::slotTextChanged);
}
