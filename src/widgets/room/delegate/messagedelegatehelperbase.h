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
#include "messages/messageattachment.h"
#include <QSize>
#include <QTextDocument>
#include "lrucache.h"
class QPainter;
class QRect;
class QModelIndex;
class QMouseEvent;
class QStyleOptionViewItem;

class Message;

class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageDelegateHelperBase
{
public:
    virtual ~MessageDelegateHelperBase();

    virtual void draw(const MessageAttachment &msgAttach, QPainter *painter, QRect attachmentsRect, const QModelIndex &index, const QStyleOptionViewItem &option) const = 0;
    virtual QSize sizeHint(const MessageAttachment &msgAttach, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const = 0;
    virtual bool handleMouseEvent(const MessageAttachment &msgAttach, QMouseEvent *mouseEvent, QRect attachmentsRect, const QStyleOptionViewItem &option, const QModelIndex &index);
protected:
    Q_REQUIRED_RESULT QSize documentDescriptionForIndexSize(const MessageAttachment &msgAttach, int width) const;
    Q_REQUIRED_RESULT QTextDocument *documentDescriptionForIndex(const MessageAttachment &msgAttach, int width) const;
    mutable LRUCache<QString, std::unique_ptr<QTextDocument>, 32> mDocumentCache;
    void drawDescription(const MessageAttachment &msgAttach, QRect messageRect, QPainter *painter, int topPos) const;
};

#endif // MESSAGEDELEGATEHELPERBASE_H
