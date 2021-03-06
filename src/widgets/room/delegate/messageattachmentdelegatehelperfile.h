/*
   Copyright (c) 2020 David Faure <faure@kde.org>

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
#include <QRect>
#include <QString>

class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageAttachmentDelegateHelperFile : public MessageDelegateHelperBase
{
public:
    MessageAttachmentDelegateHelperFile();
    ~MessageAttachmentDelegateHelperFile() override;
    void draw(const MessageAttachment &msgAttach,
              QPainter *painter,
              QRect attachmentsRect,
              const QModelIndex &index,
              const QStyleOptionViewItem &option) const override;
    QSize sizeHint(const MessageAttachment &msgAttach, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const override;
    bool handleMouseEvent(const MessageAttachment &msgAttach,
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
    Q_REQUIRED_RESULT FileLayout doLayout(const MessageAttachment &msgAttach, const QStyleOptionViewItem &option, int attachmentsWidth) const;
    void handleDownloadClicked(const QString &link, QWidget *widget);
    friend class MessageDelegateHelperFileTest;
    const QIcon mDownloadIcon;
};

