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

#include <QAbstractItemView>
#include <QFileDialog>
#include <QMessageBox>
#include <QMouseEvent>
#include <QMovie>
#include <QPainter>
#include <QPixmapCache>
#include <QPointer>
#include <QStyleOptionViewItem>

static const int margin = 8; // vertical margin between title and pixmap, and between pixmap and description (if any)

void MessageDelegateHelperImage::draw(QPainter *painter, const QRect &messageRect, const QModelIndex &index, const QStyleOptionViewItem &option) const
{
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();

    ImageLayout layout = layoutImage(message, option, messageRect.width(), messageRect.height());
    if (!layout.pixmap.isNull()) {
        // Draw title and buttons
        painter->drawText(messageRect.x(), messageRect.y() + option.fontMetrics.ascent(), layout.title);
        const QIcon hideShowIcon = QIcon::fromTheme(layout.isShown ? QStringLiteral("visibility") : QStringLiteral("hint"));
        hideShowIcon.paint(painter, layout.hideShowButtonRect.translated(messageRect.topLeft()));
        const QIcon downloadIcon = QIcon::fromTheme(QStringLiteral("cloud-download"));
        downloadIcon.paint(painter, layout.downloadButtonRect.translated(messageRect.topLeft()));

        // Draw main pixmap (if shown)
        int nextY = messageRect.y() + layout.titleSize.height() + margin;
        if (layout.isShown) {
            QPixmap scaledPixmap;
            if (layout.isAnimatedImage) {
                auto it = findRunningAnimatedImage(index);
                if (it != mRunningAnimatedImages.end()) {
                    scaledPixmap = (*it).movie->currentPixmap();
                } else {
                    mRunningAnimatedImages.emplace_back(index);
                    auto &rai = mRunningAnimatedImages.back();
                    rai.movie->setFileName(layout.imagePath);
                    rai.movie->setScaledSize(layout.imageSize);
                    auto *view = qobject_cast<QAbstractItemView *>(const_cast<QWidget *>(option.widget));
                    const QPersistentModelIndex &idx = rai.index;
                    QObject::connect(rai.movie, &QMovie::frameChanged,
                                     view, [view, idx, this]() {
                        if (view->viewport()->rect().contains(view->visualRect(idx))) {
                            view->update(idx);
                        } else {
                            removeRunningAnimatedImage(idx);
                        }
                    }, Qt::QueuedConnection);
                    rai.movie->start();
                    scaledPixmap = rai.movie->currentPixmap();
                }
            } else {
                scaledPixmap = layout.pixmap.scaled(layout.imageSize);
            }
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

    const ImageLayout layout = layoutImage(message, option, maxWidth, -1);
    int height = layout.titleSize.height() + margin;
    int pixmapWidth = 0;
    if (layout.isShown) {
        pixmapWidth = qMin(layout.pixmap.width(), maxWidth);
        height += qMin(layout.pixmap.height(), 200) + margin;
    }
    int descriptionWidth = 0;
    if (!layout.description.isEmpty()) {
        descriptionWidth = layout.descriptionSize.width();
        height += layout.descriptionSize.height() + margin;
    }
    return QSize(qMax(qMax(pixmapWidth, layout.titleSize.width()), descriptionWidth),
                 height);
}

bool MessageDelegateHelperImage::handleMouseEvent(QMouseEvent *mouseEvent, const QRect &attachmentsRect, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (mouseEvent->type() == QEvent::MouseButtonRelease) {
        const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();
        const QPoint pos = mouseEvent->pos();

        ImageLayout layout = layoutImage(message, option, attachmentsRect.width(), attachmentsRect.height());
        if (layout.hideShowButtonRect.translated(attachmentsRect.topLeft()).contains(pos)) {
            auto *model = const_cast<QAbstractItemModel *>(index.model());
            model->setData(index, !layout.isShown, MessageModel::DisplayAttachment);
            return true;
        } else if (layout.downloadButtonRect.translated(attachmentsRect.topLeft()).contains(pos)) {
            QWidget *parentWidget = const_cast<QWidget *>(option.widget);
            const QString file = QFileDialog::getSaveFileName(parentWidget, i18n("Save Image"));
            if (!file.isEmpty()) {
                QFile::remove(file); // copy() doesn't overwrite
                QFile sourceFile(layout.imagePath);
                if (!sourceFile.copy(file)) {
                    QMessageBox::warning(parentWidget, i18n("Error saving file"), sourceFile.errorString());
                }
            }
            return true;
        } else if (!layout.pixmap.isNull()) {
            const int imageY = attachmentsRect.y() + layout.titleSize.height() + margin;
            const QRect imageRect(attachmentsRect.x(), imageY, layout.imageSize.width(), layout.imageSize.height());
            if (imageRect.contains(pos)) {
                QPointer<ShowImageDialog> dlg = new ShowImageDialog();
                dlg->setIsAnimatedPixmap(layout.isAnimatedImage);
                if (layout.isAnimatedImage) {
                    dlg->setImagePath(layout.imagePath);
                } else {
                    dlg->setImage(layout.pixmap);
                }
                dlg->exec();
                delete dlg;
            }
            return true;
        }
    }
    return false;
}

MessageDelegateHelperImage::ImageLayout MessageDelegateHelperImage::layoutImage(const Message *message, const QStyleOptionViewItem &option,
                                                                                int attachmentsWidth, int attachmentsHeight) const
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
        layout.imagePath = url.toLocalFile();
        layout.pixmap = mPixmapCache.pixmapForLocalFile(layout.imagePath);
        //or we could do layout.attachment = msgAttach; if we need many fields from it
        layout.title = msgAttach.title();
        layout.description = msgAttach.description();
        layout.isShown = message->showAttachment();
        layout.isAnimatedImage = msgAttach.isAnimatedImage();
        layout.titleSize = option.fontMetrics.size(Qt::TextSingleLine, layout.title);
        layout.descriptionSize = layout.description.isEmpty() ? QSize(0, 0) : option.fontMetrics.size(Qt::TextSingleLine, layout.description);
        const int buttonMargin = 8;
        const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);
        layout.hideShowButtonRect = QRect(layout.titleSize.width() + buttonMargin, 0, iconSize, iconSize);
        layout.downloadButtonRect = layout.hideShowButtonRect.translated(iconSize + buttonMargin, 0);

        if (attachmentsHeight > 0) {
            // Vertically: attachmentsHeight = title | margin | image | margin [| description | margin]
            int imageMaxHeight = attachmentsHeight - layout.titleSize.height() - margin * 2;
            if (!layout.description.isEmpty()) {
                imageMaxHeight -= layout.descriptionSize.height() + margin;
            }
            layout.imageSize = layout.pixmap.size().scaled(attachmentsWidth, imageMaxHeight, Qt::KeepAspectRatio);
        }
    }
    return layout;
}

std::vector<MessageDelegateHelperImage::RunningAnimatedImage>::iterator MessageDelegateHelperImage::findRunningAnimatedImage(const QModelIndex &index) const
{
    auto matchesIndex = [&](const RunningAnimatedImage &rai) { return rai.index == index; };
    return std::find_if(mRunningAnimatedImages.begin(), mRunningAnimatedImages.end(), matchesIndex);
}

void MessageDelegateHelperImage::removeRunningAnimatedImage(const QModelIndex &index) const
{
    auto it = findRunningAnimatedImage(index);
    if (it != mRunningAnimatedImages.end()) {
        mRunningAnimatedImages.erase(it);
    }
}

MessageDelegateHelperImage::RunningAnimatedImage::RunningAnimatedImage(const QModelIndex &idx)
    : index(idx), movie(new QMovie)
{
}

MessageDelegateHelperImage::RunningAnimatedImage::~RunningAnimatedImage()
{
    // Note that this happens (with a nullptr movie) when the vector is re-allocated
    delete movie;
}

MessageDelegateHelperImage::RunningAnimatedImage::RunningAnimatedImage(RunningAnimatedImage &&other) noexcept
    : index(other.index), movie(other.movie)
{
    other.movie = nullptr;
}

MessageDelegateHelperImage::RunningAnimatedImage &MessageDelegateHelperImage::RunningAnimatedImage::operator=(RunningAnimatedImage &&other)
{
    index = other.index;
    movie = other.movie;
    other.movie = nullptr;
    return *this;
}
