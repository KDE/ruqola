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

#ifndef MESSAGEDELEGATEHELPERBASE_H
#define MESSAGEDELEGATEHELPERBASE_H

#include "libruqolawidgets_private_export.h"
#include <QSize>
class QPainter;
class QRect;
class QModelIndex;
class QMouseEvent;
class QStyleOptionViewItem;
class QRectF;

class Message;

class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageDelegateHelperBase
{
public:
    virtual ~MessageDelegateHelperBase();

    virtual void draw(QPainter *painter, const QRect &rect, const QModelIndex &index, const QStyleOptionViewItem &option, qreal *pBaseLine) const = 0;
    virtual QSize sizeHint(const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const = 0;
    virtual bool handleMouseEvent(QMouseEvent *mouseEvent, const QRect &messageRect, const QStyleOptionViewItem &option, const QModelIndex &index);
};

#endif // MESSAGEDELEGATEHELPERBASE_H
