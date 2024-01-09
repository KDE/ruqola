/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageattachmentdelegatehelpervideo.h"
#include "common/delegatepaintutil.h"
#include "common/delegateutil.h"
#include "dialogs/showvideodialog.h"
#include "misc/messageattachmentdownloadandsavejob.h"
#include "rocketchataccount.h"

#include <KLocalizedString>

#include <QAbstractItemView>
#include <QAbstractTextDocumentLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOptionViewItem>

MessageAttachmentDelegateHelperVideo::MessageAttachmentDelegateHelperVideo(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl)
    : MessageAttachmentDelegateHelperBase(account, view, textSelectionImpl)
    , mDownloadIcon(QIcon::fromTheme(QStringLiteral("cloud-download")))
    , mVisibilityIcon(QIcon::fromTheme(QStringLiteral("visibility")))
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
    // Draw title and buttons
    painter->drawText(messageRect.x(), messageRect.y() + option.fontMetrics.ascent(), layout.title);

    mVisibilityIcon.paint(painter, layout.showButtonRect.translated(messageRect.topLeft()));
    mDownloadIcon.paint(painter, layout.downloadButtonRect.translated(messageRect.topLeft()));

    const int nextY = messageRect.y() + layout.titleSize.height() + DelegatePaintUtil::margin();

    drawDescription(msgAttach, messageRect, painter, nextY, index, option);
}

QSize MessageAttachmentDelegateHelperVideo::sizeHint(const MessageAttachment &msgAttach,
                                                     const QModelIndex &index,
                                                     int maxWidth,
                                                     const QStyleOptionViewItem &option) const
{
    Q_UNUSED(index)
    const VideoLayout layout = layoutVideo(msgAttach, option, maxWidth);
    int height = layout.titleSize.height() + DelegatePaintUtil::margin();
    int descriptionWidth = 0;
    if (layout.hasDescription) {
        descriptionWidth = layout.descriptionSize.width();
        height += layout.descriptionSize.height() + DelegatePaintUtil::margin();
    }
    return {qMax(qMax(0, layout.titleSize.width()), descriptionWidth), height};
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
        const QPoint pos = mouseEvent->pos();

        const VideoLayout layout = layoutVideo(msgAttach, option, attachmentsRect.width());
        if (layout.downloadButtonRect.translated(attachmentsRect.topLeft()).contains(pos)) {
            MessageAttachmentDownloadAndSaveJob::MessageAttachmentDownloadJobInfo info;
            info.attachmentType = MessageAttachmentDownloadAndSaveJob::AttachmentType::Video;
            info.actionType = MessageAttachmentDownloadAndSaveJob::ActionType::DownloadAndSave;
            info.needToDownloadAttachment = !mRocketChatAccount->attachmentIsInLocalCache(layout.videoPath);
            info.parentWidget = const_cast<QWidget *>(option.widget);
            info.attachmentPath = layout.videoPath;
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
    layout.title = msgAttach.title();
    layout.hasDescription = msgAttach.hasDescription();
    layout.titleSize = option.fontMetrics.size(Qt::TextSingleLine, layout.title);
    layout.descriptionSize = documentDescriptionForIndexSize(msgAttach, attachmentsWidth);
    const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);
    layout.showButtonRect = QRect(layout.titleSize.width() + DelegatePaintUtil::margin(), 0, iconSize, iconSize);
    layout.downloadButtonRect = layout.showButtonRect.translated(iconSize + DelegatePaintUtil::margin(), 0);
    layout.videoPath = msgAttach.link();
    return layout;
}

QPoint MessageAttachmentDelegateHelperVideo::adaptMousePosition(const QPoint &pos,
                                                                const MessageAttachment &msgAttach,
                                                                QRect attachmentsRect,
                                                                const QStyleOptionViewItem &option)
{
    const VideoLayout layout = layoutVideo(msgAttach, option, attachmentsRect.width());
    const QPoint relativePos = pos - attachmentsRect.topLeft() - QPoint(0, layout.titleSize.height() + DelegatePaintUtil::margin());
    return relativePos;
}
