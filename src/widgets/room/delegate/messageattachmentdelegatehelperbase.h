/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "delegateutils/textselection.h"
#include "delegateutils/textselectionimpl.h"
#include "libruqolawidgets_private_export.h"
#include "lrucache.h"
#include "messagedelegatehelperbase.h"
#include "messages/messageattachment.h"

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
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageAttachmentDelegateHelperBase : public MessageDelegateHelperBase
{
public:
    explicit MessageAttachmentDelegateHelperBase(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl);
    virtual ~MessageAttachmentDelegateHelperBase();

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

    Q_REQUIRED_RESULT QString urlAt(const QStyleOptionViewItem &option, const MessageAttachment &msgAttach, QRect attachmentsRect, QPoint pos);

protected:
    Q_REQUIRED_RESULT virtual QPoint
    adaptMousePosition(const QPoint &pos, const MessageAttachment &msgAttach, QRect attachmentsRect, const QStyleOptionViewItem &option) = 0;

    Q_REQUIRED_RESULT int
    charPosition(const QTextDocument *doc, const MessageAttachment &msgAttach, QRect attachmentsRect, const QPoint &pos, const QStyleOptionViewItem &option);
    Q_REQUIRED_RESULT QSize documentDescriptionForIndexSize(const MessageAttachment &msgAttach, int width) const;
    Q_REQUIRED_RESULT QTextDocument *documentDescriptionForIndex(const MessageAttachment &msgAttach, int width) const;
    void drawDescription(const MessageAttachment &msgAttach,
                         QRect messageRect,
                         QPainter *painter,
                         int topPos,
                         const QModelIndex &index,
                         const QStyleOptionViewItem &option) const;

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
};
