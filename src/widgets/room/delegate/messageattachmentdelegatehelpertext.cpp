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
#include "textconverter.h"

#include <KLocalizedString>

#include <QAbstractItemView>
#include <QAbstractTextDocumentLayout>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPointer>
#include <QTextBlock>
#include <QStyleOptionViewItem>

MessageAttachmentDelegateHelperText::~MessageAttachmentDelegateHelperText()
{
}

void MessageAttachmentDelegateHelperText::draw(const MessageAttachment &msgAttach, QPainter *painter, QRect messageRect, const QModelIndex &index, const QStyleOptionViewItem &option) const
{
    Q_UNUSED(index);
    Q_UNUSED(option);

    //painter->drawRect(messageRect);

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

    //const QIcon hideShowIcon = QIcon::fromTheme(layout.isShown ? QStringLiteral("visibility") : QStringLiteral("hint"));
    //Add icon for hidding attachment

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
    //TODO add fields
}

QSize MessageAttachmentDelegateHelperText::sizeHint(const MessageAttachment &msgAttach, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const
{
    Q_UNUSED(index);
    Q_UNUSED(option);
    auto *doc = documentForIndex(msgAttach, maxWidth);
    if (!doc) {
        return QSize();
    }
    const QSize size(doc->idealWidth(), doc->size().height()); // do the layouting, required by lineAt(0) below
    return size;
}

bool MessageAttachmentDelegateHelperText::handleMouseEvent(const MessageAttachment &msgAttach, QMouseEvent *mouseEvent, QRect attachmentsRect, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    Q_UNUSED(attachmentsRect);
    Q_UNUSED(option);
    Q_UNUSED(index);
    Q_UNUSED(msgAttach);
    if (mouseEvent->type() == QEvent::MouseButtonRelease) {
        //TODO ???
    }
    //Implement click on url
    return false;
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
    const QString title = msgAttach.title();

    if (text.isEmpty() && title.isEmpty()) {
        return nullptr;
    }
    // Use TextConverter in case it starts with a [](URL) reply marker
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    TextConverter textConverter(rcAccount->emojiManager());
    //Add bold for title
    const QString fullText = title.isEmpty() ? text : (title + QLatin1Char('\n') + text);
    const QString contextString = textConverter.convertMessageText(fullText, rcAccount->userName(), {});
    auto doc = MessageDelegateUtils::createTextDocument(false, contextString, width);
    auto ret = doc.get();
    mDocumentCache.insert(attachmentId, std::move(doc));
    return ret;
}
