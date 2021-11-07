/*
   Copyright (c) 2021 David Faure <faure@kde.org>

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

#pragma once

#include "libruqolawidgets_private_export.h"

#include <QObject>
#include <QPersistentModelIndex>

class QTextCursor;
class QTextDocument;

class LIBRUQOLAWIDGETS_TESTS_EXPORT DocumentFactoryInterface
{
public:
    virtual ~DocumentFactoryInterface();
    /**
     * Creates (or retrieves from a cache) the QTextDocument for a given @p index.
     * @param width The width for layouting that QTextDocument. -1 if no layouting is desired (e.g. for converting to text or HTML)
     * @return the QTextDocument. Ownership remains with the cache, don't delete it.
     */
    virtual Q_REQUIRED_RESULT QTextDocument *documentForIndex(const QModelIndex &index) const = 0;
};

class LIBRUQOLAWIDGETS_TESTS_EXPORT TextSelection : public QObject
{
    Q_OBJECT
public:
    explicit TextSelection(DocumentFactoryInterface *factory);
    Q_REQUIRED_RESULT bool hasSelection() const;
    enum Format {
        Text,
        Html,
    };
    Q_REQUIRED_RESULT QString selectedText(Format format) const;
    Q_REQUIRED_RESULT bool contains(const QModelIndex &index, int charPos) const;
    Q_REQUIRED_RESULT QTextCursor selectionForIndex(const QModelIndex &index, QTextDocument *doc) const;

    void clear();
    void setStart(const QModelIndex &index, int charPos);
    void setEnd(const QModelIndex &index, int charPos);
    void selectWordUnderCursor();
    void selectMessage(const QModelIndex &index);

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
    DocumentFactoryInterface *const mDocumentFactory;
};
