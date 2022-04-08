/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagedelegateutils.h"
#include "model/messagemodel.h"
#include "textselection.h"

#include <QAbstractItemModel>
#include <QAbstractTextDocumentLayout>
#include <QApplication>
#include <QClipboard>
#include <QModelIndex>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QTextFrame>
#include <QTextFrameFormat>
#include <QTextStream>

std::unique_ptr<QTextDocument> MessageDelegateUtils::createTextDocument(bool useItalic, const QString &text, int width)
{
    std::unique_ptr<QTextDocument> doc(new QTextDocument);
    doc->setHtml(text);
    doc->setTextWidth(width);
    QFont font = qApp->font();
    font.setItalic(useItalic);
    doc->setDefaultFont(font);
    QTextFrame *frame = doc->frameAt(0);
    QTextFrameFormat frameFormat = frame->frameFormat();
    frameFormat.setMargin(0);
    frame->setFrameFormat(frameFormat);
    return doc;
}

bool MessageDelegateUtils::generateToolTip(const QTextDocument *doc, const QPoint &pos, QString &formattedTooltip)
{
    const auto format = doc->documentLayout()->formatAt(pos);
    const auto tooltip = format.property(QTextFormat::TextToolTip).toString();
    const auto href = format.property(QTextFormat::AnchorHref).toString();
    if (tooltip.isEmpty() && (href.isEmpty() || href.startsWith(QLatin1String("ruqola:/")))) {
        return false;
    }

    generateToolTip(tooltip, href, formattedTooltip);
    return true;
}

void MessageDelegateUtils::generateToolTip(const QString &toolTip, const QString &href, QString &formattedTooltip)
{
    QTextStream stream(&formattedTooltip);
    auto addLine = [&](const QString &line) {
        if (!line.isEmpty()) {
            stream << QLatin1String("<p>") << line << QLatin1String("</p>");
        }
    };

    stream << QLatin1String("<qt>");
    addLine(toolTip);
    addLine(href);
    stream << QLatin1String("</qt>");
}

bool MessageDelegateUtils::useItalicsForMessage(const QModelIndex &index)
{
    const auto messageType = index.data(MessageModel::MessageType).value<Message::MessageType>();
    const bool isSystemMessage =
        messageType == Message::System && index.data(MessageModel::SystemMessageType).toString() != QStringLiteral("jitsi_call_started");
    return isSystemMessage;
}

bool MessageDelegateUtils::pendingMessage(const QModelIndex &index)
{
    return index.data(MessageModel::PendingMessage).toBool();
}

QVector<QAbstractTextDocumentLayout::Selection> MessageDelegateUtils::selection(TextSelection *selection,
                                                                                QTextDocument *doc,
                                                                                const QModelIndex &index,
                                                                                const QStyleOptionViewItem &option,
                                                                                const MessageAttachment &msgAttach)
{
    QVector<QAbstractTextDocumentLayout::Selection> selections;
    const QTextCursor selectionTextCursor = selection->selectionForIndex(index, doc, msgAttach);
    if (!selectionTextCursor.isNull()) {
        QTextCharFormat selectionFormat;
        selectionFormat.setBackground(option.palette.brush(QPalette::Highlight));
        selectionFormat.setForeground(option.palette.brush(QPalette::HighlightedText));
        selections.append({selectionTextCursor, selectionFormat});
    }
    if (MessageDelegateUtils::useItalicsForMessage(index) || MessageDelegateUtils::pendingMessage(index)) {
        QTextCursor cursor(doc);
        cursor.select(QTextCursor::Document);
        QTextCharFormat format;
        format.setForeground(Qt::gray); // TODO use color from theme.
        cursor.mergeCharFormat(format);
    }
    return selections;
}

void MessageDelegateUtils::drawSelection(QTextDocument *doc,
                                         QRect rect,
                                         int top,
                                         QPainter *painter,
                                         const QModelIndex &index,
                                         const QStyleOptionViewItem &option,
                                         TextSelection *selection,
                                         const MessageAttachment &msgAttach)
{
    const QVector<QAbstractTextDocumentLayout::Selection> selections = MessageDelegateUtils::selection(selection, doc, index, option, msgAttach);
    painter->save();
    painter->translate(rect.left(), top);
    const QRect clip(0, 0, rect.width(), rect.height());

    // Same as pDoc->drawContents(painter, clip) but we also set selections
    QAbstractTextDocumentLayout::PaintContext ctx;
    ctx.selections = selections;
    if (clip.isValid()) {
        painter->setClipRect(clip);
        ctx.clip = clip;
    }
    doc->documentLayout()->draw(painter, ctx);
    painter->restore();
}

void MessageDelegateUtils::setClipboardSelection(TextSelection *selection)
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    if (selection->hasSelection() && clipboard->supportsSelection()) {
        const QString text = selection->selectedText(TextSelection::Text);
        clipboard->setText(text, QClipboard::Selection);
    }
}

QSizeF MessageDelegateUtils::dprAwareSize(const QPixmap &pixmap)
{
    if (pixmap.isNull()) {
        return {0, 0}; // prevent division-by-zero
    }
    return pixmap.size() / pixmap.devicePixelRatioF();
}

qreal MessageDelegateUtils::basicMargin()
{
    return 8;
}
