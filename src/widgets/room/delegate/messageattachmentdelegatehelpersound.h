/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#pragma once

#include "messagedelegatehelperbase.h"

#include <QIcon>
#include <QModelIndex>

class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageAttachmentDelegateHelperSound : public MessageDelegateHelperBase
{
public:
    MessageAttachmentDelegateHelperSound();
    ~MessageAttachmentDelegateHelperSound() override;
    void
    draw(const MessageAttachment &msgAttach, QPainter *painter, QRect messageRect, const QModelIndex &index, const QStyleOptionViewItem &option) const override;
    QSize sizeHint(const MessageAttachment &msgAttach, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const override;
    bool handleMouseEvent(const MessageAttachment &msgAttach,
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
    Q_REQUIRED_RESULT SoundLayout layoutSound(const MessageAttachment &msgAttach, const QStyleOptionViewItem &option, int attachmentsWidth) const;
    const QIcon mPlayerVolumeIcon;
    const QIcon mDownloadIcon;
};

