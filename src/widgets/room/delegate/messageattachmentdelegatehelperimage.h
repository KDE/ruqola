/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "messageattachmentdelegatehelperbase.h"
#include "misc/pixmapcache.h"
#include "runninganimatedimage.h"

#include <QModelIndex>
#include <QPixmap>
#include <vector>
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageAttachmentDelegateHelperImage : public MessageAttachmentDelegateHelperBase
{
public:
    explicit MessageAttachmentDelegateHelperImage(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl);
    ~MessageAttachmentDelegateHelperImage() override = default;
    void
    draw(const MessageAttachment &msgAttach, QPainter *painter, QRect messageRect, const QModelIndex &index, const QStyleOptionViewItem &option) const override;
    [[nodiscard]] QSize sizeHint(const MessageAttachment &msgAttach, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const override;
    [[nodiscard]] bool handleMouseEvent(const MessageAttachment &msgAttach,
                                        QMouseEvent *mouseEvent,
                                        QRect attachmentsRect,
                                        const QStyleOptionViewItem &option,
                                        const QModelIndex &index) override;

private:
    Q_DISABLE_COPY(MessageAttachmentDelegateHelperImage)
    friend class MessageDelegateHelperImageTest;

    struct ImageLayout {
        QPixmap pixmap;
        QString imagePreviewPath;
        QString imageBigPath;
        QString title;
        QSize titleSize;
        QSize imageSize;
        QSize descriptionSize;
        QRect hideShowButtonRect;
        QRect downloadButtonRect;
        bool isShown = true;
        bool isAnimatedImage = false;
        bool hasDescription = false;
    };
    [[nodiscard]] ImageLayout
    layoutImage(const MessageAttachment &msgAttach, const QStyleOptionViewItem &option, int attachmentsWidth, int attachmentsHeight) const;

    [[nodiscard]] std::vector<RunningAnimatedImage>::iterator findRunningAnimatedImage(const QModelIndex &index) const;
    void removeRunningAnimatedImage(const QModelIndex &index) const;
    [[nodiscard]] bool contextMenu(const QPoint &pos,
                                   const QPoint &globalPos,
                                   const MessageAttachment &msgAttach,
                                   QRect attachmentsRect,
                                   const QStyleOptionViewItem &option,
                                   QMenu *menu) override;

private:
    [[nodiscard]] QPoint
    adaptMousePosition(const QPoint &pos, const MessageAttachment &msgAttach, QRect attachmentsRect, const QStyleOptionViewItem &option) override;
    mutable PixmapCache mPixmapCache;
    mutable std::vector<RunningAnimatedImage> mRunningAnimatedImages; // not a hash or map, since QPersistentModelIndex changes value
};
