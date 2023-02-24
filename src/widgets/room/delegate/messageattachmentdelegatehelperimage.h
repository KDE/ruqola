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
    MessageAttachmentDelegateHelperImage(const MessageAttachmentDelegateHelperImage &) = delete;
    MessageAttachmentDelegateHelperImage &operator=(const MessageAttachmentDelegateHelperImage &) = delete;
    void
    draw(const MessageAttachment &msgAttach, QPainter *painter, QRect messageRect, const QModelIndex &index, const QStyleOptionViewItem &option) const override;
    Q_REQUIRED_RESULT QSize sizeHint(const MessageAttachment &msgAttach,
                                     const QModelIndex &index,
                                     int maxWidth,
                                     const QStyleOptionViewItem &option) const override;
    Q_REQUIRED_RESULT bool handleMouseEvent(const MessageAttachment &msgAttach,
                                            QMouseEvent *mouseEvent,
                                            QRect attachmentsRect,
                                            const QStyleOptionViewItem &option,
                                            const QModelIndex &index) override;

private:
    friend class MessageDelegateHelperImageTest;

    struct ImageLayout {
        QPixmap pixmap;
        QString imagePreviewPath;
        QString imageBigPath;
        QString title;
        QString description;
        QSize titleSize;
        QSize imageSize;
        QSize descriptionSize;
        QRect hideShowButtonRect;
        QRect downloadButtonRect;
        bool isShown = true;
        bool isAnimatedImage = false;
    };
    Q_REQUIRED_RESULT ImageLayout layoutImage(const MessageAttachment &msgAttach,
                                              const QStyleOptionViewItem &option,
                                              int attachmentsWidth,
                                              int attachmentsHeight) const;

    std::vector<RunningAnimatedImage>::iterator findRunningAnimatedImage(const QModelIndex &index) const;
    void removeRunningAnimatedImage(const QModelIndex &index) const;
    Q_REQUIRED_RESULT bool contextMenu(const QPoint &pos,
                                       const QPoint &globalPos,
                                       const MessageAttachment &msgAttach,
                                       QRect attachmentsRect,
                                       const QStyleOptionViewItem &option) override;

private:
    Q_REQUIRED_RESULT QPoint adaptMousePosition(const QPoint &pos,
                                                const MessageAttachment &msgAttach,
                                                QRect attachmentsRect,
                                                const QStyleOptionViewItem &option) override;
    mutable PixmapCache mPixmapCache;
    mutable std::vector<RunningAnimatedImage> mRunningAnimatedImages; // not a hash or map, since QPersistentModelIndex changes value
};
