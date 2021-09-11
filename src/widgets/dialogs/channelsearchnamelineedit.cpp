/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#include "channelsearchnamelineedit.h"
#include "common/completionlistview.h"
#include "connection.h"
#include "misc/directoryjob.h"
#include "misc/lineeditcatchreturnkey.h"
#include "model/channelcompleterfilterproxymodel.h"
#include "model/channelcompletermodel.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include <QJsonObject>

ChannelSearchNameLineEdit::ChannelSearchNameLineEdit(QWidget *parent)
    : CompletionLineEdit(parent)
    , mChannelCompleterFilterProxyModel(new ChannelCompleterFilterProxyModel(this))
    , mChannelCompleterModel(new ChannelCompleterModel(this))
{
    new LineEditCatchReturnKey(this, this);
    mChannelCompleterFilterProxyModel->setSourceModel(mChannelCompleterModel);
    connect(this, &QLineEdit::textChanged, this, &ChannelSearchNameLineEdit::slotTextChanged);
    setCompletionModel(mChannelCompleterFilterProxyModel);
    connect(this, &ChannelSearchNameLineEdit::complete, this, &ChannelSearchNameLineEdit::slotComplete);
}

ChannelSearchNameLineEdit::~ChannelSearchNameLineEdit()
{
}

void ChannelSearchNameLineEdit::slotTextChanged(const QString &text)
{
    if (!text.trimmed().isEmpty()) {
        auto *rcAccount = Ruqola::self()->rocketChatAccount();
        auto job = new RocketChatRestApi::DirectoryJob(this);
        RocketChatRestApi::DirectoryJob::DirectoryInfo info;
        info.pattern = text;
        info.searchType = RocketChatRestApi::DirectoryJob::Rooms;
        job->setDirectoryInfo(info);
        rcAccount->restApi()->initializeRestApiJob(job);
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
    const int roomsSize(rooms.size());
    channelList.reserve(roomsSize);
    for (int i = 0; i < roomsSize; i++) {
        const QJsonObject o = rooms.at(i).toObject();
        Channel channel;
        channel.parseChannel(o, Channel::ChannelType::Room);
        // Verify that it's valid
        channelList.append(channel);
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
