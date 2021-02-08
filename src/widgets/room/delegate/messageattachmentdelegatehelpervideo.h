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

#ifndef MESSAGEATTACHMENTDELEGATEHELPERVIDEO_H
#define MESSAGEATTACHMENTDELEGATEHELPERVIDEO_H

#include "messagedelegatehelperbase.h"

#include <QIcon>
#include <QModelIndex>
#include <QRect>

class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageAttachmentDelegateHelperVideo : public MessageDelegateHelperBase
{
public:
    MessageAttachmentDelegateHelperVideo();
    ~MessageAttachmentDelegateHelperVideo() override;
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
    struct VideoLayout {
        QString videoPath;
        QString title;
        QString description;
        QSize titleSize;
        QSize descriptionSize;
        QRect downloadButtonRect;
        QRect showButtonRect;
    };
    Q_REQUIRED_RESULT VideoLayout layoutVideo(const MessageAttachment &msgAttach, const QStyleOptionViewItem &option, int attachmentsWidth) const;
    const QIcon mDownloadIcon;
    const QIcon mVisibilityIcon;
};

#endif // MESSAGEATTACHMENTDELEGATEHELPERVIDEO_H
