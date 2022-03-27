/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageattachmentdelegatehelperimage.h"
#include "common/delegatepaintutil.h"
#include "common/delegateutil.h"
#include "dialogs/showimagedialog.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include "ruqolawidgets_selection_debug.h"

#include <KLocalizedString>

#include <KMessageBox>
#include <QAbstractItemView>
#include <QAbstractTextDocumentLayout>
#include <QMouseEvent>
#include <QMovie>
#include <QPainter>
#include <QPixmapCache>
#include <QStyleOptionViewItem>

MessageAttachmentDelegateHelperImage::MessageAttachmentDelegateHelperImage(QListView *view, TextSelection *textSelection)
    : MessageDelegateHelperBase(view, textSelection)
{
}

void MessageAttachmentDelegateHelperImage::draw(const MessageAttachment &msgAttach,
                                                QPainter *painter,
                                                QRect messageRect,
                                                const QModelIndex &index,
                                                const QStyleOptionViewItem &option) const
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
                    rai.movie->setFileName(layout.imagePreviewPath);
                    rai.movie->setScaledSize(layout.imageSize);
                    auto view = qobject_cast<QAbstractItemView *>(const_cast<QWidget *>(option.widget));
                    const QPersistentModelIndex &idx = rai.index;
                    QObject::connect(
                        rai.movie,
                        &QMovie::frameChanged,
                        view,
                        [view, idx, this]() {
                            if (view->viewport()->rect().contains(view->visualRect(idx))) {
                                view->update(idx);
                            } else {
                                removeRunningAnimatedImage(idx);
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
    }

    drawDescription(msgAttach, messageRect, painter, nextY, index, option);
}

QSize MessageAttachmentDelegateHelperImage::sizeHint(const MessageAttachment &msgAttach,
                                                     const QModelIndex &index,
                                                     int maxWidth,
                                                     const QStyleOptionViewItem &option) const
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
            MessageModel::AttachmentVisibility attachmentVisibility;
            attachmentVisibility.show = !layout.isShown;
            attachmentVisibility.attachmentId = msgAttach.attachmentId();
            auto *model = const_cast<QAbstractItemModel *>(index.model());
            model->setData(index, QVariant::fromValue(attachmentVisibility), MessageModel::DisplayAttachment);
            return true;
        } else if (layout.downloadButtonRect.translated(attachmentsRect.topLeft()).contains(pos)) {
            auto parentWidget = const_cast<QWidget *>(option.widget);
            DelegateUtil::saveFile(parentWidget, layout.imagePreviewPath, i18n("Save Image"));
            return true;
        } else if (!layout.pixmap.isNull()) {
            const int imageY = attachmentsRect.y() + layout.titleSize.height() + DelegatePaintUtil::margin();
            const QRect imageRect(attachmentsRect.x(), imageY, layout.imageSize.width(), layout.imageSize.height());
            if (imageRect.contains(pos)) {
                auto parentWidget = const_cast<QWidget *>(option.widget);
                auto dlg = new ShowImageDialog(Ruqola::self()->rocketChatAccount(), parentWidget);
                dlg->setAttribute(Qt::WA_DeleteOnClose);
                ShowImageWidget::ImageInfo info;
                info.bigImagePath = layout.imageBigPath;
                info.previewImagePath = layout.imagePreviewPath;
                info.needToDownloadBigImage = !Ruqola::self()->rocketChatAccount()->attachmentIsInLocalCache(layout.imageBigPath);
                info.pixmap = layout.pixmap;
                info.isAnimatedImage = layout.isAnimatedImage;
                dlg->setImageInfo(info);
                dlg->show();
            }
            return true;
        }
        break;
    }
#if 1
    case QEvent::MouseMove: {
        if (!mMightStartDrag) {
            if (const auto *doc = documentDescriptionForIndex(msgAttach, attachmentsRect.width() /*, true*/)) { // FIXME ME!
                const QPoint pos = mouseEvent->pos();
                const ImageLayout layout = layoutImage(msgAttach, option, attachmentsRect.width(), attachmentsRect.height());
                const QPoint mouseClickPos = pos - attachmentsRect.topLeft() - QPoint(0, /*layout.titleRect.height() +*/ DelegatePaintUtil::margin());
                const int charPos = doc->documentLayout()->hitTest(mouseClickPos, Qt::FuzzyHit);
                if (charPos != -1) {
                    // QWidgetTextControl also has code to support isPreediting()/commitPreedit(), selectBlockOnTripleClick
                    mSelection->setEnd(index, charPos);
                    return true;
                }
            }
        }
        break;
    }
    case QEvent::MouseButtonDblClick: {
        if (!mSelection->hasSelection()) {
            if (const auto *doc = documentDescriptionForIndex(msgAttach, attachmentsRect.width() /*, true*/)) { // FIXME ME!
                const ImageLayout layout = layoutImage(msgAttach, option, attachmentsRect.width(), attachmentsRect.height());
                const QPoint pos = mouseEvent->pos();
                const QPoint mouseClickPos = pos - attachmentsRect.topLeft() - QPoint(0, /*layout.titleRect.height() +*/ DelegatePaintUtil::margin());
                const int charPos = doc->documentLayout()->hitTest(mouseClickPos, Qt::FuzzyHit);
                qCDebug(RUQOLAWIDGETS_SELECTION_LOG) << "double-clicked at pos" << charPos;
                if (charPos == -1) {
                    return false;
                }
                mSelection->selectWordUnderCursor(index, msgAttach, charPos, this);
                return true;
            }
        }
        break;
    }
    case QEvent::MouseButtonPress: {
        mMightStartDrag = false;
        if (const auto *doc = documentDescriptionForIndex(msgAttach, attachmentsRect.width() /*, true*/)) { // FIXME ME!
            const ImageLayout layout = layoutImage(msgAttach, option, attachmentsRect.width(), attachmentsRect.height());
            const QPoint pos = mouseEvent->pos();
            const QPoint mouseClickPos = pos - attachmentsRect.topLeft() - QPoint(0, /*layout.titleRect.height() +*/ DelegatePaintUtil::margin());
            const int charPos = doc->documentLayout()->hitTest(mouseClickPos, Qt::FuzzyHit);
            qCDebug(RUQOLAWIDGETS_SELECTION_LOG) << "pressed at pos" << charPos;
            if (charPos == -1) {
                return false;
            }
            if (mSelection->contains(index, charPos) && doc->documentLayout()->hitTest(pos, Qt::ExactHit) != -1) {
                mMightStartDrag = true;
                return true;
            }

            // QWidgetTextControl also has code to support selectBlockOnTripleClick, shift to extend selection
            // (look there if you want to add these things)

            mSelection->setStart(index, charPos);
            return true;
        } else {
            mSelection->clear();
        }
        break;
    }
#endif
    default:
        break;
    }

    return MessageDelegateHelperBase::handleMouseEvent(msgAttach, mouseEvent, attachmentsRect, option, index);
}

MessageAttachmentDelegateHelperImage::ImageLayout MessageAttachmentDelegateHelperImage::layoutImage(const MessageAttachment &msgAttach,
                                                                                                    const QStyleOptionViewItem &option,
                                                                                                    int attachmentsWidth,
                                                                                                    int attachmentsHeight) const
{
    ImageLayout layout;
    const QUrl previewImageUrl = Ruqola::self()->rocketChatAccount()->attachmentUrlFromLocalCache(msgAttach.imageUrlPreview());
    layout.title = msgAttach.title();
    layout.description = msgAttach.description();
    layout.titleSize = option.fontMetrics.size(Qt::TextSingleLine, layout.title);

    layout.descriptionSize = documentDescriptionForIndexSize(msgAttach, attachmentsWidth);

    if (previewImageUrl.isLocalFile()) {
        layout.imagePreviewPath = previewImageUrl.toLocalFile();
        layout.imageBigPath = msgAttach.link();
        layout.pixmap = mPixmapCache.pixmapForLocalFile(layout.imagePreviewPath);
        layout.pixmap.setDevicePixelRatio(option.widget->devicePixelRatioF());
        // or we could do layout.attachment = msgAttach; if we need many fields from it
        layout.isShown = msgAttach.showAttachment();
        layout.isAnimatedImage = msgAttach.isAnimatedImage();
        const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);
        layout.hideShowButtonRect = QRect(layout.titleSize.width() + DelegatePaintUtil::margin(), 0, iconSize, iconSize);
        layout.downloadButtonRect = layout.hideShowButtonRect.translated(iconSize + DelegatePaintUtil::margin(), 0);

        if (attachmentsHeight > 0) {
            // Vertically: attachmentsHeight = title | DelegatePaintUtil::margin() | image | DelegatePaintUtil::margin() [| description |
            // DelegatePaintUtil::margin()]
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
