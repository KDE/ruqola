/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "messageattachmentdelegatehelperbase.h"

#include <QIcon>
#include <QRect>
#include <QString>

class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageAttachmentDelegateHelperFile : public MessageAttachmentDelegateHelperBase
{
public:
    explicit MessageAttachmentDelegateHelperFile(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl);
    ~MessageAttachmentDelegateHelperFile() override;
    void draw(const MessageAttachment &msgAttach,
              QPainter *painter,
              QRect attachmentsRect,
              const QModelIndex &index,
              const QStyleOptionViewItem &option) const override;
    [[nodiscard]] QSize sizeHint(const MessageAttachment &msgAttach, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const override;
    [[nodiscard]] bool handleMouseEvent(const MessageAttachment &msgAttach,
                                        QMouseEvent *mouseEvent,
                                        QRect attachmentsRect,
                                        const QStyleOptionViewItem &option,
                                        const QModelIndex &index) override;

private:
    struct FileLayout {
        QString title;
        QString description;
        QSize titleSize;
        QSize descriptionSize;
        QRect downloadButtonRect;
        int y; // relative
        int height;
        QString link;
    };
    [[nodiscard]] FileLayout doLayout(const MessageAttachment &msgAttach, const QStyleOptionViewItem &option, int attachmentsWidth) const;
    void handleDownloadClicked(const QString &link, QWidget *widget);
    friend class MessageDelegateHelperFileTest;
    const QIcon mDownloadIcon;
    [[nodiscard]] QPoint
    adaptMousePosition(const QPoint &pos, const MessageAttachment &msgAttach, QRect attachmentsRect, const QStyleOptionViewItem &option) override;
};
