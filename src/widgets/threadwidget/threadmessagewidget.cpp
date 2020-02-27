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

#include "threadmessagewidget.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include "room/messagelistview.h"
#include "room/messagelinewidget.h"
#include <QVBoxLayout>

ThreadMessageWidget::ThreadMessageWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);

    //TODO add specific header

    mMessageListView = new MessageListView(this);
    mMessageListView->setObjectName(QStringLiteral("mMessageListView"));
    mMessageListView->setMode(MessageListView::Mode::ThreadEditing);
    mainLayout->addWidget(mMessageListView, 1);

    mMessageLineWidget = new MessageLineWidget(this);
    mMessageLineWidget->setObjectName(QStringLiteral("mMessageLineWidget"));
    mainLayout->addWidget(mMessageLineWidget);

    connect(mMessageLineWidget, &MessageLineWidget::sendMessage, this, &ThreadMessageWidget::slotSendMessage);
//    connect(mMessageLineWidget, &MessageLineWidget::sendFile, this, &RoomWidget::slotSendFile);
//    connect(mMessageLineWidget, &MessageLineWidget::textEditing, this, &RoomWidget::slotTextEditing);
//    connect(mMessageLineWidget->messageTextEdit(), &MessageTextEdit::keyPressed, this, &RoomWidget::keyPressedInLineEdit);
}

ThreadMessageWidget::~ThreadMessageWidget()
{
}

void ThreadMessageWidget::slotSendMessage(const QString &msg)
{
    //TODO implement it. Perhaps we need to extract code from roomWidget if possible.
}

QString ThreadMessageWidget::threadMessageId() const
{
    return mThreadMessageId;
}

void ThreadMessageWidget::setThreadMessageId(const QString &threadMessageId)
{
    if (mThreadMessageId != threadMessageId) {
        mThreadMessageId = threadMessageId;
        Ruqola::self()->rocketChatAccount()->getThreadMessages(mThreadMessageId);
        mMessageListView->setModel(Ruqola::self()->rocketChatAccount()->threadMessageModel());
    }
}

void ThreadMessageWidget::setCurrentRocketChatAccount(RocketChatAccount *account)
{
    mMessageLineWidget->setCurrentRocketChatAccount(account);
}
