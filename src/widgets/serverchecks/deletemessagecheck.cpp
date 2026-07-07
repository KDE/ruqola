/*
   SPDX-FileCopyrightText: 2026 David Faure <david.faure@kdab.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "deletemessagecheck.h"

#include "model/messagesmodel.h"
#include "rocketchataccount.h"

#include <KLocalizedString>

QString DeleteMessageCheck::name() const
{
    return i18n("Delete message");
}

void DeleteMessageCheck::doStart()
{
    if (!mAccount) {
        reportFailed(i18n("No account."));
        return;
    }
    mMarker = generateMarker();
    connectModel();
    mAccount->sendMessage(mRoomId, mMarker);
}

void DeleteMessageCheck::checkState()
{
    if (isFinished()) {
        return;
    }
    switch (mPhase) {
    case Phase::WaitingForSend: {
        const Message message = findConfirmedMessageByText(mMarker);
        if (message.messageId().isEmpty()) {
            return; // not arrived yet
        }
        mMessageId = message.messageId();
        mPhase = Phase::WaitingForDelete;
        deleteTestMessage(mMessageId);
        break;
    }
    case Phase::WaitingForDelete: {
        auto model = messageModel();
        if (!model) {
            return;
        }
        if (model->findMessageById(mMessageId).messageId().isEmpty()) {
            reportPassed(); // gone from the model
        }
        break;
    }
    }
}

#include "moc_deletemessagecheck.cpp"
