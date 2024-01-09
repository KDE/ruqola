/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "messageattachmentdelegatehelperbase.h"

#include <QIcon>
#include <QModelIndex>
#include <QRect>

class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageAttachmentDelegateHelperVideo : public MessageAttachmentDelegateHelperBase
{
public:
    explicit MessageAttachmentDelegateHelperVideo(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl);
    ~MessageAttachmentDelegateHelperVideo() override;
    void
    draw(const MessageAttachment &msgAttach, QPainter *painter, QRect messageRect, const QModelIndex &index, const QStyleOptionViewItem &option) const override;
    [[nodiscard]] QSize sizeHint(const MessageAttachment &msgAttach, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const override;
    [[nodiscard]] bool handleMouseEvent(const MessageAttachment &msgAttach,
                                        QMouseEvent *mouseEvent,
                                        QRect attachmentsRect,
                                        const QStyleOptionViewItem &option,
                                        const QModelIndex &index) override;

private:
    struct VideoLayout {
        QString videoPath;
        QString title;
        QSize titleSize;
        QSize descriptionSize;
        QRect downloadButtonRect;
        QRect showButtonRect;
        bool hasDescription = false;
    };
    [[nodiscard]] VideoLayout layoutVideo(const MessageAttachment &msgAttach, const QStyleOptionViewItem &option, int attachmentsWidth) const;
    [[nodiscard]] QPoint
    adaptMousePosition(const QPoint &pos, const MessageAttachment &msgAttach, QRect attachmentsRect, const QStyleOptionViewItem &option) override;
    const QIcon mDownloadIcon;
    const QIcon mVisibilityIcon;
};
