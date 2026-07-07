/*
   SPDX-FileCopyrightText: 2026 David Faure <david.faure@kdab.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "quotemessagecheck.h"

#include "model/messagesmodel.h"
#include "rocketchataccount.h"
#include "room.h"
#include "room/roomutil.h"

#include <KLocalizedString>

using namespace Qt::Literals::StringLiterals;

QString QuoteMessageCheck::name() const
{
    return i18n("Quote message");
}

void QuoteMessageCheck::doStart()
{
    if (!mAccount) {
        reportFailed(i18n("No account."));
        return;
    }
    mOriginalText = generateMarker();
    mQuoteMarker = mOriginalText + i18n(" (quote)");
    connectModel();
    mAccount->sendMessage(mRoomId, mOriginalText);
}

void QuoteMessageCheck::checkState()
{
    if (isFinished()) {
        return;
    }
    switch (mPhase) {
    case Phase::WaitingForOriginal: {
        const Message message = findConfirmedMessageByText(mOriginalText);
        if (message.messageId().isEmpty()) {
            return; // not arrived yet
        }
        mOriginalId = message.messageId();
        mPhase = Phase::WaitingForQuote;

        Room *room = mAccount->room(mRoomId);
        if (!room) {
            reportFailed(i18n("Could not find the room."));
            return;
        }
        QString permalink = mAccount->serverUrl() + u'/' + RoomUtil::generatePermalink(QString::fromLatin1(mOriginalId), room->name(), room->channelType());
        if (!permalink.startsWith(u"https://"_s)) {
            permalink.prepend(u"https://"_s);
        }
        // This is how Ruqola itself sends a quote (see MessageLineWidget).
        mAccount->sendMessage(mRoomId, u"[ ](%1) %2"_s.arg(permalink, mQuoteMarker));
        break;
    }
    case Phase::WaitingForQuote: {
        auto model = messageModel();
        if (!model) {
            return;
        }
        // Ruqola drops the server's message_link attachment and renders the quote client-side
        // from the permalink in the text, so verify the quoting message round-trips with the
        // permalink still pointing at the original message.
        const QString originalId = QString::fromLatin1(mOriginalId);
        const int rows = model->rowCount();
        for (int row = 0; row < rows; ++row) {
            const QByteArray messageId = model->messageIdFromIndex(row);
            const Message message = model->findMessageById(messageId);
            if (message.pendingMessage()) {
                continue;
            }
            const QString text = message.text();
            if (text.contains(mQuoteMarker) && text.contains(originalId)) {
                reportPassed();
                deleteTestMessage(messageId);
                deleteTestMessage(mOriginalId);
                return;
            }
        }
        break;
    }
    }
}

#include "moc_quotemessagecheck.cpp"
