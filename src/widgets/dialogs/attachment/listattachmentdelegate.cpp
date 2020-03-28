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
#include "ruqola.h"
#include "rocketchataccount.h"
#include "common/delegatepaintutil.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QMimeDatabase>
#include <QMouseEvent>
#include <QPainter>
#include <QStyle>
#include "model/filesforroommodel.h"
#include <KIconLoader>
#include <KLocalizedString>
#include <KMessageBox>

ListAttachmentDelegate::ListAttachmentDelegate(QObject *parent)
    : QItemDelegate(parent)
    , mDownloadIcon(QIcon::fromTheme(QStringLiteral("cloud-download")))
    , mDeleteIcon(QIcon::fromTheme(QStringLiteral("delete")))
{
}

ListAttachmentDelegate::~ListAttachmentDelegate()
{
}

static qreal basicMargin()
{
    return 8;
}

void ListAttachmentDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    QStyleOptionViewItem optionCopy = option;
    optionCopy.showDecorationSelected = true;

    drawBackground(painter, optionCopy, index);

    // Draw Mimetype Icon
    const Layout layout = doLayout(option, index);
    const File *file = index.data(FilesForRoomModel::FilePointer).value<File *>();
    const bool fileComplete = file->complete();
    QMimeDatabase db;
    const QMimeType mimeType = db.mimeTypeForName(file->mimeType());
    QPixmap pix;
    if (mimeType.isValid()) {
        pix = KIconLoader::global()->loadMimeTypeIcon(mimeType.iconName(), KIconLoader::Desktop);
    } else {
        pix = QIcon::fromTheme(QStringLiteral("unknown")).pixmap(layout.mimetypeHeight, layout.mimetypeHeight);
    }

    painter->drawPixmap(option.rect.x(), option.rect.y(), pix);

    // Draw filename
    const QFont oldFont = painter->font();
    if (!fileComplete) {
        QFont newFont = oldFont;
        newFont.setStrikeOut(true);
        painter->setFont(newFont);
    }
    painter->drawText(basicMargin() + option.rect.x() + layout.mimetypeHeight, layout.attachmentNameY, layout.attachmentName);
    // Draw the sender (below the filename)
    painter->setFont(layout.senderFont);
    painter->drawText(basicMargin() + option.rect.x() + layout.mimetypeHeight, layout.senderY, layout.senderText);
    painter->setFont(oldFont);

    // Draw the timestamp (below the sender)
    DelegatePaintUtil::drawTimestamp(painter, layout.timeStampText, QPoint(basicMargin() + option.rect.x() + layout.mimetypeHeight, layout.timeStampY));

    // Draw delete icon (for our own messages)
    if (file->userId() == Ruqola::self()->rocketChatAccount()->userID()) {
        mDeleteIcon.paint(painter, layout.deleteAttachmentRect);
    }

    if (fileComplete) {
        // Draw download icon
        mDownloadIcon.paint(painter, layout.downloadAttachmentRect);
    }

    painter->restore();
}

bool ListAttachmentDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    const QEvent::Type eventType = event->type();
    if (eventType == QEvent::MouseButtonRelease) {
        auto *mev = static_cast<QMouseEvent *>(event);

        const File *file = index.data(FilesForRoomModel::FilePointer).value<File *>();

        const Layout layout = doLayout(option, index);

        if (layout.downloadAttachmentRect.contains(mev->pos())) {
            QWidget *parentWidget = const_cast<QWidget *>(option.widget);
            const QString fileName = QFileDialog::getSaveFileName(parentWidget, i18n("Save Attachment"));

            if (!fileName.isEmpty()) {
                Ruqola::self()->rocketChatAccount()->downloadFile(file->url(), QUrl::fromLocalFile(fileName));
            }
            return true;
        }
        if (layout.deleteAttachmentRect.contains(mev->pos()) && (file->userId() == Ruqola::self()->rocketChatAccount()->userID())) {
            QWidget *parentWidget = const_cast<QWidget *>(option.widget);
            if (KMessageBox::Yes == KMessageBox::questionYesNo(parentWidget, i18n("Do you want to Delete this File?"), i18n("Delete File"))) {
                //TODO
                //appid.rocketChatAccount.deleteFileMessage(appid.selectedRoomID, fileid, appid.selectedRoom.channelType)
            }
            return true;
        }
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

    const int iconSize = layout.mimetypeHeight;
    layout.downloadAttachmentRect = QRect(option.rect.width() - iconSize, option.rect.y(), iconSize, iconSize);
    layout.deleteAttachmentRect = QRect(option.rect.width() - 2 * iconSize - basicMargin(), option.rect.y(), iconSize, iconSize);
    return layout;
}
