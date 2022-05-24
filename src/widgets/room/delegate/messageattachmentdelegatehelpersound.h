/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "messagedelegatehelperbase.h"

#include <QIcon>
#include <QModelIndex>

class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageAttachmentDelegateHelperSound : public MessageDelegateHelperBase
{
public:
    explicit MessageAttachmentDelegateHelperSound(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl);
    ~MessageAttachmentDelegateHelperSound() override;
    void
    draw(const MessageAttachment &msgAttach, QPainter *painter, QRect messageRect, const QModelIndex &index, const QStyleOptionViewItem &option) const override;
    QSize sizeHint(const MessageAttachment &msgAttach, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const override;
    Q_REQUIRED_RESULT bool handleMouseEvent(const MessageAttachment &msgAttach,
                                            QMouseEvent *mouseEvent,
                                            QRect attachmentsRect,
                                            const QStyleOptionViewItem &option,
                                            const QModelIndex &index) override;

private:
    struct SoundLayout {
        QString audioPath;
        QString title;
        QString description;
        QSize titleSize;
        QSize descriptionSize;
        QRect playerVolumeButtonRect;
        QRect downloadButtonRect;
    };
    Q_REQUIRED_RESULT QPoint adaptMousePosition(const QPoint &pos,
                                                const MessageAttachment &msgAttach,
                                                QRect attachmentsRect,
                                                const QStyleOptionViewItem &option) override;
    Q_REQUIRED_RESULT SoundLayout layoutSound(const MessageAttachment &msgAttach, const QStyleOptionViewItem &option, int attachmentsWidth) const;
    const QIcon mPlayerVolumeIcon;
    const QIcon mDownloadIcon;
};
