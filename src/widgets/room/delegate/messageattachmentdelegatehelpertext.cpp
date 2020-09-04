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

void MessageAttachmentDelegateHelperText::draw(QPainter *painter, QRect messageRect, const QModelIndex &index, const QStyleOptionViewItem &option) const
{
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();

    const TextLayout layout = layoutText(message, option);
    painter->drawText(messageRect.x(), messageRect.y() + option.fontMetrics.ascent(), layout.text);
}

QSize MessageAttachmentDelegateHelperText::sizeHint(const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const
{
    Q_UNUSED(maxWidth)
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();

    const TextLayout layout = layoutText(message, option);
    int height = layout.textSize.height() + DelegatePaintUtil::margin();
    int pixmapWidth = 0;
    return QSize(qMax(pixmapWidth, layout.textSize.width()),
                 height);
}

bool MessageAttachmentDelegateHelperText::handleMouseEvent(QMouseEvent *mouseEvent, QRect attachmentsRect, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    Q_UNUSED(attachmentsRect);
    Q_UNUSED(option);
    Q_UNUSED(index);
    if (mouseEvent->type() == QEvent::MouseButtonRelease) {
        //TODO ???
//        const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();
//        const QPoint pos = mouseEvent->pos();

//        TextLayout layout = layoutText(message, option);
    }
    return false;
}

MessageAttachmentDelegateHelperText::TextLayout MessageAttachmentDelegateHelperText::layoutText(const Message *message, const QStyleOptionViewItem &option) const
{
    TextLayout layout;
    if (message->attachements().isEmpty()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "No attachments in Text message";
        return layout;
    }
    if (message->attachements().count() > 1) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Multiple attachments in Text message? Can this happen?" << " message->attachements()" << message->attachements();
    }
    const MessageAttachment &msgAttach = message->attachements().at(0);
    layout.text = msgAttach.text();
    layout.textSize = option.fontMetrics.size(Qt::TextSingleLine, layout.text);
    return layout;
}
