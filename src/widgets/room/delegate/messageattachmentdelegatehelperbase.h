/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "delegateutils/textselectionimpl.h"
#include "libruqolawidgets_private_export.h"
#include "messagedelegatehelperbase.h"
#include "messages/messageattachment.h"

#include <QPersistentModelIndex>
#include <QSize>

class QTextDocument;
class QPainter;
class QRect;
class QModelIndex;
class QMouseEvent;
class QStyleOptionViewItem;
class QHelpEvent;

class Message;
class QListView;
class RocketChatAccount;
class QMenu;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageAttachmentDelegateHelperBase : public MessageDelegateHelperBase
{
public:
    explicit MessageAttachmentDelegateHelperBase(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl);
    ~MessageAttachmentDelegateHelperBase() override;

    virtual void
    draw(const MessageAttachment &msgAttach, QPainter *painter, QRect attachmentsRect, const QModelIndex &index, const QStyleOptionViewItem &option) const = 0;
    [[nodiscard]] virtual QSize
    sizeHint(const MessageAttachment &msgAttach, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const = 0;
    virtual bool handleMouseEvent(const MessageAttachment &msgAttach,
                                  QMouseEvent *mouseEvent,
                                  QRect attachmentsRect,
                                  const QStyleOptionViewItem &option,
                                  const QModelIndex &index);

    [[nodiscard]] bool
    maybeStartDrag(const MessageAttachment &msgAttach, QMouseEvent *event, QRect attachmentsRect, const QStyleOptionViewItem &option, const QModelIndex &index);

    [[nodiscard]] virtual bool
    handleHelpEvent(QHelpEvent *helpEvent, QRect messageRect, const MessageAttachment &msgAttach, const QStyleOptionViewItem &option);

    [[nodiscard]] QString urlAt(const QStyleOptionViewItem &option, const MessageAttachment &msgAttach, QRect attachmentsRect, QPoint pos);

    [[nodiscard]] virtual bool contextMenu(const QPoint &pos,
                                           const QPoint &globalPos,
                                           const MessageAttachment &msgAttach,
                                           QRect attachmentsRect,
                                           const QStyleOptionViewItem &option,
                                           QMenu *menu);

    void removeMessageDescriptionTitleCache(const MessageAttachment &attachment);

protected:
    [[nodiscard]] virtual QPoint
    adaptMousePosition(const QPoint &pos, const MessageAttachment &msgAttach, QRect attachmentsRect, const QStyleOptionViewItem &option) = 0;

    [[nodiscard]] int
    charPosition(const QTextDocument *doc, const MessageAttachment &msgAttach, QRect attachmentsRect, const QPoint &pos, const QStyleOptionViewItem &option);
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
    [[nodiscard]] QTextDocument *documentForAttachement(const MessageAttachment &msgAttach) const override;

    [[nodiscard]] MessageDelegateHelperBase::DocumentDescriptionInfo convertAttachmentToDocumentDescriptionInfo(const MessageAttachment &msgAttach,
                                                                                                                int width) const;

    void drawTitle(const MessageAttachment &msgAttach, QPainter *painter);
    [[nodiscard]] MessageDelegateHelperBase::DocumentDescriptionInfo convertAttachmentToDocumentTitleInfo(const MessageAttachment &msgAttach, int width) const;

protected:
    enum class DocumentIdType : uint8_t {
        Unknown = 0,
        Title,
        Description,
    };
    Q_ENUM(DocumentIdType);
    [[nodiscard]] QByteArray documentId(MessageAttachmentDelegateHelperBase::DocumentIdType type, const MessageAttachment &msgAttach) const;

private:
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QByteArray documendIdPrefix(MessageAttachmentDelegateHelperBase::DocumentIdType type) const;
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT MessageDelegateHelperBase::DocumentDescriptionInfo
    convertAttachmentToDocumentTypeInfo(DocumentIdType type, const MessageAttachment &msgAttach, int width) const;
    QPersistentModelIndex mCurrentIndex;
};
