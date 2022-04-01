/*
   SPDX-FileCopyrightText: 2021 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "textselection.h"
#include "messages/message.h"
#include "messages/messageattachment.h"
#include "model/messagemodel.h"

#include <QTextCursor>
#include <QTextDocument>
#include <QTextDocumentFragment>

TextSelection::TextSelection()
{
}

DocumentFactoryInterface::~DocumentFactoryInterface() = default;

bool TextSelection::hasSelection() const
{
    return mStartIndex.isValid() && mEndIndex.isValid() && mStartPos > -1 && mEndPos > -1 && mStartPos != mEndPos;
}

TextSelection::OrderedPositions TextSelection::orderedPositions() const
{
    Q_ASSERT(!mStartIndex.isValid() || !mEndIndex.isValid() || mStartIndex.model() == mEndIndex.model());
    TextSelection::OrderedPositions ret{mStartIndex.row(), mStartPos, mEndIndex.row(), mEndPos};
    if (ret.fromRow > ret.toRow) {
        std::swap(ret.fromRow, ret.toRow);
        std::swap(ret.fromCharPos, ret.toCharPos);
    }
    return ret;
}

QString TextSelection::selectedText(Format format, DocumentFactoryInterface *factory) const
{
    if (!hasSelection()) {
        return {};
    }
    const OrderedPositions ordered = orderedPositions();
    QString str;
    for (int row = ordered.fromRow; row <= ordered.toRow; ++row) {
        const QModelIndex index = QModelIndex(mStartIndex).siblingAtRow(row);
        QTextDocument *doc = factory->documentForIndex(index);
        selectionText(ordered, format, row, index, doc, str);
    }
    return str;
}

void TextSelection::selectionText(const OrderedPositions ordered, Format format, int row, const QModelIndex &index, QTextDocument *doc, QString &str) const
{
    if (doc) {
        const QTextCursor cursor = selectionForIndex(index, doc);
        const QTextDocumentFragment fragment(cursor);
        str += format == Text ? fragment.toPlainText() : fragment.toHtml();
        if (row < ordered.toRow) {
            str += QLatin1Char('\n');
        }
    }
}

DocumentFactoryInterface *TextSelection::textHelperFactory() const
{
    return mTextHelperFactory;
}

const QVector<DocumentFactoryInterface *> &TextSelection::attachmentFactories() const
{
    return mAttachmentFactories;
}

void TextSelection::setAttachmentFactories(const QVector<DocumentFactoryInterface *> &newAttachmentFactories)
{
    mAttachmentFactories = newAttachmentFactories;
}

void TextSelection::setTextHelperFactory(DocumentFactoryInterface *newTextHelperFactory)
{
    mTextHelperFactory = newTextHelperFactory;
}

QString TextSelection::selectedText(Format format) const
{
    if (!hasSelection()) {
        return {};
    }
    const OrderedPositions ordered = orderedPositions();
    QString str;
    for (int row = ordered.fromRow; row <= ordered.toRow; ++row) {
        const QModelIndex index = QModelIndex(mStartIndex).siblingAtRow(row);
        QTextDocument *doc = mTextHelperFactory ? mTextHelperFactory->documentForIndex(index) : nullptr;
        selectionText(ordered, format, row, index, doc, str);
        const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();
        if (message) {
            const auto attachements = message->attachements();
            for (const auto &att : attachements) {
                for (auto factory : std::as_const(mAttachmentFactories)) {
                    // TODO verify if it's startattach/
                    doc = factory->documentForIndex(att);
                    if (doc) {
                        selectionText(ordered, format, row, index, doc, str);
                        break;
                    }
                }
            }
        }
    }
    return str;
}

bool TextSelection::contains(const QModelIndex &index, int charPos) const
{
    if (!hasSelection())
        return false;
    Q_ASSERT(index.model() == mStartIndex.model());
    const int row = index.row();
    const OrderedPositions ordered = orderedPositions();
    if (row == ordered.fromRow) {
        if (row == ordered.toRow) // single line selection
            return ordered.fromCharPos <= charPos && charPos <= ordered.toCharPos;
        return ordered.fromCharPos <= charPos;
    } else if (row == ordered.toRow) {
        return charPos <= ordered.toCharPos;
    } else {
        return row > ordered.fromRow && row < ordered.toRow;
    }
}

QTextCursor TextSelection::selectionForIndex(const QModelIndex &index, QTextDocument *doc) const
{
    if (!hasSelection())
        return {};
    Q_ASSERT(index.model() == mStartIndex.model());
    Q_ASSERT(index.model() == mEndIndex.model());

    const OrderedPositions ordered = orderedPositions();

    QTextCursor cursor(doc);
    const int row = index.row();
    if (row == ordered.fromRow)
        cursor.setPosition(ordered.fromCharPos);
    else if (row > ordered.fromRow)
        cursor.setPosition(0);
    else
        return {};
    if (row == ordered.toRow)
        cursor.setPosition(ordered.toCharPos, QTextCursor::KeepAnchor);
    else if (row < ordered.toRow)
        cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    else
        return {};
    return cursor;
}

void TextSelection::clear()
{
    const QModelIndex index = mStartIndex;
    const OrderedPositions ordered = orderedPositions();

    mStartIndex = QPersistentModelIndex{};
    mEndIndex = QPersistentModelIndex{};
    mStartPos = -1;
    mEndPos = -1;
    mStartMsgAttach = {};
    mEndMsgAttach = {};

    // Repaint indexes that are no longer selected
    if (ordered.fromRow > -1) {
        if (ordered.toRow > -1) {
            for (int row = ordered.fromRow; row <= ordered.toRow; ++row) {
                Q_EMIT repaintNeeded(index.siblingAtRow(row));
            }
        } else {
            Q_EMIT repaintNeeded(index);
        }
    }
}

void TextSelection::setStart(const QModelIndex &index, int charPos, const MessageAttachment &msgAttach)
{
    clear();
    Q_ASSERT(index.isValid());
    mStartIndex = index;
    mStartPos = charPos;
    mStartMsgAttach = msgAttach;
}

void TextSelection::setEnd(const QModelIndex &index, int charPos, const MessageAttachment &msgAttach)
{
    int from = mEndIndex.row();
    int to = index.row();
    if (from != -1 && from != to) {
        mEndIndex = index;

        if (from > to) { // reducing (moving the end up)
            std::swap(from, to);
            ++from; // 'from' is @p index, it's under the mouse anyway
        } else { // extending (moving the down)
            --to; // 'to' is @p index, it's under the mouse anyway
        }

        // Repaint indexes that are no longer selected
        // or that got selected when moving the mouse down fast
        for (int row = from; row <= to; ++row) {
            Q_EMIT repaintNeeded(index.siblingAtRow(row));
        }
    }

    Q_ASSERT(index.isValid());
    mEndIndex = index;
    mEndPos = charPos;
    mEndMsgAttach = msgAttach;
}

void TextSelection::selectWordUnderCursor(const QModelIndex &index, int charPos, DocumentFactoryInterface *factory)
{
    QTextDocument *doc = factory->documentForIndex(index);
    QTextCursor cursor(doc);
    cursor.setPosition(charPos);
    clear();
    cursor.select(QTextCursor::WordUnderCursor);
    mStartIndex = index;
    mEndIndex = index;
    mStartPos = cursor.selectionStart();
    mEndPos = cursor.selectionEnd();
    mStartMsgAttach = {};
    mEndMsgAttach = {};
}

void TextSelection::selectWordUnderCursor(const QModelIndex &index, const MessageAttachment &msgAttach, int charPos, DocumentFactoryInterface *factory)
{
    QTextDocument *doc = factory->documentForIndex(msgAttach);
    QTextCursor cursor(doc);
    cursor.setPosition(charPos);
    clear();
    cursor.select(QTextCursor::WordUnderCursor);
    mStartIndex = index;
    mEndIndex = index;
    mStartPos = cursor.selectionStart();
    mEndPos = cursor.selectionEnd();
    mStartMsgAttach = msgAttach;
    mEndMsgAttach = msgAttach;
    qDebug() << " mEndPos " << mEndPos << "mStartPos  " << mStartPos << "doc" << doc->toPlainText() << " cusor" << cursor.selectedText();
}

void TextSelection::selectMessage(const QModelIndex &index, DocumentFactoryInterface *factory)
{
    Q_ASSERT(index.isValid());
    clear();
    mStartIndex = index;
    mEndIndex = index;
    mStartPos = 0;
    QTextDocument *doc = nullptr;
#ifdef ATTACHMENT_SUPPORT_ACTIVATED // Bug at the moment as we call it with     DocumentFactoryInterface *factory == messagedelegatehelpertext all the time and
    // not with attachment
    if (mStartMsgAttach.isValid()) {
        doc = factory->documentForIndex(mStartMsgAttach);
    } else {
        doc = factory->documentForIndex(index);
    }
#else
    doc = factory->documentForIndex(index);
#endif
    if (!doc) {
        return;
    }
    mEndPos = doc->characterCount() - 1;
}

void TextSelection::selectMessage(const QModelIndex &index)
{
    Q_ASSERT(index.isValid());
    clear();
    mStartIndex = index;
    mEndIndex = index;
    mStartPos = 0;
#ifdef ATTACHMENT_SUPPORT_ACTIVATED // Bug at the moment as we call it with     DocumentFactoryInterface *factory == messagedelegatehelpertext all the time and
    QTextDocument *doc = nullptr;
    // not with attachment
    if (mStartMsgAttach.isValid()) {
        doc = factory->documentForIndex(mStartMsgAttach);
    } else {
        doc = factory->documentForIndex(index);
    }
    doc = factory->documentForIndex(index);
    if (!doc) {
        return;
    }
    mEndPos = doc->characterCount() - 1;
#endif
}
