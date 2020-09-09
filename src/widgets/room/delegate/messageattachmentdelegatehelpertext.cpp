/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#include "messageattachmentdelegatehelpertext.h"
#include "ruqolawidgets_debug.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include "dialogs/showvideodialog.h"
#include "common/delegatepaintutil.h"
#include "common/delegateutil.h"

#include <KLocalizedString>

#include <QAbstractItemView>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPointer>
#include <QStyleOptionViewItem>

MessageAttachmentDelegateHelperText::~MessageAttachmentDelegateHelperText()
{
}

void MessageAttachmentDelegateHelperText::draw(const MessageAttachment &msgAttach, QPainter *painter, QRect messageRect, const QModelIndex &index, const QStyleOptionViewItem &option) const
{
    Q_UNUSED(index);
    const TextLayout layout = layoutText(msgAttach, option);
    int y = messageRect.y();
    if (!layout.title.isEmpty()) {
        y += option.fontMetrics.ascent();
        painter->drawText(messageRect.x(), y, layout.title);
    }
    if (!layout.text.isEmpty()) {
        painter->drawText(messageRect.x(), y + option.fontMetrics.ascent(), layout.text);
    }
    //TODO add fields
}

QSize MessageAttachmentDelegateHelperText::sizeHint(const MessageAttachment &msgAttach, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const
{
    Q_UNUSED(index)
    Q_UNUSED(maxWidth)
    const TextLayout layout = layoutText(msgAttach, option);
    int height = layout.textSize.height() + DelegatePaintUtil::margin();
    int descriptionWidth = 0;
    if (!layout.title.isEmpty()) {
        descriptionWidth = layout.titleSize.width();
        height += layout.titleSize.height() + DelegatePaintUtil::margin();
    }
    return QSize(qMax(qMax(0, layout.textSize.width()), descriptionWidth),
                 height);
}

bool MessageAttachmentDelegateHelperText::handleMouseEvent(const MessageAttachment &msgAttach, QMouseEvent *mouseEvent, QRect attachmentsRect, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    Q_UNUSED(attachmentsRect);
    Q_UNUSED(option);
    Q_UNUSED(index);
    Q_UNUSED(msgAttach);
    if (mouseEvent->type() == QEvent::MouseButtonRelease) {
        //TODO ???
//        const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();
//        const QPoint pos = mouseEvent->pos();

//        TextLayout layout = layoutText(message, option);
    }
    return false;
}

MessageAttachmentDelegateHelperText::TextLayout MessageAttachmentDelegateHelperText::layoutText(const MessageAttachment &msgAttach, const QStyleOptionViewItem &option) const
{
    TextLayout layout;
    layout.title = msgAttach.title();
    layout.titleSize = option.fontMetrics.size(Qt::TextSingleLine, layout.title);
    layout.text = msgAttach.text();
    layout.textSize = option.fontMetrics.size(Qt::TextSingleLine, layout.text);
    return layout;
}
