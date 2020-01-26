/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#include "ruqolamainwidget.h"
#include "channellist/channellistwidget.h"
#include "room/roomwidget.h"

#include <KConfigGroup>
#include <KSharedConfig>
#include <QSplitter>
#include <QVBoxLayout>

namespace {
static const char myConfigGroupName[] = "RuqolaMainWidget";
}

RuqolaMainWidget::RuqolaMainWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainlayout"));

    mSplitter = new QSplitter(this);
    mSplitter->setObjectName(QStringLiteral("mSplitter"));
    mSplitter->setChildrenCollapsible(false);
    mainLayout->addWidget(mSplitter);

    mChannelList = new ChannelListWidget(this);
    mChannelList->setObjectName(QStringLiteral("mChannelList"));
    mSplitter->addWidget(mChannelList);

    mRoomWidget = new RoomWidget(this);
    mRoomWidget->setObjectName(QStringLiteral("mRoomWidget"));
    mSplitter->addWidget(mRoomWidget);

    connect(mChannelList, &ChannelListWidget::channelSelected, mRoomWidget, &RoomWidget::channelSelected);

    KConfigGroup group(KSharedConfig::openConfig(), myConfigGroupName);
    mSplitter->restoreState(group.readEntry("SplitterSizes", QByteArray()));
}

RuqolaMainWidget::~RuqolaMainWidget()
{
    KConfigGroup group(KSharedConfig::openConfig(), myConfigGroupName);
    group.writeEntry("SplitterSizes", mSplitter->saveState());
}

QString RuqolaMainWidget::roomId() const
{
    return mRoomWidget->roomId();
}
