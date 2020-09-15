/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#include "messageattachmentdelegatehelpertext.h"
#include "messagedelegateutils.h"
#include "ruqolawidgets_debug.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include "dialogs/showvideodialog.h"
#include "common/delegatepaintutil.h"
#include "common/delegateutil.h"

#include <KLocalizedString>

#include <QAbstractItemView>
#include <QAbstractTextDocumentLayout>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPointer>
#include <QTextBlock>
#include <QStyleOptionViewItem>
#define DRAW_NOT_MULTILINE 0
MessageAttachmentDelegateHelperText::~MessageAttachmentDelegateHelperText()
{
}

void MessageAttachmentDelegateHelperText::draw(const MessageAttachment &msgAttach, QPainter *painter, QRect messageRect, const QModelIndex &index, const QStyleOptionViewItem &option) const
{
    const TextLayout layout = layoutText(msgAttach, option);
#if DRAW_NOT_MULTILINE
    Q_UNUSED(index);
    int y = messageRect.y();
    if (!layout.title.isEmpty()) {
        qDebug() << " draw title!!!!!!!!!!!!!!!!!!!!" << layout.title;
        y += option.fontMetrics.ascent();
        painter->drawText(messageRect.x(), y, layout.title);
    }
    if (!layout.text.isEmpty()) {
        painter->drawText(messageRect.x(), y + option.fontMetrics.ascent(), layout.text);
    }
#else
#if 1
    auto *doc = documentForIndex(msgAttach, messageRect.width());
    if (!doc) {
        return;
    }
#if 0
    QVector<QAbstractTextDocumentLayout::Selection> selections;
    if (index == mCurrentIndex) {
        QTextCharFormat selectionFormat;
        selectionFormat.setBackground(option.palette.brush(QPalette::Highlight));
        selectionFormat.setForeground(option.palette.brush(QPalette::HighlightedText));
        selections.append({mCurrentTextCursor, selectionFormat});
    }
    if (useItalicsForMessage(index)) {
        QTextCursor cursor(doc);
        cursor.select(QTextCursor::Document);
        QTextCharFormat format;
        format.setForeground(Qt::gray); //TODO use color from theme.
        cursor.mergeCharFormat(format);
    }
#endif

    //qDebug() << " draw !!!!!!!!!!!!!!!!!!!!" << layout.text;
    painter->save();
    painter->translate(messageRect.left(), messageRect.top());
    const QRect clip(0, 0, messageRect.width(), messageRect.height());

    // Same as pDoc->drawContents(painter, clip) but we also set selections
    QAbstractTextDocumentLayout::PaintContext ctx;
    //FIXME ctx.selections = selections;
    if (clip.isValid()) {
        painter->setClipRect(clip);
        ctx.clip = clip;
    }
    doc->documentLayout()->draw(painter, ctx);
    painter->restore();
#endif
#endif
    //TODO add fields
}

QSize MessageAttachmentDelegateHelperText::sizeHint(const MessageAttachment &msgAttach, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const
{
#if DRAW_NOT_MULTILINE
    Q_UNUSED(index)
    Q_UNUSED(maxWidth)
    const TextLayout layout = layoutText(msgAttach, option);
    int height = layout.textSize.height() + DelegatePaintUtil::margin();
    int descriptionWidth = 0;
    if (!layout.title.isEmpty()) {
        descriptionWidth = layout.titleSize.width();
        height += layout.titleSize.height() + DelegatePaintUtil::margin();
    }
    return QSize(qMax(qMax(0, layout.textSize.width()), descriptionWidth),
                 height);
#else
    auto *doc = documentForIndex(msgAttach, maxWidth);
    if (!doc) {
        return QSize();
    }
    const QSize size(doc->idealWidth(), doc->size().height()); // do the layouting, required by lineAt(0) below

    const QTextLine &line = doc->firstBlock().layout()->lineAt(0);
    //FIXME *pBaseLine = line.y() + line.ascent(); // relative

    return size;
#endif
}

bool MessageAttachmentDelegateHelperText::handleMouseEvent(const MessageAttachment &msgAttach, QMouseEvent *mouseEvent, QRect attachmentsRect, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    Q_UNUSED(attachmentsRect);
    Q_UNUSED(option);
    Q_UNUSED(index);
    Q_UNUSED(msgAttach);
    if (mouseEvent->type() == QEvent::MouseButtonRelease) {
        //TODO ???
//        const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();
//        const QPoint pos = mouseEvent->pos();

//        TextLayout layout = layoutText(message, option);
    }
    return false;
}

MessageAttachmentDelegateHelperText::TextLayout MessageAttachmentDelegateHelperText::layoutText(const MessageAttachment &msgAttach, const QStyleOptionViewItem &option) const
{
    TextLayout layout;
    layout.title = msgAttach.title();
    layout.titleSize = option.fontMetrics.size(Qt::TextSingleLine, layout.title);
    layout.text = msgAttach.text();
    layout.textSize = option.fontMetrics.size(Qt::TextSingleLine, layout.text);
    return layout;
}

QTextDocument *MessageAttachmentDelegateHelperText::documentForIndex(const MessageAttachment &msgAttach, int width) const
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

    const QString text = msgAttach.text();
    if (text.isEmpty()) {
        return nullptr;
    }

    auto doc = MessageDelegateUtils::createTextDocument(false, text, width);
    auto ret = doc.get();
    mDocumentCache.insert(attachmentId, std::move(doc));
    return ret;
}

