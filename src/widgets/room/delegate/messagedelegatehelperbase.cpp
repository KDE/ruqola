/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "messagedelegatehelperbase.h"
#include "messagedelegateutils.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "textconverter.h"

#include <QAbstractTextDocumentLayout>
#include <QDrag>
#include <QMimeData>
#include <QPainter>
#include <QRect>
#include <QStyleOptionViewItem>

MessageDelegateHelperBase::~MessageDelegateHelperBase() = default;

bool MessageDelegateHelperBase::handleMouseEvent(const MessageAttachment &msgAttach,
                                                 QMouseEvent *mouseEvent,
                                                 QRect attachmentsRect,
                                                 const QStyleOptionViewItem &option,
                                                 const QModelIndex &index)
{
    Q_UNUSED(msgAttach)
    Q_UNUSED(option)

    switch (mouseEvent->type()) {
    case QEvent::MouseButtonPress:
        if (attachmentsRect.contains(mouseEvent->pos())) {
            mCurrentIndex = index;
            mMightStartDrag = true;
        } else {
            mMightStartDrag = false;
            mCurrentIndex = QModelIndex();
        }
        break;
    default:
        break;
    };
    return false;
}

bool MessageDelegateHelperBase::maybeStartDrag(const MessageAttachment &msgAttach,
                                               QMouseEvent *mouseEvent,
                                               QRect attachmentsRect,
                                               const QStyleOptionViewItem &option,
                                               const QModelIndex &index)
{
    if (!mMightStartDrag || index != mCurrentIndex || !attachmentsRect.contains(mouseEvent->pos())) {
        return false;
    }

    auto mimeData = new QMimeData;
    mimeData->setUrls({Ruqola::self()->rocketChatAccount()->attachmentUrlFromLocalCache(msgAttach.link())});

    auto drag = new QDrag(const_cast<QWidget *>(option.widget));
    drag->setMimeData(mimeData);
    drag->exec(Qt::CopyAction);

    return true;
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
    // FIXME ctx.selections = selections;
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
        if (!qFuzzyCompare(ret->textWidth(), width)) {
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
    QString needUpdateMessageId; // TODO use it ?
    const QString contextString = TextConverter::convertMessageText(description,
                                                                    rcAccount->userName(),
                                                                    {},
                                                                    rcAccount->highlightWords(),
                                                                    rcAccount->emojiManager(),
                                                                    rcAccount->messageCache(),
                                                                    needUpdateMessageId);
    auto doc = MessageDelegateUtils::createTextDocument(false, contextString, width);
    auto ret = doc.get();
    mDocumentCache.insert(attachmentId, std::move(doc));
    return ret;
}
