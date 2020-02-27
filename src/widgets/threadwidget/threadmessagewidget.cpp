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
#include "room/messagetextedit.h"
#include <QMimeData>
#include <QApplication>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QClipboard>

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
    mMessageLineWidget->setMessageLineType(MessageLineWidget::MessageLineType::ThreadLineEdit);
    mainLayout->addWidget(mMessageLineWidget);

    connect(mMessageLineWidget->messageTextEdit(), &MessageTextEdit::keyPressed, this, &ThreadMessageWidget::keyPressedInLineEdit);

}

ThreadMessageWidget::~ThreadMessageWidget()
{
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
        mMessageLineWidget->setRoomId(mThreadMessageId);
    }
}

void ThreadMessageWidget::setCurrentRocketChatAccount(RocketChatAccount *account)
{
    mMessageLineWidget->setCurrentRocketChatAccount(account);
}

void ThreadMessageWidget::keyPressedInLineEdit(QKeyEvent *ev)
{
    const int key = ev->key();
    if (key == Qt::Key_Escape) {
        if (!mMessageLineWidget->messageIdBeingEdited().isEmpty()) {
            mMessageLineWidget->clearMessageIdBeingEdited();
        }
        ev->accept();
    } else if (ev->matches(QKeySequence::Paste)) {
        const QMimeData *mimeData = qApp->clipboard()->mimeData();
        if (mMessageLineWidget->handleMimeData(mimeData)) {
            ev->accept();
        }
    } else if ((key == Qt::Key_Up || key == Qt::Key_Down) && ev->modifiers() & Qt::AltModifier) {
        MessageModel *model = Ruqola::self()->rocketChatAccount()->threadMessageModel();
        Q_ASSERT(model);
        auto isEditable = [this](const Message &msg) {
                              return Ruqola::self()->rocketChatAccount()->isMessageEditable(msg);
                          };
        if (key == Qt::Key_Up) {
            const Message &msg = model->findLastMessageBefore(mMessageLineWidget->messageIdBeingEdited(), isEditable);
            mMessageLineWidget->setEditMessage(msg.messageId(), msg.text());
        } else {
            const Message &msg = model->findNextMessageAfter(mMessageLineWidget->messageIdBeingEdited(), isEditable);
            mMessageLineWidget->setEditMessage(msg.messageId(), msg.text());
        }
        ev->accept();
    }
}
