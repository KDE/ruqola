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
#include "listattachmentdelegate.h"
#include <QMouseEvent>
#include <QPainter>
#include "model/filesforroommodel.h"

ListAttachmentDelegate::ListAttachmentDelegate(QObject *parent)
    : QItemDelegate(parent)
    , mDownloadIcon(QIcon::fromTheme(QStringLiteral("cloud-download")))
{
}

ListAttachmentDelegate::~ListAttachmentDelegate()
{
}

void ListAttachmentDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // [M] <icon> [M] <name> [M] <download icon> [M]   ([M] = margin)
    // user
    // alias
    // date
    painter->save();

    QStyleOptionViewItem optionCopy = option;
    optionCopy.showDecorationSelected = true;

    drawBackground(painter, optionCopy, index);

    const File *file = index.data(FilesForRoomModel::FilePointer).value<File *>();
    const Layout layout = doLayout(option, index);

    //TODO draw filename
    painter->drawText(0, layout.attachmentNameY, layout.attachmentName);


    // Draw the sender
    painter->setFont(layout.senderFont);
    //TODO fix me timeStampHeight
    painter->drawText(0, layout.senderY, layout.senderText);

    //TODO draw timestamp



    // Timestamp
    //drawTimestamp(painter, layout.timeStampText, layout.timeStampPos);

    painter->restore();
}

bool ListAttachmentDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    const QEvent::Type eventType = event->type();
    if (eventType == QEvent::MouseButtonRelease) {
        auto *mev = static_cast<QMouseEvent *>(event);
        //TODO
    }
    return QItemDelegate::editorEvent(event, model, option, index);
}

QSize ListAttachmentDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // Note: option.rect in this method is huge (as big as the viewport)
    const Layout layout = doLayout(option, index);

    int additionalHeight = 0;
    // A little bit of margin below the very last item, it just looks better
    if (index.row() == index.model()->rowCount() - 1) {
        additionalHeight += 4;
    }

    const int contentsHeight = layout.timeStampY + layout.senderY + layout.attachmentNameY - option.rect.y();
    return QSize(option.rect.width(),
                 contentsHeight + additionalHeight);
}

static QSize timeStampSize(const QString &timeStampText, const QStyleOptionViewItem &option)
{
    // This gives incorrect results (too small bounding rect), no idea why!
    //const QSize timeSize = painter->fontMetrics().boundingRect(timeStampText).size();
    return QSize(option.fontMetrics.horizontalAdvance(timeStampText), option.fontMetrics.height());
}

ListAttachmentDelegate::Layout ListAttachmentDelegate::doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const File *file = index.data(FilesForRoomModel::FilePointer).value<File *>();

    Layout layout;
    QRect usableRect = option.rect;
    layout.usableRect = usableRect; // Just for the top, for now. The left will move later on.

    layout.attachmentName = file->fileName();
    layout.attachmentNameY = usableRect.top() + option.fontMetrics.height();


    layout.senderText = file->userName();
    layout.senderFont = option.font;
    layout.senderFont.setItalic(true);
    layout.senderY = layout.attachmentNameY + option.fontMetrics.height();

    // Timestamp
    layout.timeStampText = index.data(FilesForRoomModel::TimeStamp).toString();
    layout.timeStampY = layout.senderY + option.fontMetrics.height();

    return layout;
}
