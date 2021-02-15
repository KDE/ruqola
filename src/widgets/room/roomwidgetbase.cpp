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

#include "roomwidgetbase.h"
#include "messagelinewidget.h"
#include "readonlylineeditwidget.h"
#include "roomquotemessagewidget.h"
#include "roomreplythreadwidget.h"
#include "uploadfileprogressstatuswidget.h"

#include <QStackedWidget>
#include <QVBoxLayout>

RoomWidgetBase::RoomWidgetBase(MessageListView::Mode mode, QWidget *parent)
    : QWidget(parent)
    , mUploadFileProgressStatusWidget(new UploadFileProgressStatusWidget(this))
    , mMessageListView(new MessageListView(mode, this))
    , mRoomReplyThreadWidget(new RoomReplyThreadWidget(this))
    , mRoomQuoteMessageWidget(new RoomQuoteMessageWidget(this))
    , mStackedWidget(new QStackedWidget(this))
    , mMessageLineWidget(new MessageLineWidget(this))
    , mReadOnlyLineEditWidget(new ReadOnlyLineEditWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mUploadFileProgressStatusWidget->setObjectName(QStringLiteral("mUploadFileProgressStatusWidget"));
    mUploadFileProgressStatusWidget->setVisible(false);
    mainLayout->addWidget(mUploadFileProgressStatusWidget);

    mMessageListView->setObjectName(QStringLiteral("mMessageListView"));
    mainLayout->addWidget(mMessageListView, 1);

    mRoomReplyThreadWidget->setObjectName(QStringLiteral("mRoomReplyThreadWidget"));
    mRoomReplyThreadWidget->setVisible(false);

    connect(mRoomReplyThreadWidget, &RoomReplyThreadWidget::cancelReplyingInThread, this, [this] {
        mMessageLineWidget->setThreadMessageId({});
    });

    mRoomQuoteMessageWidget->setObjectName(QStringLiteral("mRoomQuoteMessageWidget"));
    mRoomQuoteMessageWidget->setVisible(false);

    connect(mRoomQuoteMessageWidget, &RoomQuoteMessageWidget::cancelQuoteMessage, this, [this] {
        mMessageLineWidget->setQuoteMessage({}, QString());
    });

    mainLayout->addWidget(mRoomReplyThreadWidget);
    mainLayout->addWidget(mRoomQuoteMessageWidget);

    mStackedWidget->setObjectName(QStringLiteral("mStackedWidget"));
    mainLayout->addWidget(mStackedWidget);

    mMessageLineWidget->setObjectName(QStringLiteral("mMessageLineWidget"));
    mStackedWidget->addWidget(mMessageLineWidget);

    mReadOnlyLineEditWidget->setObjectName(QStringLiteral("mReadOnlyLineEditWidget"));
    mStackedWidget->addWidget(mReadOnlyLineEditWidget);

    mStackedWidget->setCurrentWidget(mMessageLineWidget);

    //    connect(mMessageLineWidget, &MessageLineWidget::keyPressed, this, &RoomWidgetBase::keyPressedInLineEdit);
    //    connect(mMessageLineWidget, &MessageLineWidget::threadMessageIdChanged, this, &RoomWidgetBase::slotShowThreadMessage);
    //    connect(mMessageLineWidget, &MessageLineWidget::quoteMessageChanged, this, &RoomWidgetBase::slotShowQuoteMessage);

    connect(mMessageListView, &MessageListView::editMessageRequested, mMessageLineWidget, &MessageLineWidget::setEditMessage);
    connect(mMessageListView, &MessageListView::quoteMessageRequested, mMessageLineWidget, &MessageLineWidget::setQuoteMessage);
    //    connect(mMessageListView, &MessageListView::createNewDiscussion, this, &RoomWidgetBase::slotCreateNewDiscussion);
    //    connect(mMessageListView, &MessageListView::createPrivateConversation, this, &RoomWidgetBase::slotCreatePrivateDiscussion);
    //    connect(mMessageListView, &MessageListView::loadHistoryRequested, this, &RoomWidgetBase::slotLoadHistory);
    connect(mMessageListView, &MessageListView::replyInThreadRequested, mMessageLineWidget, [this](const QString &messageId) {
        mMessageLineWidget->setThreadMessageId(messageId);
    });

    setAcceptDrops(true);
}

RoomWidgetBase::~RoomWidgetBase()
{
}

MessageListView *RoomWidgetBase::messageListView() const
{
    return mMessageListView;
}

MessageLineWidget *RoomWidgetBase::messageLineWidget() const
{
    return mMessageLineWidget;
}
