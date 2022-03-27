/*
   SPDX-FileCopyrightText: 2021 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"

#include <QObject>
#include <QPersistentModelIndex>

class QTextCursor;
class QTextDocument;
class MessageAttachment;

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
    Q_REQUIRED_RESULT QString selectedText(Format format, DocumentFactoryInterface *factory) const;
    Q_REQUIRED_RESULT bool contains(const QModelIndex &index, int charPos) const;
    Q_REQUIRED_RESULT QTextCursor selectionForIndex(const QModelIndex &index, QTextDocument *doc) const;

    void clear();
    void setStart(const QModelIndex &index, int charPos);
    void setEnd(const QModelIndex &index, int charPos);
    void selectWordUnderCursor(const QModelIndex &index, int charPos, DocumentFactoryInterface *factory);
    void selectWordUnderCursor(const QModelIndex &index, const MessageAttachment &msgAttach, int charPos, DocumentFactoryInterface *factory);
    void selectMessage(const QModelIndex &index, DocumentFactoryInterface *factory);

Q_SIGNALS:
    void repaintNeeded(const QModelIndex &index);

private:
    struct OrderedPositions {
        int fromRow;
        int fromCharPos;
        int toRow;
        int toCharPos;
    };
    OrderedPositions orderedPositions() const;

    QPersistentModelIndex mStartIndex;
    QPersistentModelIndex mEndIndex;
    int mStartPos = -1; // first selected character in start row
    int mEndPos = -1; // last selected character in end row
};
