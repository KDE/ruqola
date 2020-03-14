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
#include <QMimeDatabase>
#include <QMouseEvent>
#include <QPainter>
#include <QStyle>
#include "model/filesforroommodel.h"
#include <KIconLoader>

ListAttachmentDelegate::ListAttachmentDelegate(QObject *parent)
    : QItemDelegate(parent)
    , mDownloadIcon(QIcon::fromTheme(QStringLiteral("cloud-download")))
{
}

ListAttachmentDelegate::~ListAttachmentDelegate()
{
}

static qreal basicMargin()
{
    return 8;
}

static void drawTimestamp(QPainter *painter, const QString &timeStampText, const QPoint &timeStampPos)
{
    const QPen oldPen = painter->pen();
    QColor col = painter->pen().color();
    col.setAlpha(128); // TimestampText.qml had opacity: .5
    painter->setPen(col);
    painter->drawText(timeStampPos, timeStampText);
    painter->setPen(oldPen);
}

void ListAttachmentDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // user
    // alias
    // date
    painter->save();

    QStyleOptionViewItem optionCopy = option;
    optionCopy.showDecorationSelected = true;

    drawBackground(painter, optionCopy, index);

    // Draw Mimetype
    const Layout layout = doLayout(option, index);
    const File *file = index.data(FilesForRoomModel::FilePointer).value<File *>();
    QMimeDatabase db;
    const QMimeType mimeType = db.mimeTypeForName(file->mimeType());
    QPixmap pix;
    if (mimeType.isValid()) {
        pix = KIconLoader::global()->loadMimeTypeIcon(mimeType.iconName(), KIconLoader::Desktop);
    } else {
        pix = QIcon::fromTheme(QStringLiteral("unknown")).pixmap(layout.mimetypeHeight, layout.mimetypeHeight);
    }

    painter->drawPixmap(option.rect.x(), option.rect.y(), pix);

    //draw filename
    painter->drawText(basicMargin() + option.rect.x() + layout.mimetypeHeight, layout.attachmentNameY, layout.attachmentName);

    // Draw the sender
    const QFont oldFont = painter->font();
    painter->setFont(layout.senderFont);
    //TODO fix me timeStampHeight
    painter->drawText(basicMargin() + option.rect.x() + layout.mimetypeHeight, layout.senderY, layout.senderText);
    painter->setFont(oldFont);

    // Timestamp
    drawTimestamp(painter, layout.timeStampText, QPoint(basicMargin() + option.rect.x() + layout.mimetypeHeight, layout.timeStampY));

    //TOOD draw Icon.

    const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);
    mDownloadIcon.paint(painter, QRect(option.rect.width() - iconSize, option.rect.y(), iconSize, iconSize));
    painter->drawPixmap(option.rect.x(), option.rect.y(), pix);

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

    const int contentsHeight = layout.timeStampY /* + layout.senderY + layout.attachmentNameY*/ - option.rect.y();
    return QSize(option.rect.width(),
                 contentsHeight + additionalHeight);
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
    layout.timeStampText = file->uploadedDateTimeStr();
    layout.timeStampY = layout.senderY + option.fontMetrics.height();

    layout.mimetypeHeight = option.rect.height();
    usableRect.setLeft(layout.mimetypeHeight);

    return layout;
}
