/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageattachmentdelegatehelpervideo.h"

#include "common/delegatepaintutil.h"
#include "dialogs/showvideodialog.h"
#include "misc/messageattachmentdownloadandsavejob.h"
#include "rocketchataccount.h"

#include <KLocalizedString>

#include <QAbstractItemView>
#include <QAbstractTextDocumentLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOptionViewItem>

using namespace Qt::Literals::StringLiterals;
MessageAttachmentDelegateHelperVideo::MessageAttachmentDelegateHelperVideo(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl)
    : MessageAttachmentDelegateHelperBase(account, view, textSelectionImpl)
    , mDownloadIcon(QIcon::fromTheme(u"cloud-download"_s))
    , mVisibilityIcon(QIcon::fromTheme(u"visibility"_s))
{
}

MessageAttachmentDelegateHelperVideo::~MessageAttachmentDelegateHelperVideo() = default;

void MessageAttachmentDelegateHelperVideo::draw(const MessageAttachment &msgAttach,
                                                QPainter *painter,
                                                QRect messageRect,
                                                const QModelIndex &index,
                                                const QStyleOptionViewItem &option) const
{
    const VideoLayout layout = layoutVideo(msgAttach, option, messageRect.width());
    // drawTitle(msgAttach, painter, );
    // Draw title and buttons
    painter->drawText(messageRect.x(), messageRect.y() + option.fontMetrics.ascent(), layout.title);

    mVisibilityIcon.paint(painter, layout.showButtonRect.translated(messageRect.topLeft()));
    if (layout.downloadButtonRect.isValid()) {
        mDownloadIcon.paint(painter, layout.downloadButtonRect.translated(messageRect.topLeft()));
    }

    const int nextY = messageRect.y() + layout.headerSize.height() + DelegatePaintUtil::margin();

    drawDescription(msgAttach, messageRect, painter, nextY, index, option);
}

QSize MessageAttachmentDelegateHelperVideo::sizeHint(const MessageAttachment &msgAttach,
                                                     [[maybe_unused]] const QModelIndex &index,
                                                     int maxWidth,
                                                     const QStyleOptionViewItem &option) const
{
    const VideoLayout layout = layoutVideo(msgAttach, option, maxWidth);
    int height = layout.headerSize.height() + DelegatePaintUtil::margin();
    int descriptionWidth = 0;
    if (layout.hasDescription) {
        descriptionWidth = layout.descriptionSize.width();
        height += layout.descriptionSize.height() + DelegatePaintUtil::margin();
    }
    return {qMax(layout.headerSize.width(), descriptionWidth), height};
}

bool MessageAttachmentDelegateHelperVideo::handleMouseEvent(const MessageAttachment &msgAttach,
                                                            QMouseEvent *mouseEvent,
                                                            QRect attachmentsRect,
                                                            const QStyleOptionViewItem &option,
                                                            const QModelIndex &index)
{
    const QEvent::Type eventType = mouseEvent->type();
    switch (eventType) {
    case QEvent::MouseButtonRelease: {
        // Only a plain left click acts on the buttons: a release ending a text selection must go to the
        // base class, otherwise selecting the description pops up the video dialog.
        if (mouseEvent->button() != Qt::LeftButton || mTextSelectionImpl->textSelection()->hasSelection()) {
            break;
        }
        const QPoint pos = mouseEvent->pos();

        // downloadButtonRect is invalid when the attachment can't be downloaded => it contains() nothing
        const VideoLayout layout = layoutVideo(msgAttach, option, attachmentsRect.width());
        if (layout.downloadButtonRect.translated(attachmentsRect.topLeft()).contains(pos)) {
            MessageAttachmentDownloadAndSaveJob::MessageAttachmentDownloadJobInfo info;
            info.attachmentType = MessageAttachmentDownloadAndSaveJob::AttachmentType::Video;
            info.actionType = MessageAttachmentDownloadAndSaveJob::ActionType::DownloadAndSave;
            info.attachmentPath = layout.videoPath;
            info.needToDownloadAttachment = !mRocketChatAccount->attachmentIsInLocalCache(info.attachmentPath);
            info.parentWidget = const_cast<QWidget *>(option.widget);
            auto job = new MessageAttachmentDownloadAndSaveJob(this);
            job->setRocketChatAccount(mRocketChatAccount);
            job->setInfo(info);
            job->start();
            return true;
        } else if (QRect(attachmentsRect.topLeft(), layout.titleSize).contains(pos)
                   || layout.showButtonRect.translated(attachmentsRect.topLeft()).contains(pos)) {
            auto parentWidget = const_cast<QWidget *>(option.widget);
            ShowVideoDialog dlg(mRocketChatAccount, parentWidget);
            dlg.setVideoPath(layout.videoPath);
            dlg.exec();
            return true;
        }
        break;
    }
    default:
        break;
    }

    return MessageAttachmentDelegateHelperBase::handleMouseEvent(msgAttach, mouseEvent, attachmentsRect, option, index);
}

MessageAttachmentDelegateHelperVideo::VideoLayout
MessageAttachmentDelegateHelperVideo::layoutVideo(const MessageAttachment &msgAttach, const QStyleOptionViewItem &option, int attachmentsWidth) const
{
    VideoLayout layout;
    // or we could do layout.attachment = msgAttach; if we need many fields from it
    layout.title = msgAttach.attachmentGeneratedTitle();
    layout.hasDescription = msgAttach.hasDescription();
    layout.titleSize = option.fontMetrics.size(Qt::TextSingleLine, layout.title);
    layout.descriptionSize = documentTypeForIndexSize(convertAttachmentToDocumentDescriptionInfo(msgAttach, attachmentsWidth));
    const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);
    const int headerHeight = qMax(layout.titleSize.height(), iconSize);
    const int iconY = (headerHeight - iconSize) / 2;
    layout.showButtonRect = QRect(layout.titleSize.width() + DelegatePaintUtil::margin(), iconY, iconSize, iconSize);
    int headerWidth = layout.showButtonRect.right() + 1;
    if (msgAttach.canDownloadAttachment()) {
        layout.downloadButtonRect = layout.showButtonRect.translated(iconSize + DelegatePaintUtil::margin(), 0);
        headerWidth = layout.downloadButtonRect.right() + 1;
    }
    layout.headerSize = QSize(headerWidth, headerHeight);
    layout.videoPath = msgAttach.link();
    return layout;
}

QPoint MessageAttachmentDelegateHelperVideo::adaptMousePosition(const QPoint &pos,
                                                                const MessageAttachment &msgAttach,
                                                                QRect attachmentsRect,
                                                                const QStyleOptionViewItem &option)
{
    const VideoLayout layout = layoutVideo(msgAttach, option, attachmentsRect.width());
    const QPoint relativePos = pos - attachmentsRect.topLeft() - QPoint(0, layout.headerSize.height() + DelegatePaintUtil::margin());
    return relativePos;
}
