/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "messagedelegatehelperbase.h"

#include <QIcon>
#include <QModelIndex>
#include <QRect>

class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageAttachmentDelegateHelperVideo : public MessageDelegateHelperBase
{
public:
    explicit MessageAttachmentDelegateHelperVideo(QListView *view, TextSelection *textSelection);
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
