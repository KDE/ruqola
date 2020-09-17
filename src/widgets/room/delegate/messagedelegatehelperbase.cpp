/*
   Copyright (c) 2020 David Faure <faure@kde.org>

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

#include "messagedelegatehelperbase.h"
#include "textconverter.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include "messagedelegateutils.h"

#include <QRect>

MessageDelegateHelperBase::~MessageDelegateHelperBase()
{
}

bool MessageDelegateHelperBase::handleMouseEvent(const MessageAttachment &msgAttach, QMouseEvent *mouseEvent, QRect attachmentsRect, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    Q_UNUSED(msgAttach)
    Q_UNUSED(mouseEvent)
    Q_UNUSED(attachmentsRect)
    Q_UNUSED(option)
    Q_UNUSED(index)
    return false;
}

QTextDocument *MessageDelegateHelperBase::documentDescriptionForIndex(const MessageAttachment &msgAttach, int width) const
{
    const QString attachmentId = msgAttach.attachementId();
    auto it = mDocumentCache.find(attachmentId);
    if (it != mDocumentCache.end()) {
        auto ret = it->value.get();
        if (ret->textWidth() != width) {
            ret->setTextWidth(width);
        }
        return ret;
    }

    const QString description = msgAttach.description();

    if (description.isEmpty()) {
        return nullptr;
    }
    // Use TextConverter in case it starts with a [](URL) reply marker
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    TextConverter textConverter(rcAccount->emojiManager());
    const QString contextString = textConverter.convertMessageText(description, rcAccount->userName(), {});
    auto doc = MessageDelegateUtils::createTextDocument(false, contextString, width);
    auto ret = doc.get();
    mDocumentCache.insert(attachmentId, std::move(doc));
    return ret;
}
