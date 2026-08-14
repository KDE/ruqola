/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageattachmentdelegatehelperimage.h"

#include "common/delegatepaintutil.h"
#include "dialogs/showimagedialog.h"
#include "misc/messageattachmentdownloadandsavejob.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolaglobalconfig.h"
#include "ruqolawidgets_debug.h"

#include <KLocalizedString>

#include <QAbstractItemView>
#include <QAbstractTextDocumentLayout>
#include <QApplication>
#include <QClipboard>
#include <QMenu>
#include <QMimeData>
#include <QMouseEvent>
#include <QMovie>
#include <QPainter>
#include <QPixmapCache>
#include <QStyleOptionViewItem>

using namespace Qt::Literals::StringLiterals;
MessageAttachmentDelegateHelperImage::MessageAttachmentDelegateHelperImage(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl)
    : MessageAttachmentDelegateHelperBase(account, view, textSelectionImpl)
{
    mPixmapCache.setMaxEntries(32); // Enough ?
}

void MessageAttachmentDelegateHelperImage::draw(const MessageAttachment &msgAttach,
                                                QPainter *painter,
                                                QRect messageRect,
                                                const QModelIndex &index,
                                                const QStyleOptionViewItem &option) const
{
    const ImageLayout layout = layoutImage(msgAttach, option, messageRect.width(), messageRect.height());
    // Only an animated attachment that we actually paint may keep a running QMovie: otherwise it would
    // go on emitting frameChanged() -> view->update() forever, repainting something we don't draw.
    const bool animateImage = !layout.pixmap.isNull() && layout.isShown && layout.isAnimatedImage && RuqolaGlobalConfig::self()->animateGifImage();
    const QByteArray attachmentId = msgAttach.attachmentId();
    if (!animateImage) {
        removeRunningAnimatedImage(index, attachmentId);
    }
    // drawTitle(msgAttach, painter, );
    painter->drawText(messageRect.x(), messageRect.y() + option.fontMetrics.ascent(), layout.title);
    int nextY = messageRect.y() + layout.titleSize.height() + DelegatePaintUtil::margin();
    const QIcon downloadIcon = QIcon::fromTheme(u"cloud-download"_s);
    if (!layout.pixmap.isNull()) {
        // Draw title and buttons
        const QIcon hideShowIcon = QIcon::fromTheme(layout.isShown ? u"visibility"_s : u"hint"_s);
        hideShowIcon.paint(painter, layout.hideShowButtonRect.translated(messageRect.topLeft()));
        downloadIcon.paint(painter, layout.downloadButtonRect.translated(messageRect.topLeft()));

        // Draw main pixmap (if shown)
        if (layout.isShown) {
            QPixmap scaledPixmap;
            if (animateImage) {
                auto it = findRunningAnimatedImage(index, attachmentId);
                if (it != mRunningAnimatedImages.end()) {
                    scaledPixmap = (*it).movie->currentPixmap();
                } else {
                    mRunningAnimatedImages.emplace_back(index, attachmentId);
                    auto &rai = mRunningAnimatedImages.back();
                    rai.movie->setFileName(layout.imagePreviewPath);
                    rai.movie->setScaledSize(layout.imageSize);
                    auto view = qobject_cast<QAbstractItemView *>(const_cast<QWidget *>(option.widget));
                    const QPersistentModelIndex &idx = rai.index;
                    QObject::connect(
                        rai.movie,
                        &QMovie::frameChanged,
                        view,
                        [view, idx, this]() {
                            if (view->viewport()->rect().intersects(view->visualRect(idx))) {
                                view->update(idx);
                            } else {
                                // The whole message is out of sight: stop all its animations, not just this one.
                                removeRunningAnimatedImages(idx);
                            }
                        },
                        Qt::QueuedConnection);
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
    } else {
        if (layout.imagePreviewPath.isEmpty()) {
            // Not a bug, it's just that the image is currently being downloaded by RocketChatCache::downloadFileFromServer
        } else {
            qCWarning(RUQOLAWIDGETS_LOG) << "Invalid image (Qt bug or others). It will not render: " << layout.imagePreviewPath;
            downloadIcon.paint(painter, layout.downloadButtonRect.translated(messageRect.topLeft()));
        }
    }

    drawDescription(msgAttach, messageRect, painter, nextY, index, option);
}

QSize MessageAttachmentDelegateHelperImage::sizeHint(const MessageAttachment &msgAttach,
                                                     [[maybe_unused]] const QModelIndex &index,
                                                     int maxWidth,
                                                     const QStyleOptionViewItem &option) const
{
    const ImageLayout layout = layoutImage(msgAttach, option, maxWidth, -1);
    int height = layout.titleSize.height() + DelegatePaintUtil::margin();
    int pixmapWidth = 0;
    if (layout.isShown) {
        pixmapWidth = qMin(layout.pixmap.width(), maxWidth);
        height += qMin(layout.pixmap.height(), 200) + DelegatePaintUtil::margin();
    }
    int descriptionWidth = 0;
    if (layout.hasDescription) {
        descriptionWidth = layout.descriptionSize.width();
        height += layout.descriptionSize.height() + DelegatePaintUtil::margin();
    }
    return {qMax(qMax(pixmapWidth, layout.titleSize.width()), descriptionWidth), height};
}

bool MessageAttachmentDelegateHelperImage::handleMouseEvent(const MessageAttachment &msgAttach,
                                                            QMouseEvent *mouseEvent,
                                                            QRect attachmentsRect,
                                                            const QStyleOptionViewItem &option,
                                                            const QModelIndex &index)
{
    const QEvent::Type eventType = mouseEvent->type();
    switch (eventType) {
    case QEvent::MouseButtonRelease: {
        const QPoint pos = mouseEvent->pos();
        const ImageLayout layout = layoutImage(msgAttach, option, attachmentsRect.width(), attachmentsRect.height());
        if (layout.hideShowButtonRect.translated(attachmentsRect.topLeft()).contains(pos)) {
            MessagesModel::AttachmentAndUrlPreviewVisibility attachmentVisibility;
            attachmentVisibility.show = !layout.isShown;
            attachmentVisibility.elementId = msgAttach.attachmentId();
            auto model = const_cast<QAbstractItemModel *>(index.model());
            model->setData(index, QVariant::fromValue(attachmentVisibility), MessagesModel::DisplayAttachment);
            return true;
        } else if (layout.downloadButtonRect.translated(attachmentsRect.topLeft()).contains(pos)) {
            MessageAttachmentDownloadAndSaveJob::MessageAttachmentDownloadJobInfo info;
            info.attachmentType = MessageAttachmentDownloadAndSaveJob::AttachmentType::Image;
            info.actionType = MessageAttachmentDownloadAndSaveJob::ActionType::DownloadAndSave;
            info.attachmentPath = layout.imageBigPath;
            info.needToDownloadAttachment = !mRocketChatAccount->attachmentIsInLocalCache(info.attachmentPath);
            info.parentWidget = const_cast<QWidget *>(option.widget);
            auto job = new MessageAttachmentDownloadAndSaveJob(this);
            job->setRocketChatAccount(mRocketChatAccount);
            job->setInfo(info);
            job->start();
            return true;
        } else if (layout.isShown && !layout.pixmap.isNull()) {
            // imageSize is in device pixels (as the cached pixmap is), draw() paints it scaled down by the dpr.
            const qreal dpr = layout.pixmap.devicePixelRatioF();
            const int imageY = attachmentsRect.y() + layout.titleSize.height() + DelegatePaintUtil::margin();
            const QRect imageRect(attachmentsRect.x(), imageY, layout.imageSize.width() / dpr, layout.imageSize.height() / dpr);
            if (imageRect.contains(pos)) {
                auto parentWidget = const_cast<QWidget *>(option.widget);
                auto dlg = new ShowImageDialog(mRocketChatAccount, parentWidget);
                dlg->setAttribute(Qt::WA_DeleteOnClose);
                ShowImageWidget::ImageInfo info;
                info.bigImagePath = layout.imageBigPath;
                info.previewImagePath = layout.imagePreviewPath;
                info.needToDownloadBigImage = !mRocketChatAccount->attachmentIsInLocalCache(info.bigImagePath);
                info.pixmap = layout.pixmap;
                info.isAnimatedImage = layout.isAnimatedImage;
                dlg->setImageInfo(info);
                dlg->show();
                return true;
            }
        }
        break;
    }
    default:
        break;
    }
    return MessageAttachmentDelegateHelperBase::handleMouseEvent(msgAttach, mouseEvent, attachmentsRect, option, index);
}

MessageAttachmentDelegateHelperImage::ImageLayout MessageAttachmentDelegateHelperImage::layoutImage(const MessageAttachment &msgAttach,
                                                                                                    const QStyleOptionViewItem &option,
                                                                                                    int attachmentsWidth,
                                                                                                    int attachmentsHeight) const
{
    ImageLayout layout;
    // Laurent keep Ruqola::self()->rocketChatAccount() for autotest
    const QUrl previewImageUrl = mRocketChatAccount ? mRocketChatAccount->attachmentUrlFromLocalCache(msgAttach.imageUrlPreview())
                                                    : Ruqola::self()->rocketChatAccount()->attachmentUrlFromLocalCache(msgAttach.imageUrlPreview());
    layout.title = msgAttach.attachmentGeneratedTitle();
    layout.hasDescription = msgAttach.hasDescription();
    layout.titleSize = option.fontMetrics.size(Qt::TextSingleLine, layout.title);

    layout.descriptionSize = documentTypeForIndexSize(convertAttachmentToDocumentDescriptionInfo(msgAttach, attachmentsWidth));

    if (!previewImageUrl.isEmpty() && previewImageUrl.isLocalFile()) {
        layout.imagePreviewPath = previewImageUrl.toLocalFile();
        layout.imageBigPath = msgAttach.link();
        layout.pixmap = mPixmapCache.pixmapForLocalFile(layout.imagePreviewPath, option.widget->devicePixelRatioF());
        // or we could do layout.attachment = msgAttach; if we need many fields from it
        layout.isShown = msgAttach.showAttachment();
        layout.isAnimatedImage = msgAttach.isAnimatedImage();
        const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);
        const QRect firstIconRect = QRect(layout.titleSize.width() + DelegatePaintUtil::margin(), 0, iconSize, iconSize);
        if (!layout.pixmap.isNull()) {
            layout.hideShowButtonRect = firstIconRect;
            layout.downloadButtonRect = layout.hideShowButtonRect.translated(iconSize + DelegatePaintUtil::margin(), 0);
        } else {
            layout.downloadButtonRect = firstIconRect;
        }

        if (attachmentsHeight > 0) {
            // Vertically: attachmentsHeight = title | DelegatePaintUtil::margin() | image | DelegatePaintUtil::margin() [| description |
            // DelegatePaintUtil::margin()]
            int imageMaxHeight = attachmentsHeight - layout.titleSize.height() - DelegatePaintUtil::margin() * 2;
            if (layout.hasDescription) {
                imageMaxHeight -= layout.descriptionSize.height() + DelegatePaintUtil::margin();
            }
            const auto dpr = layout.pixmap.devicePixelRatioF();
            layout.imageSize = layout.pixmap.size().scaled(attachmentsWidth * dpr, imageMaxHeight * dpr, Qt::KeepAspectRatio);
        }
    }
    return layout;
}

std::vector<RunningAnimatedImage>::iterator MessageAttachmentDelegateHelperImage::findRunningAnimatedImage(const QModelIndex &index,
                                                                                                           const QByteArray &identifier) const
{
    auto matchesImage = [&](const RunningAnimatedImage &rai) {
        return rai.index == index && rai.identifier == identifier;
    };
    return std::find_if(mRunningAnimatedImages.begin(), mRunningAnimatedImages.end(), matchesImage);
}

void MessageAttachmentDelegateHelperImage::removeRunningAnimatedImage(const QModelIndex &index, const QByteArray &identifier) const
{
    auto it = findRunningAnimatedImage(index, identifier);
    if (it != mRunningAnimatedImages.end()) {
        mRunningAnimatedImages.erase(it);
    }
}

void MessageAttachmentDelegateHelperImage::removeRunningAnimatedImages(const QModelIndex &index) const
{
    std::erase_if(mRunningAnimatedImages, [&](const RunningAnimatedImage &rai) {
        return rai.index == index;
    });
}

QPoint MessageAttachmentDelegateHelperImage::adaptMousePosition(const QPoint &pos,
                                                                const MessageAttachment &msgAttach,
                                                                QRect attachmentsRect,
                                                                const QStyleOptionViewItem &option)
{
    const ImageLayout layout = layoutImage(msgAttach, option, attachmentsRect.width(), attachmentsRect.height());
    // Same vertical layout as draw(): title | margin [| image | margin] | description
    int descriptionY = layout.titleSize.height() + DelegatePaintUtil::margin();
    if (layout.isShown && !layout.pixmap.isNull()) {
        descriptionY += layout.imageSize.height() / layout.pixmap.devicePixelRatioF() + DelegatePaintUtil::margin();
    }
    const QPoint relativePos = pos - attachmentsRect.topLeft() - QPoint(0, descriptionY);
    return relativePos;
}

bool MessageAttachmentDelegateHelperImage::contextMenu(const QPoint &pos,
                                                       [[maybe_unused]] const QPoint &globalPos,
                                                       const MessageAttachment &msgAttach,
                                                       QRect attachmentsRect,
                                                       const QStyleOptionViewItem &option,
                                                       QMenu *menu)
{
    const ImageLayout layout = layoutImage(msgAttach, option, attachmentsRect.width(), attachmentsRect.height());
    if (layout.isShown && !layout.pixmap.isNull()) {
        const QRect rectAdjusted = attachmentsRect.adjusted(0, 0, 0, -(layout.titleSize.height() + DelegatePaintUtil::margin()));
        if (rectAdjusted.contains(pos)) {
            auto copyImageAction = new QAction(QIcon::fromTheme(u"edit-copy"_s), i18n("Copy Image to Clipboard"), menu);
            connect(copyImageAction, &QAction::triggered, this, [layout]() {
                auto data = new QMimeData();
                data->setImageData(layout.pixmap.toImage());
                data->setData(u"x-kde-force-image-copy"_s, QByteArray());
                QApplication::clipboard()->setMimeData(data, QClipboard::Clipboard);
            });
            menu->addAction(copyImageAction);
            menu->addSeparator();
            return true;
        }
    }
    return false;
}
