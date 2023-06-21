/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "listattachmentdelegate.h"
#include "common/delegatepaintutil.h"
#include "common/delegateutil.h"
#include "model/filesforroommodel.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <QMimeDatabase>
#include <QMouseEvent>
#include <QPainter>
#include <QStyle>

ListAttachmentDelegate::ListAttachmentDelegate(RocketChatAccount *account, QObject *parent)
    : QItemDelegate(parent)
    , mDownloadIcon(QIcon::fromTheme(QStringLiteral("cloud-download")))
    , mDeleteIcon(QIcon::fromTheme(QStringLiteral("delete")))
    , mRocketChatAccount(account)
{
}

ListAttachmentDelegate::~ListAttachmentDelegate() = default;

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
    DelegatePaintUtil::drawLighterText(
        painter,
        layout.timeStampText,
        QPoint(DelegatePaintUtil::margin() + option.rect.x() + layout.mimetypeHeight, layout.timeStampY + painter->fontMetrics().ascent()));

    // Draw delete icon (for our own messages)
    if (file->userId() == mRocketChatAccount->userId()) {
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
            auto parentWidget = const_cast<QWidget *>(option.widget);
            const QString fileName = DelegateUtil::querySaveFileName(parentWidget, i18n("Save Attachment"), QUrl(file->url()));

            if (!fileName.isEmpty()) {
                mRocketChatAccount->downloadFile(file->url(), QUrl::fromLocalFile(fileName));
            }
            return true;
        }
        if (layout.deleteAttachmentRect.contains(mev->pos()) && (file->userId() == mRocketChatAccount->userId())) {
            auto parentWidget = const_cast<QWidget *>(option.widget);
            if (KMessageBox::ButtonCode::PrimaryAction
                == KMessageBox::questionTwoActions(parentWidget,
                                                   i18n("Do you want to Delete this File?"),
                                                   i18nc("@title", "Delete File"),
                                                   KStandardGuiItem::del(),
                                                   KStandardGuiItem::cancel())) {
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

#include "moc_listattachmentdelegate.cpp"
