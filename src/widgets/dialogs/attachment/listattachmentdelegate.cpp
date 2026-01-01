/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "listattachmentdelegate.h"
using namespace Qt::Literals::StringLiterals;

#include "common/delegatepaintutil.h"
#include "model/filesforroommodel.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <QMimeDatabase>
#include <QMouseEvent>
#include <QPainter>
#include <QStyle>
#include <TextAddonsWidgets/SaveFileUtils>

ListAttachmentDelegate::ListAttachmentDelegate(RocketChatAccount *account, QObject *parent)
    : QItemDelegate(parent)
    , mDownloadIcon(QIcon::fromTheme(u"cloud-download"_s))
    , mDeleteIcon(QIcon::fromTheme(u"delete"_s))
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
    const QPixmap pix = QIcon::fromTheme(mimeType.iconName(), QIcon::fromTheme(u"application-octet-stream"_s)).pixmap(layout.mimetypeHeight);

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
    if (mRocketChatAccount->isFileDeletable(file->roomId(), file->userId(), file->uploadedAt())) {
        mDeleteIcon.paint(painter, layout.deleteAttachmentRect);
    }

    if (fileComplete) {
        // Draw download icon
        mDownloadIcon.paint(painter, layout.downloadAttachmentRect);
    }

    painter->restore();
}

void ListAttachmentDelegate::saveAttachment(const QStyleOptionViewItem &option, const File *file)
{
    auto parentWidget = const_cast<QWidget *>(option.widget);
    const QString fileName = TextAddonsWidgets::SaveFileUtils::querySaveFileName(parentWidget, i18nc("@title:window", "Save Attachment"), QUrl(file->url()));

    if (!fileName.isEmpty()) {
        mRocketChatAccount->downloadFile(file->url(), QUrl::fromLocalFile(fileName));
    }
}

bool ListAttachmentDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    const QEvent::Type eventType = event->type();
    if (eventType == QEvent::MouseButtonRelease) {
        auto mev = static_cast<QMouseEvent *>(event);

        const File *file = index.data(FilesForRoomModel::FilePointer).value<File *>();

        const Layout layout = doLayout(option, index);

        if (layout.downloadAttachmentRect.contains(mev->pos())) {
            saveAttachment(option, file);
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
                const QByteArray fileId = file->fileId();
                Q_EMIT deleteAttachment(fileId);
            }
            return true;
        }
    } else if (eventType == QEvent::MouseButtonDblClick) {
        const File *file = index.data(FilesForRoomModel::FilePointer).value<File *>();
        if (file) {
            if (file->typeGroup() == "image"_L1) {
                Q_EMIT showImage(file->fileId());
            } else {
                saveAttachment(option, file);
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
    return {0, contentsHeight};
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
    layout.downloadAttachmentRect = QRect(option.rect.right() - iconSize, option.rect.y(), iconSize, iconSize);
    layout.deleteAttachmentRect = QRect(layout.downloadAttachmentRect.left() - iconSize - DelegatePaintUtil::margin(), option.rect.y(), iconSize, iconSize);
    return layout;
}

#include "moc_listattachmentdelegate.cpp"
