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

#ifndef MESSAGEDELEGATEHELPERIMAGE_H
#define MESSAGEDELEGATEHELPERIMAGE_H

#include "messagedelegatehelperbase.h"
#include "pixmapcache.h"

#include <QPixmap>
#include <QVector>

class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageDelegateHelperImage : public MessageDelegateHelperBase
{
public:
    void draw(QPainter *painter, const QRect &messageRect, const QModelIndex &index, const QStyleOptionViewItem &option) const override;
    QSize sizeHint(const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const override;
    bool handleMouseEvent(QMouseEvent *mouseEvent, const QRect &attachmentsRect, const QStyleOptionViewItem &option, const QModelIndex &index) override;

private:
    friend class MessageDelegateHelperImageTest;

    struct ImageLayout {
        QPixmap pixmap;
        QString title;
        QString description;
        QSize titleSize;
        QSize descriptionSize;
        QRect hideShowButtonRect;
        QRect downloadButtonRect;
        bool isShown;
    };
    ImageLayout layoutImage(const Message *message, const QStyleOptionViewItem &option) const;

    mutable PixmapCache mPixmapCache;
};

#endif // MESSAGEDELEGATEHELPERIMAGE_H
