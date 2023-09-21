/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "delegateutils/textselection.h"
#include "delegateutils/textselectionimpl.h"

#include "messagedelegatehelperbase.h"

#include <QModelIndex>
#include <QSize>
class QTextDocument;
#include <lrucache.h>

#include <memory>

class QListView;
class QPainter;
class QRect;
class QMouseEvent;
class QHelpEvent;
class QStyleOptionViewItem;
class RocketChatAccount;

class MessageDelegateHelperText : public MessageDelegateHelperBase
{
    Q_OBJECT
public:
    explicit MessageDelegateHelperText(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl);
    ~MessageDelegateHelperText() override;
    void draw(QPainter *painter, QRect rect, const QModelIndex &index, const QStyleOptionViewItem &option);
    [[nodiscard]] QSize sizeHint(const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option, qreal *pBaseLine) const;
    [[nodiscard]] bool handleMouseEvent(QMouseEvent *mouseEvent, QRect messageRect, const QStyleOptionViewItem &option, const QModelIndex &index);
    [[nodiscard]] bool handleHelpEvent(QHelpEvent *helpEvent, QRect messageRect, const QModelIndex &index);
    [[nodiscard]] bool maybeStartDrag(QMouseEvent *event, QRect messageRect, const QStyleOptionViewItem &option, const QModelIndex &index);

    void setShowThreadContext(bool b);
    [[nodiscard]] bool showThreadContext() const
    {
        return mShowThreadContext;
    }

    void setSearchText(const QString &newSearchText);

    [[nodiscard]] QString urlAt(const QModelIndex &index, QPoint relativePos) const;

    [[nodiscard]] QString searchText() const;

private:
    friend class TextSelection; // for documentForIndex
    [[nodiscard]] QString makeMessageText(const QPersistentModelIndex &index, bool connectToUpdates) const;
    /**
     * Creates (or retrieves from a cache) the QTextDocument for a given @p index.
     * @param width The width for layouting that QTextDocument. -1 if no layouting is desired (e.g. for converting to text or HTML)
     * @param widget The view to update when fetching thread context on demand. nullptr if this isn't needed (e.g. from SelectionManager)
     * @return the QTextDocument. Ownership remains with the cache, don't delete it.
     */
    [[nodiscard]] QTextDocument *documentForIndex(const QModelIndex &index) const override;
    [[nodiscard]] QTextDocument *documentForIndex(const MessageAttachment &msgAttach) const override;
    [[nodiscard]] QTextDocument *documentForIndex(const Block &block) const override;
    [[nodiscard]] QTextDocument *documentForIndex(const QModelIndex &index, int width, bool connectToUpdates) const;

    QString mSearchText;
    bool mShowThreadContext = true;
};
