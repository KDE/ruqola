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

#include "messagedelegatehelperimage.h"
#include "ruqolawidgets_debug.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include "dialogs/showimagedialog.h"

#include <KLocalizedString>

#include <QMouseEvent>
#include <QPainter>
#include <QPixmapCache>
#include <QStyleOptionViewItem>
#include <QPointer>
#include <QFileDialog>

static const int margin = 8; // vertical margin between title and pixmap, and between pixmap and description (if any)

void MessageDelegateHelperImage::draw(QPainter *painter, const QRect &messageRect, const QModelIndex &index, const QStyleOptionViewItem &option, qreal *) const
{
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();

    ImageLayout layout = layoutImage(message, option);
    if (!layout.pixmap.isNull()) {
        const QSize titleSize = option.fontMetrics.size(Qt::TextSingleLine, layout.title);
        const QSize descriptionSize = layout.description.isEmpty() ? QSize(0, 0) : option.fontMetrics.size(Qt::TextSingleLine, layout.description);

        // Draw title and buttons
        painter->drawText(messageRect.x(), messageRect.y() + option.fontMetrics.ascent(), layout.title);
        const QIcon hideShowIcon = QIcon::fromTheme(layout.isShown ? QStringLiteral("visibility") : QStringLiteral("hint"));
        hideShowIcon.paint(painter, layout.hideShowButtonRect.translated(messageRect.topLeft()));
        const QIcon downloadIcon = QIcon::fromTheme(QStringLiteral("cloud-download"));
        downloadIcon.paint(painter, layout.downloadButtonRect.translated(messageRect.topLeft()));

        // Draw main pixmap (if shown)
        int nextY = messageRect.y() + titleSize.height() + margin;
        if (layout.isShown) {
            const int imageMaxWidth = messageRect.width();
            int imageMaxHeight = messageRect.bottom() - nextY - margin;
            if (!layout.description.isEmpty()) {
                imageMaxHeight -= descriptionSize.height() + margin;
            }
            const QPixmap scaledPixmap = layout.pixmap.scaled(imageMaxWidth, imageMaxHeight, Qt::KeepAspectRatio);
            painter->drawPixmap(messageRect.x(), nextY, scaledPixmap);
            nextY += scaledPixmap.height() + margin;
        }

        // Draw description (if any)
        if (!layout.description.isEmpty()) {
            painter->drawText(messageRect.x(), nextY + option.fontMetrics.ascent(), layout.description);
        }
    }
}

QSize MessageDelegateHelperImage::sizeHint(const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const
{
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();

    const ImageLayout layout = layoutImage(message, option);
    const QSize titleSize = option.fontMetrics.size(Qt::TextSingleLine, layout.title);
    int height = titleSize.height() + margin;
    int pixmapWidth = 0;
    if (layout.isShown) {
        pixmapWidth = qMin(layout.pixmap.width(), maxWidth);
        height += qMin(layout.pixmap.height(), 200) + margin;
    }
    int descriptionWidth = 0;
    if (!layout.description.isEmpty()) {
        const QSize descriptionSize = option.fontMetrics.size(Qt::TextSingleLine, layout.description);
        descriptionWidth = descriptionSize.width();
        height += descriptionSize.height() + margin;
    }
    return QSize(qMax(qMax(pixmapWidth, titleSize.width()), descriptionWidth),
                 height);
}

bool MessageDelegateHelperImage::handleMouseEvent(QMouseEvent *mouseEvent, const QRect &messageRect, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();
    const QPoint pos = mouseEvent->pos();

    ImageLayout layout = layoutImage(message, option);
    if (layout.hideShowButtonRect.translated(messageRect.topLeft()).contains(pos)) {
        QAbstractItemModel *model = const_cast<QAbstractItemModel *>(index.model());
        model->setData(index, !layout.isShown, MessageModel::DisplayAttachment);
        return true;
    } else if (layout.downloadButtonRect.translated(messageRect.topLeft()).contains(pos)) {
        const QString file = QFileDialog::getSaveFileName(const_cast<QWidget *>(option.widget), i18n("Save Image"));
        if (!file.isEmpty()) {
            layout.pixmap.save(file);
        }
        return true;
    } else if (!layout.pixmap.isNull() && messageRect.contains(pos)) { // TODO reduce by titleSize and descriptionSize
        QPointer<ShowImageDialog> dlg = new ShowImageDialog();
        dlg->setImage(layout.pixmap);
        dlg->exec();
        delete dlg;
        return true;
    }
    return false;
}

QPixmap MessageDelegateHelperImage::findCachedPixmap(const QString &link) const
{
    auto matchesLink = [&](const CachedImage &cached) {
                           return cached.link == link;
                       };
    auto it = std::find_if(mCachedImages.begin(), mCachedImages.end(), matchesLink);
    if (it == mCachedImages.end()) {
        return QPixmap();
    }
    // Move it to the front
    if (it != mCachedImages.begin()) {
        const auto idx = std::distance(mCachedImages.begin(), it);
        mCachedImages.move(idx, 0);
    }
    return it->pixmap;
}

void MessageDelegateHelperImage::insertCachedPixmap(const QString &link, const QPixmap &pixmap) const
{
    mCachedImages.prepend(CachedImage{link, pixmap});
    static const int s_maxCacheSize = 5;
    if (mCachedImages.size() > s_maxCacheSize) {
        mCachedImages.resize(s_maxCacheSize);
    }
}

MessageDelegateHelperImage::ImageLayout MessageDelegateHelperImage::layoutImage(const Message *message, const QStyleOptionViewItem &option) const
{
    ImageLayout layout;
    if (message->attachements().isEmpty()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "No attachments in Image message";
        return layout;
    }
    if (message->attachements().count() > 1) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Multiple attachments in Image message? Can this happen?";
    }
    const MessageAttachment &msgAttach = message->attachements().at(0);
    const QUrl url = Ruqola::self()->rocketChatAccount()->attachmentUrl(msgAttach.link());
    if (url.isLocalFile()) {
        const QString path = url.toLocalFile();
        // QPixmapCache is too small for this, let's have our own LRU cache
        QPixmap pixmap = findCachedPixmap(path);
        if (pixmap.isNull()) {
            if (pixmap.load(path)) {
                insertCachedPixmap(path, pixmap);
            } else {
                qCWarning(RUQOLAWIDGETS_LOG) << "Could not load" << path;
            }
        }
        layout.pixmap = pixmap;
        //or we could do layout.attachment = msgAttach; if we need many fields from it
        layout.title = msgAttach.title();
        layout.description = msgAttach.description();
        layout.isShown = message->showAttachment();
        const QSize titleSize = option.fontMetrics.size(Qt::TextSingleLine, layout.title);
        const int buttonMargin = 8;
        const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);
        layout.hideShowButtonRect = QRect(titleSize.width() + buttonMargin, 0, iconSize, iconSize);
        layout.downloadButtonRect = layout.hideShowButtonRect.translated(iconSize + buttonMargin, 0);
    }
    return layout;
}
