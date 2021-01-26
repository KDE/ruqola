/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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
#include "common/delegatepaintutil.h"
#include "common/delegateutil.h"
#include "model/filesforroommodel.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <QMimeDatabase>
#include <QMouseEvent>
#include <QPainter>
#include <QStyle>

ListAttachmentDelegate::ListAttachmentDelegate(QObject *parent)
    : QItemDelegate(parent)
    , mDownloadIcon(QIcon::fromTheme(QStringLiteral("cloud-download")))
    , mDeleteIcon(QIcon::fromTheme(QStringLiteral("delete")))
{
}

ListAttachmentDelegate::~ListAttachmentDelegate()
{
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
    const QPixmap pix = QIcon::fromTheme(mimeType.iconName(), QIcon::fromTheme(QStringLiteral("application-octet-stream"))).pixmap(layout.mimetypeHeight);

    painter->drawPixmap(option.rect.x(), option.rect.y(), pix);

    // Draw filename
    const QFont oldFont = painter->font();
    if (!fileComplete) {
        QFont newFont = oldFont;
        newFont.setStrikeOut(true);
        painter->setFont(newFont);
    }
    painter->drawText(DelegatePaintUtil::margin() + option.rect.x() + layout.mimetypeHeight,
                      layout.attachmentNameY + painter->fontMetrics().ascent(),
                      layout.attachmentName);
    // Draw the sender (below the filename)
    painter->setFont(layout.senderFont);
    painter->drawText(DelegatePaintUtil::margin() + option.rect.x() + layout.mimetypeHeight,
                      layout.senderY + painter->fontMetrics().ascent(),
                      layout.senderText);
    painter->setFont(oldFont);

    // Draw the timestamp (below the sender)
    DelegatePaintUtil::drawTimestamp(
        painter,
        layout.timeStampText,
        QPoint(DelegatePaintUtil::margin() + option.rect.x() + layout.mimetypeHeight, layout.timeStampY + painter->fontMetrics().ascent()));

    // Draw delete icon (for our own messages)
    if (file->userId() == Ruqola::self()->rocketChatAccount()->userId()) {
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
        auto mev = static_cast<QMouseEvent *>(event);

        const File *file = index.data(FilesForRoomModel::FilePointer).value<File *>();

        const Layout layout = doLayout(option, index);

        if (layout.downloadAttachmentRect.contains(mev->pos())) {
            auto *parentWidget = const_cast<QWidget *>(option.widget);
            const QString fileName = DelegateUtil::querySaveFileName(parentWidget, i18n("Save Attachment"), QUrl(file->url()));

            if (!fileName.isEmpty()) {
                Ruqola::self()->rocketChatAccount()->downloadFile(file->url(), QUrl::fromLocalFile(fileName));
            }
            return true;
        }
        if (layout.deleteAttachmentRect.contains(mev->pos()) && (file->userId() == Ruqola::self()->rocketChatAccount()->userId())) {
            auto *parentWidget = const_cast<QWidget *>(option.widget);
            if (KMessageBox::Yes == KMessageBox::questionYesNo(parentWidget, i18n("Do you want to Delete this File?"), i18n("Delete File"))) {
                const QString fileId = file->fileId();
                Q_EMIT deleteAttachment(fileId);
                // TODO
                // appid.rocketChatAccount.deleteFileMessage(appid.selectedRoomID, fileId, appid.selectedRoom.channelType)
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

    const int contentsHeight = layout.timeStampY + option.fontMetrics.height() - option.rect.y();
    return {option.rect.width(), contentsHeight};
}

ListAttachmentDelegate::Layout ListAttachmentDelegate::doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const File *file = index.data(FilesForRoomModel::FilePointer).value<File *>();

    Layout layout;
    QRect usableRect = option.rect;
    layout.usableRect = usableRect; // Just for the top, for now. The left will move later on.

    layout.attachmentName = file->fileName();
    layout.attachmentNameY = usableRect.top();

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
    layout.deleteAttachmentRect = QRect(option.rect.width() - 2 * iconSize - DelegatePaintUtil::margin(), option.rect.y(), iconSize, iconSize);
    return layout;
}
