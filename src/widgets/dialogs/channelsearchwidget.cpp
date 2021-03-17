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

#include "channelsearchwidget.h"
#include "channelsearchnamelineresultwidget.h"
#include <KLocalizedString>
#include <QStackedWidget>
#include <QVBoxLayout>
ChannelSearchWidget::ChannelSearchWidget(QWidget *parent)
    : QWidget(parent)
    , mStackedWidget(new QStackedWidget(this))
    , mChannelSearchNameLineEdit(new ChannelSearchNameLineEdit(this))
    , mChannelSearchLineResult(new ChannelSearchNameLineResultWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mainLayout->addWidget(mStackedWidget);
    mStackedWidget->setObjectName(QStringLiteral("mStackedWidget"));
    mChannelSearchNameLineEdit->setObjectName(QStringLiteral("mChannelSearchNameLineEdit"));
    mStackedWidget->addWidget(mChannelSearchNameLineEdit);

    mChannelSearchLineResult->setObjectName(QStringLiteral("mChannelSearchLineResult"));
    mStackedWidget->addWidget(mChannelSearchLineResult);

    connect(mChannelSearchNameLineEdit, &ChannelSearchNameLineEdit::newRoomName, this, &ChannelSearchWidget::slotSelectedRoom);
    connect(mChannelSearchLineResult, &ChannelSearchNameLineResultWidget::clearRoomName, this, &ChannelSearchWidget::slotClearRoom);
}

ChannelSearchWidget::~ChannelSearchWidget()
{
}

void ChannelSearchWidget::setChannelInfo(const QString &name, const QString &channelId)
{
    mUserInfo.channelName = name;
    mUserInfo.channelId = channelId;
    slotSelectedRoom(mUserInfo);
    mChannelSearchLineResult->setReadOnly(true);
}

QString ChannelSearchWidget::channelName() const
{
    return mUserInfo.channelName;
}

QString ChannelSearchWidget::channelId() const
{
    return mUserInfo.channelId;
}

void ChannelSearchWidget::slotSelectedRoom(const ChannelSearchNameLineEdit::ChannelCompletionInfo &userInfo)
{
    mUserInfo = userInfo;
    mStackedWidget->setCurrentWidget(mChannelSearchLineResult);
    mChannelSearchLineResult->setRoomName(userInfo.channelName);
    Q_EMIT updateRoomName(true);
}

void ChannelSearchWidget::slotClearRoom()
{
    mUserInfo = ChannelSearchNameLineEdit::ChannelCompletionInfo();
    mStackedWidget->setCurrentWidget(mChannelSearchNameLineEdit);
    Q_EMIT updateRoomName(false);
}
