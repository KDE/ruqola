/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagedelegatehelperconferencevideo.h"
#include "common/delegatepaintutil.h"
#include "common/delegateutil.h"
#include "conferencecalldialog/conferenceinfodialog.h"
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
    , mInfoIcon(QIcon::fromTheme(QStringLiteral("documentinfo")))
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
    // Draw title and buttons
    painter->drawText(messageRect.x(), messageRect.y() + option.fontMetrics.ascent(), layout.title);
    mInfoIcon.paint(painter, layout.infoButtonRect.translated(messageRect.topLeft()));
    const int nextY = messageRect.y() + layout.titleSize.height() + DelegatePaintUtil::margin();
    drawDescription(block, messageRect, painter, nextY, index, option);
}

QSize MessageDelegateHelperConferenceVideo::sizeHint(const Block &block, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const
{
    Q_UNUSED(index)
    const ConferenceCallLayout layout = layoutConferenceCall(block, option, maxWidth);
    int height = layout.titleSize.height() + DelegatePaintUtil::margin();
    int descriptionWidth = 0;
    if (!layout.description.isEmpty()) {
        descriptionWidth = layout.descriptionSize.width();
        height += layout.descriptionSize.height() + DelegatePaintUtil::margin();
    }
    return {qMax(qMax(0, layout.titleSize.width()), descriptionWidth), height};
}

QPoint MessageDelegateHelperConferenceVideo::adaptMousePosition(const QPoint &pos, const Block &block, QRect blocksRect, const QStyleOptionViewItem &option)
{
    const ConferenceCallLayout layout = layoutConferenceCall(block, option, blocksRect.width());
    const QPoint relativePos = pos - blocksRect.topLeft() - QPoint(0, layout.titleSize.height() + DelegatePaintUtil::margin());
    return relativePos;
}

bool MessageDelegateHelperConferenceVideo::handleMouseEvent(const Block &block,
                                                            QMouseEvent *mouseEvent,
                                                            QRect blocksRect,
                                                            const QStyleOptionViewItem &option,
                                                            const QModelIndex &index)
{
    const QEvent::Type eventType = mouseEvent->type();
    switch (eventType) {
    case QEvent::MouseButtonRelease: {
        const QPoint pos = mouseEvent->pos();

        const ConferenceCallLayout layout = layoutConferenceCall(block, option, blocksRect.width());
        if (layout.infoButtonRect.translated(blocksRect.topLeft()).contains(pos)) {
            auto parentWidget = const_cast<QWidget *>(option.widget);
            ConferenceInfoDialog dlg(mRocketChatAccount, parentWidget);
            dlg.setConferenceId(block.callId());
            dlg.initializeInfo();
            dlg.exec();
            return true;
        }
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
    layout.title = block.generateDescription();
    layout.titleSize = option.fontMetrics.size(Qt::TextSingleLine, layout.title);
    layout.descriptionSize = documentDescriptionForIndexSize(block, attachmentsWidth);
    const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);
    layout.infoButtonRect = QRect(layout.titleSize.width() + DelegatePaintUtil::margin(), 0, iconSize, iconSize);
    return layout;
}

QTextDocument *MessageDelegateHelperConferenceVideo::documentForIndex(const QModelIndex &index) const
{
    return nullptr;
}

QTextDocument *MessageDelegateHelperConferenceVideo::documentForIndex(const MessageAttachment &msgAttach) const
{
    // Unused.
    Q_UNUSED(msgAttach);
    Q_ASSERT(false);
    return nullptr;
}

QTextDocument *MessageDelegateHelperConferenceVideo::documentForIndex(const Block &block) const
{
    Q_UNUSED(block);
    Q_ASSERT(false);
    return nullptr;
}
