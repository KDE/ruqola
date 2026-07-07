/*
   SPDX-FileCopyrightText: 2026 David Faure <david.faure@kdab.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "reactioncheck.h"

#include "messages/reactions.h"
#include "model/messagesmodel.h"
#include "rocketchataccount.h"

#include <KLocalizedString>

using namespace Qt::Literals::StringLiterals;

QString ReactionCheck::name() const
{
    return i18n("Emoji reaction");
}

void ReactionCheck::doStart()
{
    if (!mAccount) {
        reportFailed(i18n("No account."));
        return;
    }
    mMarker = generateMarker();
    connectModel();
    mAccount->sendMessage(mRoomId, mMarker);
}

void ReactionCheck::checkState()
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
        mPhase = Phase::WaitingForReaction;
        mAccount->reactOnMessage(mMessageId, u":thumbsup:"_s, true);
        break;
    }
    case Phase::WaitingForReaction: {
        auto model = messageModel();
        if (!model) {
            return;
        }
        const Message message = model->findMessageById(mMessageId);
        if (message.reactions() && !message.reactions()->isEmpty()) {
            reportPassed();
            deleteTestMessage(mMessageId);
        }
        break;
    }
    }
}

#include "moc_reactioncheck.cpp"
