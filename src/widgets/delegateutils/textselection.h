/*
   SPDX-FileCopyrightText: 2021 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "messages/block.h"
#include "messages/messageattachment.h"

#include <QObject>
#include <QPersistentModelIndex>

class QTextCursor;
class QTextDocument;
class Message;

class LIBRUQOLAWIDGETS_TESTS_EXPORT DocumentFactoryInterface
{
public:
    virtual ~DocumentFactoryInterface();
    /**
     * Creates (or retrieves from a cache) the QTextDocument for a given @p index.
     * @param width The width for layouting that QTextDocument. -1 if no layouting is desired (e.g. for converting to text or HTML)
     * @return the QTextDocument. Ownership remains with the cache, don't delete it.
     */
    Q_REQUIRED_RESULT virtual QTextDocument *documentForIndex(const QModelIndex &index) const = 0;
    Q_REQUIRED_RESULT virtual QTextDocument *documentForIndex(const MessageAttachment &msgAttach) const = 0;
    Q_REQUIRED_RESULT virtual QTextDocument *documentForIndex(const Block &block) const = 0;
};

class LIBRUQOLAWIDGETS_TESTS_EXPORT TextSelection : public QObject
{
    Q_OBJECT
public:
    TextSelection();
    Q_REQUIRED_RESULT bool hasSelection() const;
    enum Format {
        Text,
        Html,
    };
    Q_REQUIRED_RESULT QString selectedText(Format format) const;
    Q_REQUIRED_RESULT bool contains(const QModelIndex &index, int charPos, const MessageAttachment &att = {}) const;
    Q_REQUIRED_RESULT QTextCursor selectionForIndex(const QModelIndex &index, QTextDocument *doc, const MessageAttachment &att = {}) const;

    void clear();
    void setStart(const QModelIndex &index, int charPos, const MessageAttachment &msgAttach = {});
    void setEnd(const QModelIndex &index, int charPos, const MessageAttachment &msgAttach = {});
    void selectWordUnderCursor(const QModelIndex &index, int charPos, DocumentFactoryInterface *factory);
    void selectWordUnderCursor(const QModelIndex &index, int charPos, DocumentFactoryInterface *factory, const MessageAttachment &msgAttach);
    void selectMessage(const QModelIndex &index);

    void setTextHelperFactory(DocumentFactoryInterface *newTextHelperFactory);
    Q_REQUIRED_RESULT DocumentFactoryInterface *textHelperFactory() const;

    void setAttachmentFactories(const QVector<DocumentFactoryInterface *> &newAttachmentFactories);

    Q_REQUIRED_RESULT const QVector<DocumentFactoryInterface *> &attachmentFactories() const;

Q_SIGNALS:
    void repaintNeeded(const QModelIndex &index);

private:
    void selectWord(const QModelIndex &index, int charPos, QTextDocument *doc);
    struct OrderedPositions {
        int fromRow;
        int fromCharPos;
        int toRow;
        int toCharPos;
    };

    struct AttachmentSelection {
        MessageAttachment attachment;
        int fromCharPos = 0;
        int toCharPos = 0;
    };

    Q_REQUIRED_RESULT OrderedPositions orderedPositions() const;
    void selectionText(const OrderedPositions ordered,
                       Format format,
                       int row,
                       const QModelIndex &index,
                       QTextDocument *doc,
                       QString &str,
                       const MessageAttachment &att = {}) const;

    QPersistentModelIndex mStartIndex;
    QPersistentModelIndex mEndIndex;
    QVector<AttachmentSelection> mAttachmentSelection;
    int mStartPos = -1; // first selected character in start row
    int mEndPos = -1; // last selected character in end row

    DocumentFactoryInterface *mTextHelperFactory = nullptr;
    QVector<DocumentFactoryInterface *> mAttachmentFactories;
};
