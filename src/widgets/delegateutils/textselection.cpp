/*
   SPDX-FileCopyrightText: 2021 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "textselection.h"
#include "messages/message.h"
#include "model/messagemodel.h"
#include "ruqolawidgets_selection_debug.h"

#include <QTextCursor>
#include <QTextDocument>
#include <QTextDocumentFragment>

TextSelection::TextSelection() = default;

DocumentFactoryInterface::~DocumentFactoryInterface() = default;

bool TextSelection::hasSelection() const
{
    return mStartIndex.isValid() && mEndIndex.isValid() && ((mStartPos > -1 && mEndPos > -1 && mStartPos != mEndPos) || !mAttachmentSelection.isEmpty());
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

void TextSelection::selectionText(const OrderedPositions ordered,
                                  Format format,
                                  int row,
                                  const QModelIndex &index,
                                  QTextDocument *doc,
                                  QString &str,
                                  const MessageAttachment &att) const
{
    const QTextCursor cursor = selectionForIndex(index, doc, att);
    const QTextDocumentFragment fragment(cursor);
    str += format == Text ? fragment.toPlainText() : fragment.toHtml();
    if (row < ordered.toRow) {
        str += QLatin1Char('\n');
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
        if (doc) {
            selectionText(ordered, format, row, index, doc, str);
        }
        const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();
        if (message) {
            const auto attachments = message->attachments();
            for (const auto &att : attachments) {
                for (auto factory : std::as_const(mAttachmentFactories)) {
                    doc = factory->documentForIndex(att);
                    if (doc) {
                        if (!str.endsWith(QLatin1Char('\n'))) {
                            str += QLatin1Char('\n');
                        }
                        selectionText(ordered, format, row, index, doc, str, att);
                        break;
                    }
                }
            }
        }
    }
    return str;
}

bool TextSelection::contains(const QModelIndex &index, int charPos, const MessageAttachment &att) const
{
    if (!hasSelection())
        return false;
    Q_ASSERT(index.model() == mStartIndex.model());
    // TODO implement check attachment
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

QTextCursor TextSelection::selectionForIndex(const QModelIndex &index, QTextDocument *doc, const MessageAttachment &att) const
{
    if (!hasSelection())
        return {};
    Q_ASSERT(index.model() == mStartIndex.model());
    Q_ASSERT(index.model() == mEndIndex.model());

    if (att.isValid() && mAttachmentSelection.isEmpty()) {
        return {};
    }
    const OrderedPositions ordered = orderedPositions();
    int fromCharPos = ordered.fromCharPos;
    int toCharPos = ordered.toCharPos;
    // qDebug() << "BEFORE toCharPos" << toCharPos << " fromCharPos " << fromCharPos;
    QTextCursor cursor(doc);

    if (att.isValid()) {
        for (const AttachmentSelection &attSelection : std::as_const(mAttachmentSelection)) {
            if (attSelection.attachment == att) {
                fromCharPos = attSelection.fromCharPos;
                toCharPos = attSelection.toCharPos;
                // qDebug() << "ATTACHNMENT toCharPos" << toCharPos << " fromCharPos " << fromCharPos;
                break;
            }
        }
    }
    // qDebug() << "AFTER toCharPos" << toCharPos << " fromCharPos " << fromCharPos;
    const int row = index.row();
    if (row == ordered.fromRow)
        cursor.setPosition(qMax(fromCharPos, 0));
    else if (row > ordered.fromRow)
        cursor.setPosition(0);
    else
        return {};
    if (row == ordered.toRow)
        cursor.setPosition(qMax(toCharPos, 0), QTextCursor::KeepAnchor);
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
    mAttachmentSelection.clear();

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
    if (msgAttach.isValid()) {
        AttachmentSelection selection;
        selection.fromCharPos = charPos;
        selection.attachment = msgAttach;
        mAttachmentSelection.append(std::move(selection));
        // qDebug() << " start selection is in attachment ";
    } else {
        mStartPos = charPos;
    }
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
    if (msgAttach.isValid()) {
        const auto countAtt{mAttachmentSelection.count()};
        for (int i = 0; i < countAtt; ++i) {
            if (mAttachmentSelection.at(i).attachment == msgAttach) {
                AttachmentSelection attachmentSelectFound = mAttachmentSelection.takeAt(i);
                attachmentSelectFound.toCharPos = charPos;
                mAttachmentSelection.append(attachmentSelectFound);
                return;
            }
        }

        AttachmentSelection selection;
        selection.toCharPos = charPos;
        selection.attachment = msgAttach;
        mAttachmentSelection.append(std::move(selection));
    } else {
        mEndPos = charPos;
    }
}

void TextSelection::selectWord(const QModelIndex &index, int charPos, QTextDocument *doc)
{
    QTextCursor cursor(doc);
    cursor.setPosition(charPos);
    clear();
    cursor.select(QTextCursor::WordUnderCursor);
    mStartIndex = index;
    mEndIndex = index;
    mStartPos = cursor.selectionStart();
    mEndPos = cursor.selectionEnd();
}

void TextSelection::selectWordUnderCursor(const QModelIndex &index, int charPos, DocumentFactoryInterface *factory, const MessageAttachment &msgAttach)
{
    if (!factory) {
        qCWarning(RUQOLAWIDGETS_SELECTION_LOG) << " Factory is null. It's a bug";
        return;
    }
    if (msgAttach.isValid()) {
        QTextDocument *doc = factory->documentForIndex(msgAttach);
        selectWord(index, charPos, doc);

        AttachmentSelection selection;
        selection.fromCharPos = mStartPos;
        selection.toCharPos = mEndPos;
        selection.attachment = msgAttach;
        mAttachmentSelection.append(std::move(selection));
    } else {
        QTextDocument *doc = factory->documentForIndex(index);
        selectWord(index, charPos, doc);
    }
}

void TextSelection::selectMessage(const QModelIndex &index)
{
    Q_ASSERT(index.isValid());
    clear();
    mStartIndex = index;
    mEndIndex = index;
    mStartPos = 0;
    QTextDocument *doc = mTextHelperFactory ? mTextHelperFactory->documentForIndex(index) : nullptr;
    if (doc) {
        mEndPos = doc->characterCount() - 1;
    }
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();
    if (message) {
        const auto attachments = message->attachments();
        for (const auto &att : attachments) {
            for (auto factory : std::as_const(mAttachmentFactories)) {
                doc = factory->documentForIndex(att);
                if (doc) {
                    AttachmentSelection selection;
                    selection.attachment = att;
                    selection.fromCharPos = 0;
                    selection.toCharPos = doc->characterCount() - 1;
                    mAttachmentSelection.append(std::move(selection));
                }
            }
        }
    }
}
