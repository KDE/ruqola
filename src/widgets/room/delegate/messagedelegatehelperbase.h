/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "lrucache.h"
#include "messages/messageattachment.h"
#include "textselection.h"

#include <QPersistentModelIndex>
#include <QSize>
#include <QTextDocument>

#include <memory>

class QPainter;
class QRect;
class QModelIndex;
class QMouseEvent;
class QStyleOptionViewItem;
class QHelpEvent;

class Message;
class QListView;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageDelegateHelperBase : public QObject, public DocumentFactoryInterface
{
public:
    explicit MessageDelegateHelperBase(QListView *view, TextSelection *textSelection);
    virtual ~MessageDelegateHelperBase();

    virtual void
    draw(const MessageAttachment &msgAttach, QPainter *painter, QRect attachmentsRect, const QModelIndex &index, const QStyleOptionViewItem &option) const = 0;
    virtual QSize sizeHint(const MessageAttachment &msgAttach, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const = 0;
    virtual bool handleMouseEvent(const MessageAttachment &msgAttach,
                                  QMouseEvent *mouseEvent,
                                  QRect attachmentsRect,
                                  const QStyleOptionViewItem &option,
                                  const QModelIndex &index);

    Q_REQUIRED_RESULT bool
    maybeStartDrag(const MessageAttachment &msgAttach, QMouseEvent *event, QRect attachmentsRect, const QStyleOptionViewItem &option, const QModelIndex &index);

    virtual bool handleHelpEvent(QHelpEvent *helpEvent, QRect messageRect, const MessageAttachment &msgAttach, const QStyleOptionViewItem &option);

    void clearTextDocumentCache();

protected:
    Q_REQUIRED_RESULT virtual int charPosition(const QTextDocument *doc,
                                               const MessageAttachment &msgAttach,
                                               QRect attachmentsRect,
                                               const QPoint &pos,
                                               const QStyleOptionViewItem &option) = 0;
    Q_REQUIRED_RESULT QSize documentDescriptionForIndexSize(const MessageAttachment &msgAttach, int width) const;
    Q_REQUIRED_RESULT QTextDocument *documentDescriptionForIndex(const MessageAttachment &msgAttach, int width) const;
    mutable LRUCache<QString, std::unique_ptr<QTextDocument>, 32> mDocumentCache;
    void drawDescription(const MessageAttachment &msgAttach,
                         QRect messageRect,
                         QPainter *painter,
                         int topPos,
                         const QModelIndex &index,
                         const QStyleOptionViewItem &option) const;

    QListView *const mListView;
    TextSelection *const mSelection;
    // TODO use unique mMightStartDrag see messageDelegateHelperText
    bool mMightStartDrag = false;

    /**
     * Creates (or retrieves from a cache) the QTextDocument for a given @p index.
     * @param width The width for layouting that QTextDocument. -1 if no layouting is desired (e.g. for converting to text or HTML)
     * @param widget The view to update when fetching thread context on demand. nullptr if this isn't needed (e.g. from SelectionManager)
     * @return the QTextDocument. Ownership remains with the cache, don't delete it.
     */
    Q_REQUIRED_RESULT QTextDocument *documentForIndex(const QModelIndex &index) const override;
    Q_REQUIRED_RESULT QTextDocument *documentForIndex(const MessageAttachment &msgAttach) const override;

private:
    QPersistentModelIndex mCurrentIndex;
    void updateView(const QModelIndex &index);
};
