/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelsearchwidget.h"
using namespace Qt::Literals::StringLiterals;

#include "channelsearchnamelineresultwidget.h"
#include <KLocalizedString>
#include <QStackedWidget>
#include <QVBoxLayout>
ChannelSearchWidget::ChannelSearchWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mStackedWidget(new QStackedWidget(this))
    , mChannelSearchNameLineEdit(new ChannelSearchNameLineEdit(account, this))
    , mChannelSearchLineResult(new ChannelSearchNameLineResultWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mainLayout->addWidget(mStackedWidget);
    mStackedWidget->setObjectName(u"mStackedWidget"_s);
    mChannelSearchNameLineEdit->setObjectName(u"mChannelSearchNameLineEdit"_s);
    mChannelSearchNameLineEdit->setPlaceholderText(i18nc("@info:placeholder", "Select a channel"));
    mStackedWidget->addWidget(mChannelSearchNameLineEdit);

    mChannelSearchLineResult->setObjectName(u"mChannelSearchLineResult"_s);
    mStackedWidget->addWidget(mChannelSearchLineResult);

    connect(mChannelSearchNameLineEdit, &ChannelSearchNameLineEdit::newRoomName, this, &ChannelSearchWidget::slotSelectedRoom);
    connect(mChannelSearchLineResult, &ChannelSearchNameLineResultWidget::clearRoomName, this, &ChannelSearchWidget::slotClearRoom);
}

ChannelSearchWidget::~ChannelSearchWidget() = default;

void ChannelSearchWidget::setChannelInfo(const QString &name, const QByteArray &channelId)
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

QByteArray ChannelSearchWidget::channelId() const
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

#include "moc_channelsearchwidget.cpp"
