/*
   SPDX-FileCopyrightText: 2026 David Faure <david.faure@kdab.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "threadreplycheck.h"

#include "model/messagesmodel.h"
#include "rocketchataccount.h"

#include <KLocalizedString>

QString ThreadReplyCheck::name() const
{
    return i18n("Thread reply");
}

void ThreadReplyCheck::doStart()
{
    if (!mAccount) {
        reportFailed(i18n("No account."));
        return;
    }
    mParentText = generateMarker();
    mReplyText = mParentText + i18n(" (thread reply)");
    connectModel();
    mAccount->sendMessage(mRoomId, mParentText);
}

void ThreadReplyCheck::checkState()
{
    if (isFinished()) {
        return;
    }
    switch (mPhase) {
    case Phase::WaitingForParent: {
        const Message message = findConfirmedMessageByText(mParentText);
        if (message.messageId().isEmpty()) {
            return; // not arrived yet
        }
        mParentId = message.messageId();
        mPhase = Phase::WaitingForReply;
        mAccount->replyOnThread(mRoomId, mParentId, mReplyText);
        break;
    }
    case Phase::WaitingForReply: {
        auto model = messageModel();
        if (!model) {
            return;
        }
        const int rows = model->rowCount();
        for (int row = 0; row < rows; ++row) {
            const QByteArray messageId = model->messageIdFromIndex(row);
            const Message message = model->findMessageById(messageId);
            if (message.pendingMessage() || message.text() != mReplyText) {
                continue;
            }
            if (message.threadMessageId() == mParentId) {
                reportPassed();
            } else {
                reportFailed(i18n("The reply arrived but is not linked to the parent thread."));
            }
            deleteTestMessage(messageId);
            deleteTestMessage(mParentId);
            return;
        }
        break;
    }
    }
}

#include "moc_threadreplycheck.cpp"
