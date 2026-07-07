/*
   SPDX-FileCopyrightText: 2026 David Faure <david.faure@kdab.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "linkpreviewcheck.h"

#include "messages/messageurl.h"
#include "messages/messageurls.h"
#include "model/messagesmodel.h"
#include "rocketchataccount.h"

#include <KLocalizedString>

using namespace Qt::Literals::StringLiterals;

QString LinkPreviewCheck::name() const
{
    return i18n("Link preview (URL unfurl)");
}

int LinkPreviewCheck::watchdogTimeoutMs() const
{
    // The server fetches and unfurls the URL asynchronously, then edits the message.
    return 60000;
}

void LinkPreviewCheck::doStart()
{
    if (!mAccount) {
        reportFailed(i18n("No account."));
        return;
    }
    mMarker = generateMarker();
    connectModel();
    mAccount->sendMessage(mRoomId, mMarker + u" https://www.kde.org"_s);
}

void LinkPreviewCheck::checkState()
{
    if (isFinished()) {
        return;
    }
    auto model = messageModel();
    if (!model) {
        return;
    }
    const int rows = model->rowCount();
    for (int row = 0; row < rows; ++row) {
        const QByteArray messageId = model->messageIdFromIndex(row);
        const Message message = model->findMessageById(messageId);
        if (message.pendingMessage() || !message.text().contains(mMarker) || !message.urls()) {
            continue;
        }
        const QList<MessageUrl> urls = message.urls()->messageUrls();
        for (const MessageUrl &url : urls) {
            // A populated preview means the server successfully unfurled the URL
            // (the raw URL is always present; the meta fields only after unfurling).
            if (!url.pageTitle().isEmpty() || !url.description().isEmpty() || !url.siteName().isEmpty()) {
                reportPassed();
                deleteTestMessage(messageId);
                return;
            }
        }
    }
}

#include "moc_linkpreviewcheck.cpp"
