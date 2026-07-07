/*
   SPDX-FileCopyrightText: 2026 David Faure <david.faure@kdab.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "favoritemessagecheck.h"

#include "chat/starmessagejob.h"
#include "connection.h"
#include "model/messagesmodel.h"
#include "rocketchataccount.h"

#include <KLocalizedString>

QString FavoriteMessageCheck::name() const
{
    return i18n("Favorite (star) message");
}

void FavoriteMessageCheck::doStart()
{
    if (!mAccount) {
        reportFailed(i18n("No account."));
        return;
    }
    mMarker = generateMarker();
    connectModel();
    mAccount->sendMessage(mRoomId, mMarker);
}

void FavoriteMessageCheck::checkState()
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
        mPhase = Phase::WaitingForStar;
        auto job = new RocketChatRestApi::StarMessageJob(this);
        mAccount->restApi()->initializeRestApiJob(job);
        job->setMessageId(mMessageId);
        job->setStarMessage(true);
        if (!job->start()) {
            reportFailed(i18n("Could not start the star-message request."));
        }
        break;
    }
    case Phase::WaitingForStar: {
        auto model = messageModel();
        if (!model) {
            return;
        }
        const Message message = model->findMessageById(mMessageId);
        if (message.isStarred()) {
            reportPassed();
            deleteTestMessage(mMessageId);
        }
        break;
    }
    }
}

#include "moc_favoritemessagecheck.cpp"
