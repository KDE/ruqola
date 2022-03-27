/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageattachmentdelegatehelpervideo.h"
#include "common/delegatepaintutil.h"
#include "common/delegateutil.h"
#include "dialogs/showvideodialog.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"

#include <KLocalizedString>

#include <KMessageBox>
#include <QAbstractItemView>
#include <QMouseEvent>
#include <QPainter>
#include <QPointer>
#include <QStyleOptionViewItem>

MessageAttachmentDelegateHelperVideo::MessageAttachmentDelegateHelperVideo(QListView *view, TextSelection *textSelection)
    : MessageDelegateHelperBase(view, textSelection)
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
    Q_UNUSED(index)
    const VideoLayout layout = layoutVideo(msgAttach, option, messageRect.width());
    // Draw title and buttons
    painter->drawText(messageRect.x(), messageRect.y() + option.fontMetrics.ascent(), layout.title);

    mVisibilityIcon.paint(painter, layout.showButtonRect.translated(messageRect.topLeft()));
    mDownloadIcon.paint(painter, layout.downloadButtonRect.translated(messageRect.topLeft()));

    const int nextY = messageRect.y() + layout.titleSize.height() + DelegatePaintUtil::margin();

    drawDescription(msgAttach, messageRect, painter, nextY);
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
    if (!layout.description.isEmpty()) {
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
    if (mouseEvent->type() == QEvent::MouseButtonRelease) {
        const QPoint pos = mouseEvent->pos();

        VideoLayout layout = layoutVideo(msgAttach, option, attachmentsRect.width());
        if (layout.downloadButtonRect.translated(attachmentsRect.topLeft()).contains(pos)) {
            auto parentWidget = const_cast<QWidget *>(option.widget);
            DelegateUtil::saveFile(parentWidget, layout.videoPath, i18n("Save Image"));
            return true;
        } else if (attachmentsRect.contains(pos) || layout.showButtonRect.translated(attachmentsRect.topLeft()).contains(pos)) {
            auto parentWidget = const_cast<QWidget *>(option.widget);
            ShowVideoDialog dlg(parentWidget);
            dlg.setVideoUrl(QUrl::fromLocalFile(layout.videoPath));
            dlg.exec();
            return true;
        }
    }

    return MessageDelegateHelperBase::handleMouseEvent(msgAttach, mouseEvent, attachmentsRect, option, index);
}

MessageAttachmentDelegateHelperVideo::VideoLayout
MessageAttachmentDelegateHelperVideo::layoutVideo(const MessageAttachment &msgAttach, const QStyleOptionViewItem &option, int attachmentsWidth) const
{
    VideoLayout layout;
    const QUrl url = Ruqola::self()->rocketChatAccount()->attachmentUrlFromLocalCache(msgAttach.link());
    // or we could do layout.attachment = msgAttach; if we need many fields from it
    layout.title = msgAttach.title();
    layout.description = msgAttach.description();
    layout.titleSize = option.fontMetrics.size(Qt::TextSingleLine, layout.title);
    layout.descriptionSize = documentDescriptionForIndexSize(msgAttach, attachmentsWidth);
    const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);
    layout.showButtonRect = QRect(layout.titleSize.width() + DelegatePaintUtil::margin(), 0, iconSize, iconSize);
    layout.downloadButtonRect = layout.showButtonRect.translated(iconSize + DelegatePaintUtil::margin(), 0);
    if (url.isLocalFile()) {
        layout.videoPath = url.toLocalFile();
    }
    return layout;
}
