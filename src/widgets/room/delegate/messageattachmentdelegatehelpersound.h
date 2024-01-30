/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "messageattachmentdelegatehelperbase.h"

#include <QIcon>
#include <QModelIndex>

class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageAttachmentDelegateHelperSound : public MessageAttachmentDelegateHelperBase
{
public:
    explicit MessageAttachmentDelegateHelperSound(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl);
    ~MessageAttachmentDelegateHelperSound() override;
    void
    draw(const MessageAttachment &msgAttach, QPainter *painter, QRect messageRect, const QModelIndex &index, const QStyleOptionViewItem &option) const override;
    [[nodiscard]] QSize sizeHint(const MessageAttachment &msgAttach, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const override;
    [[nodiscard]] bool handleMouseEvent(const MessageAttachment &msgAttach,
                                        QMouseEvent *mouseEvent,
                                        QRect attachmentsRect,
                                        const QStyleOptionViewItem &option,
                                        const QModelIndex &index) override;

private:
    struct SoundLayout {
        QString audioPath;
        QString title;
        QSize titleSize;
        QSize descriptionSize;
        QRect playerVolumeButtonRect;
        QRect downloadButtonRect;
        bool hasDescription = false;
    };
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QPoint adaptMousePosition(const QPoint &pos,
                                                                       const MessageAttachment &msgAttach,
                                                                       QRect attachmentsRect,
                                                                       const QStyleOptionViewItem &option) override;
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT SoundLayout layoutSound(const MessageAttachment &msgAttach,
                                                                     const QStyleOptionViewItem &option,
                                                                     int attachmentsWidth) const;
    const QIcon mPlayerVolumeIcon;
    const QIcon mDownloadIcon;
};
