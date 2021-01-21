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

#include "messageattachmentdelegatehelperimage.h"
#include "ruqolawidgets_debug.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include "dialogs/showimagedialog.h"
#include "common/delegatepaintutil.h"
#include "common/delegateutil.h"

#include <KLocalizedString>

#include <QAbstractItemView>
#include <QAbstractTextDocumentLayout>
#include <QMessageBox>
#include <QMouseEvent>
#include <QMovie>
#include <QPainter>
#include <QPixmapCache>
#include <QPointer>
#include <QStyleOptionViewItem>

void MessageAttachmentDelegateHelperImage::draw(const MessageAttachment &msgAttach, QPainter *painter, QRect messageRect, const QModelIndex &index, const QStyleOptionViewItem &option) const
{
    const ImageLayout layout = layoutImage(msgAttach, option, messageRect.width(), messageRect.height());
    painter->drawText(messageRect.x(), messageRect.y() + option.fontMetrics.ascent(), layout.title);
    int nextY = messageRect.y() + layout.titleSize.height() + DelegatePaintUtil::margin();
    if (!layout.pixmap.isNull()) {
        // Draw title and buttons
        const QIcon hideShowIcon = QIcon::fromTheme(layout.isShown ? QStringLiteral("visibility") : QStringLiteral("hint"));
        hideShowIcon.paint(painter, layout.hideShowButtonRect.translated(messageRect.topLeft()));
        const QIcon downloadIcon = QIcon::fromTheme(QStringLiteral("cloud-download"));
        downloadIcon.paint(painter, layout.downloadButtonRect.translated(messageRect.topLeft()));

        // Draw main pixmap (if shown)
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
                    auto view = qobject_cast<QAbstractItemView *>(const_cast<QWidget *>(option.widget));
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
                scaledPixmap.setDevicePixelRatio(option.widget->devicePixelRatioF());
            } else {
                scaledPixmap = layout.pixmap.scaled(layout.imageSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            }
            painter->drawPixmap(messageRect.x(), nextY, scaledPixmap);
            nextY += scaledPixmap.height() / scaledPixmap.devicePixelRatioF() + DelegatePaintUtil::margin();
        }
    }

    drawDescription(msgAttach, messageRect, painter, nextY);
}

QSize MessageAttachmentDelegateHelperImage::sizeHint(const MessageAttachment &msgAttach, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const
{
    Q_UNUSED(index)
    const ImageLayout layout = layoutImage(msgAttach, option, maxWidth, -1);
    int height = layout.titleSize.height() + DelegatePaintUtil::margin();
    int pixmapWidth = 0;
    if (layout.isShown) {
        pixmapWidth = qMin(layout.pixmap.width(), maxWidth);
        height += qMin(layout.pixmap.height(), 200) + DelegatePaintUtil::margin();
    }
    int descriptionWidth = 0;
    if (!layout.description.isEmpty()) {
        descriptionWidth = layout.descriptionSize.width();
        height += layout.descriptionSize.height() + DelegatePaintUtil::margin();
    }
    return {qMax(qMax(pixmapWidth, layout.titleSize.width()), descriptionWidth),
            height};
}

bool MessageAttachmentDelegateHelperImage::handleMouseEvent(const MessageAttachment &msgAttach, QMouseEvent *mouseEvent, QRect attachmentsRect, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (mouseEvent->type() == QEvent::MouseButtonRelease) {
        const QPoint pos = mouseEvent->pos();

        const ImageLayout layout = layoutImage(msgAttach, option, attachmentsRect.width(), attachmentsRect.height());
        if (layout.hideShowButtonRect.translated(attachmentsRect.topLeft()).contains(pos)) {
            MessageModel::AttachmentVisibility attachmentVisibility;
            attachmentVisibility.show = !layout.isShown;
            attachmentVisibility.attachmentId = msgAttach.attachmentId();
            auto *model = const_cast<QAbstractItemModel *>(index.model());
            model->setData(index, QVariant::fromValue(attachmentVisibility), MessageModel::DisplayAttachment);
            return true;
        } else if (layout.downloadButtonRect.translated(attachmentsRect.topLeft()).contains(pos)) {
            auto *parentWidget = const_cast<QWidget *>(option.widget);
            const auto file = DelegateUtil::querySaveFileName(parentWidget, i18n("Save Image"), QUrl::fromLocalFile(layout.imagePath));
            if (!file.isEmpty()) {
                if (!QFile::remove(file)) { // copy() doesn't overwrite
                    qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to remove : " << file;
                }
                QFile sourceFile(layout.imagePath);
                if (!sourceFile.copy(file)) {
                    QMessageBox::warning(parentWidget, i18n("Error saving file"), sourceFile.errorString());
                }
            }
            return true;
        } else if (!layout.pixmap.isNull()) {
            const int imageY = attachmentsRect.y() + layout.titleSize.height() + DelegatePaintUtil::margin();
            const QRect imageRect(attachmentsRect.x(), imageY, layout.imageSize.width(), layout.imageSize.height());
            if (imageRect.contains(pos)) {
                auto *parentWidget = const_cast<QWidget *>(option.widget);
                auto dlg = new ShowImageDialog(parentWidget);
                dlg->setIsAnimatedPixmap(layout.isAnimatedImage);
                dlg->setAttribute(Qt::WA_DeleteOnClose);
                if (layout.isAnimatedImage) {
                    dlg->setImagePath(layout.imagePath);
                } else {
                    dlg->setImage(layout.pixmap);
                }
                dlg->show();
            }
            return true;
        }
    }
    return false;
}

MessageAttachmentDelegateHelperImage::ImageLayout MessageAttachmentDelegateHelperImage::layoutImage(const MessageAttachment &msgAttach, const QStyleOptionViewItem &option, int attachmentsWidth, int attachmentsHeight) const
{
    ImageLayout layout;
    const QUrl url = Ruqola::self()->rocketChatAccount()->attachmentUrl(msgAttach.link());
    layout.title = msgAttach.title();
    layout.description = msgAttach.description();
    layout.titleSize = option.fontMetrics.size(Qt::TextSingleLine, layout.title);

    layout.descriptionSize = documentDescriptionForIndexSize(msgAttach, attachmentsWidth);

    if (url.isLocalFile()) {
        layout.imagePath = url.toLocalFile();
        layout.pixmap = mPixmapCache.pixmapForLocalFile(layout.imagePath);
        layout.pixmap.setDevicePixelRatio(option.widget->devicePixelRatioF());
        //or we could do layout.attachment = msgAttach; if we need many fields from it
        layout.isShown = msgAttach.showAttachment();
        layout.isAnimatedImage = msgAttach.isAnimatedImage();
        const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);
        layout.hideShowButtonRect = QRect(layout.titleSize.width() + DelegatePaintUtil::margin(), 0, iconSize, iconSize);
        layout.downloadButtonRect = layout.hideShowButtonRect.translated(iconSize + DelegatePaintUtil::margin(), 0);

        if (attachmentsHeight > 0) {
            // Vertically: attachmentsHeight = title | DelegatePaintUtil::margin() | image | DelegatePaintUtil::margin() [| description | DelegatePaintUtil::margin()]
            int imageMaxHeight = attachmentsHeight - layout.titleSize.height() - DelegatePaintUtil::margin() * 2;
            if (!layout.description.isEmpty()) {
                imageMaxHeight -= layout.descriptionSize.height() + DelegatePaintUtil::margin();
            }
            const auto dpr = layout.pixmap.devicePixelRatioF();
            layout.imageSize = layout.pixmap.size().scaled(attachmentsWidth * dpr, imageMaxHeight * dpr, Qt::KeepAspectRatio);
        }
    }
    return layout;
}

std::vector<RunningAnimatedImage>::iterator MessageAttachmentDelegateHelperImage::findRunningAnimatedImage(const QModelIndex &index) const
{
    auto matchesIndex = [&](const RunningAnimatedImage &rai) {
                            return rai.index == index;
                        };
    return std::find_if(mRunningAnimatedImages.begin(), mRunningAnimatedImages.end(), matchesIndex);
}

void MessageAttachmentDelegateHelperImage::removeRunningAnimatedImage(const QModelIndex &index) const
{
    auto it = findRunningAnimatedImage(index);
    if (it != mRunningAnimatedImages.end()) {
        mRunningAnimatedImages.erase(it);
    }
}
