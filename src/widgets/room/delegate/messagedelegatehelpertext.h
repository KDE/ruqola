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

#ifndef MESSAGEDELEGATEHELPERTEXT_H
#define MESSAGEDELEGATEHELPERTEXT_H

#include <QModelIndex>
#include <QPointer>
#include <QSize>
#include <QTextCursor>
#include <QTextDocument>
#include <lrucache.h>

#include <memory>

class QPainter;
class QRect;
class QModelIndex;
class QMouseEvent;
class QHelpEvent;
class QStyleOptionViewItem;
class QWidget;

class MessageDelegateHelperText : public QObject
{
    Q_OBJECT
public:
    ~MessageDelegateHelperText() override;
    void draw(QPainter *painter, QRect rect, const QModelIndex &index, const QStyleOptionViewItem &option);
    Q_REQUIRED_RESULT QSize sizeHint(const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option, qreal *pBaseLine) const;
    Q_REQUIRED_RESULT bool handleMouseEvent(QMouseEvent *mouseEvent, QRect messageRect, const QStyleOptionViewItem &option, const QModelIndex &index);
    Q_REQUIRED_RESULT bool handleHelpEvent(QHelpEvent *helpEvent, QWidget *view, QRect messageRect, const QModelIndex &index);
    Q_REQUIRED_RESULT bool maybeStartDrag(QMouseEvent *event, QRect messageRect, const QStyleOptionViewItem &option, const QModelIndex &index);

    void setShowThreadContext(bool b);

    Q_REQUIRED_RESULT bool hasSelection() const;
    Q_REQUIRED_RESULT QString selectedText() const;

    void selectAll(const QWidget *view, QRect messageRect, const QModelIndex &index);
    void removeMessageCache(const QString &messageId);
    void clearTextDocumentCache();

private:
    Q_REQUIRED_RESULT QString makeMessageText(const QModelIndex &index, const QWidget *widget) const;
    void setClipboardSelection();
    void updateView(const QWidget *widget, const QModelIndex &index);
    Q_REQUIRED_RESULT QTextDocument *documentForIndex(const QModelIndex &index, int width, const QWidget *widget) const;
    void setCurrentIndex(const QModelIndex &index, const QWidget *view, QRect messageRect);

    bool mShowThreadContext = true;
    bool mMightStartDrag = false;
    QPersistentModelIndex mCurrentIndex; // during selection
    QPointer<QTextDocument> mCurrentDocument = nullptr; // during selection
    QTextCursor mCurrentTextCursor; // during selection
    mutable LRUCache<QString, std::unique_ptr<QTextDocument>, 32> mDocumentCache;
};

#endif // MESSAGEDELEGATEHELPERTEXT_H
