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

#include <QAbstractTextDocumentLayout>
#include <QPainter>
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

void MessageDelegateHelperBase::drawDescription(const MessageAttachment &msgAttach, QRect descriptionRect, QPainter *painter, int topPos) const
{
    auto *doc = documentDescriptionForIndex(msgAttach, descriptionRect.width());
    if (!doc) {
        return;
    }

    painter->save();
    painter->translate(descriptionRect.left(), topPos);
    const QRect clip(0, 0, descriptionRect.width(), descriptionRect.height());

    // Same as pDoc->drawContents(painter, clip) but we also set selections
    QAbstractTextDocumentLayout::PaintContext ctx;
    //FIXME ctx.selections = selections;
    if (clip.isValid()) {
        painter->setClipRect(clip);
        ctx.clip = clip;
    }
    doc->documentLayout()->draw(painter, ctx);
    painter->restore();
}

QSize MessageDelegateHelperBase::documentDescriptionForIndexSize(const MessageAttachment &msgAttach, int width) const
{
    auto *doc = documentDescriptionForIndex(msgAttach, width);
    return doc ? QSize(doc->idealWidth(), doc->size().height()) : QSize();
}

QTextDocument *MessageDelegateHelperBase::documentDescriptionForIndex(const MessageAttachment &msgAttach, int width) const
{
    const QString attachmentId = msgAttach.attachmentId();
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
    const QString contextString = textConverter.convertMessageText(description, rcAccount->userName(), {}, rcAccount->highlightWords());
    auto doc = MessageDelegateUtils::createTextDocument(false, contextString, width);
    auto ret = doc.get();
    mDocumentCache.insert(attachmentId, std::move(doc));
    return ret;
}
