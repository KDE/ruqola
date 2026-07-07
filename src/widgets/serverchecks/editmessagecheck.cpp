/*
   SPDX-FileCopyrightText: 2026 David Faure <david.faure@kdab.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "editmessagecheck.h"

#include "model/messagesmodel.h"
#include "rocketchataccount.h"

#include <KLocalizedString>

QString EditMessageCheck::name() const
{
    return i18n("Edit message");
}

void EditMessageCheck::doStart()
{
    if (!mAccount) {
        reportFailed(i18n("No account."));
        return;
    }
    mInitialText = generateMarker();
    mEditedText = mInitialText + i18n(" (edited)");
    connectModel();
    mAccount->sendMessage(mRoomId, mInitialText);
}

void EditMessageCheck::checkState()
{
    if (isFinished()) {
        return;
    }
    switch (mPhase) {
    case Phase::WaitingForInitial: {
        const Message message = findConfirmedMessageByText(mInitialText);
        if (message.messageId().isEmpty()) {
            return; // not arrived yet
        }
        mMessageId = message.messageId();
        mPhase = Phase::WaitingForEdit;
        mAccount->updateMessage(mRoomId, mMessageId, mEditedText);
        break;
    }
    case Phase::WaitingForEdit: {
        auto model = messageModel();
        if (!model) {
            return;
        }
        const Message message = model->findMessageById(mMessageId);
        if (message.text() != mEditedText) {
            return; // edit not applied yet
        }
        if (!message.wasEdited()) {
            reportFailed(i18n("Edited text arrived but the message is not flagged as edited."));
        } else {
            reportPassed();
        }
        deleteTestMessage(mMessageId);
        break;
    }
    }
}

#include "moc_editmessagecheck.cpp"
