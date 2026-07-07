/*
   SPDX-FileCopyrightText: 2026 David Faure <david.faure@kdab.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sendmessagecheck.h"

#include "rocketchataccount.h"

#include <KLocalizedString>

QString SendMessageCheck::name() const
{
    return i18n("Send message");
}

void SendMessageCheck::doStart()
{
    if (!mAccount) {
        reportFailed(i18n("No account."));
        return;
    }
    mMarker = generateMarker();
    connectModel();
    mAccount->sendMessage(mRoomId, mMarker);
}

void SendMessageCheck::checkState()
{
    if (isFinished()) {
        return;
    }
    const Message message = findConfirmedMessageByText(mMarker);
    if (message.messageId().isEmpty()) {
        return; // not arrived yet
    }
    deleteTestMessage(message.messageId());
    reportPassed();
}

#include "moc_sendmessagecheck.cpp"
