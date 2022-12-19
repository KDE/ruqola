/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagedelegatehelperconferencevideo.h"
#include "common/delegatepaintutil.h"
#include "common/delegateutil.h"
#include "rocketchataccount.h"

#include <KLocalizedString>

#include <QAbstractItemView>
#include <QAbstractTextDocumentLayout>
#include <QListView>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOptionViewItem>

MessageDelegateHelperConferenceVideo::MessageDelegateHelperConferenceVideo(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl)
    : MessageBlockDelegateHelperBase(account, view, textSelectionImpl)
{
}

MessageDelegateHelperConferenceVideo::~MessageDelegateHelperConferenceVideo() = default;

void MessageDelegateHelperConferenceVideo::draw(const Block &block,
                                                QPainter *painter,
                                                QRect messageRect,
                                                const QModelIndex &index,
                                                const QStyleOptionViewItem &option) const
{
    const ConferenceCallLayout layout = layoutConferenceCall(block, option, messageRect.width());
#if 0
    // Draw title and buttons
    painter->drawText(messageRect.x(), messageRect.y() + option.fontMetrics.ascent(), layout.title);

    const int nextY = messageRect.y() + layout.titleSize.height() + DelegatePaintUtil::margin();
    drawDescription(msgAttach, messageRect, painter, nextY, index, option);
#endif
}

QSize MessageDelegateHelperConferenceVideo::sizeHint(const Block &block, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const
{
    Q_UNUSED(index)
#if 0
    const ConferenceCallLayout layout = layoutConferenceCall(block, option, maxWidth);
    int height = layout.titleSize.height() + DelegatePaintUtil::margin();
    int descriptionWidth = 0;
    if (!layout.description.isEmpty()) {
        descriptionWidth = layout.descriptionSize.width();
        height += layout.descriptionSize.height() + DelegatePaintUtil::margin();
    }
    return {qMax(qMax(0, layout.titleSize.width()), descriptionWidth), height};
#endif
    return {};
}

QPoint
MessageDelegateHelperConferenceVideo::adaptMousePosition(const QPoint &pos, const Block &block, QRect attachmentsRect, const QStyleOptionViewItem &option)
{
#if 0
    const ConferenceCallLayout layout = layoutConferenceCall(block, option, attachmentsRect.width());
    const QPoint relativePos = pos - attachmentsRect.topLeft() - QPoint(0, layout.titleSize.height() + DelegatePaintUtil::margin());
    return relativePos;
#endif
    return {};
}

bool MessageDelegateHelperConferenceVideo::handleMouseEvent(const Block &block,
                                                            QMouseEvent *mouseEvent,
                                                            QRect attachmentsRect,
                                                            const QStyleOptionViewItem &option,
                                                            const QModelIndex &index)
{
    const QEvent::Type eventType = mouseEvent->type();
    switch (eventType) {
    case QEvent::MouseButtonRelease: {
        const QPoint pos = mouseEvent->pos();

        const ConferenceCallLayout layout = layoutConferenceCall(block, option, attachmentsRect.width());
        break;
    }
    default:
        break;
    }

    return false;
}

MessageDelegateHelperConferenceVideo::ConferenceCallLayout
MessageDelegateHelperConferenceVideo::layoutConferenceCall(const Block &block, const QStyleOptionViewItem &option, int attachmentsWidth) const
{
    ConferenceCallLayout layout;
#if 0
    const QUrl url = mRocketChatAccount->attachmentUrlFromLocalCache(msgAttach.link());
    // or we could do layout.attachment = msgAttach; if we need many fields from it
    layout.title = msgAttach.title();
    layout.description = msgAttach.description();
    layout.titleSize = option.fontMetrics.size(Qt::TextSingleLine, layout.title);
    layout.descriptionSize = documentDescriptionForIndexSize(msgAttach, attachmentsWidth);
    const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);
    layout.playerVolumeButtonRect = QRect(layout.titleSize.width() + DelegatePaintUtil::margin(), 0, iconSize, iconSize);
    layout.downloadButtonRect = layout.playerVolumeButtonRect.translated(iconSize + DelegatePaintUtil::margin(), 0);
    if (url.isLocalFile()) {
        layout.audioPath = url.toLocalFile();
    }
#endif
    return layout;
}

QTextDocument *MessageDelegateHelperConferenceVideo::documentForIndex(const QModelIndex &index) const
{
    return nullptr;
}

QTextDocument *MessageDelegateHelperConferenceVideo::documentForIndex(const MessageAttachment &msgAttach) const
{
    Q_UNUSED(msgAttach);
    Q_ASSERT(false);
    return nullptr;
}
