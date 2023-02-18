/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagedelegatehelperconferencevideo.h"
#include "common/delegatepaintutil.h"
#include "conferencecalldialog/conferenceinfodialog.h"
#include "rocketchataccount.h"

#include <KLocalizedString>

#include <QAbstractItemView>
#include <QAbstractTextDocumentLayout>
#include <QListView>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QToolTip>

MessageDelegateHelperConferenceVideo::MessageDelegateHelperConferenceVideo(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl)
    : MessageBlockDelegateHelperBase(account, view, textSelectionImpl)
    , mInfoIcon(QIcon::fromTheme(QStringLiteral("documentinfo")))
{
}

MessageDelegateHelperConferenceVideo::~MessageDelegateHelperConferenceVideo() = default;

// Title [margin] <icon info>
// Button join [margin] <list avatar user>
//
void MessageDelegateHelperConferenceVideo::draw(const Block &block,
                                                QPainter *painter,
                                                QRect blockRect,
                                                const QModelIndex &index,
                                                const QStyleOptionViewItem &option) const
{
    Q_UNUSED(index)
    const ConferenceCallLayout layout = layoutConferenceCall(block, option, blockRect.width());
    // Draw title and buttons
    int positionY = blockRect.y() + option.fontMetrics.ascent();
    painter->drawText(blockRect.x(), positionY, layout.title);
    mInfoIcon.paint(painter, layout.infoButtonRect.translated(blockRect.topLeft()));

    if (layout.canJoin) {
        // Draw join button
        const QPen origPen = painter->pen();
        const QBrush origBrush = painter->brush();
        const QPen buttonPen(option.palette.color(QPalette::Highlight).darker());
        QColor backgroundColor = option.palette.color(QPalette::Highlight);
        backgroundColor.setAlpha(60);
        const QBrush buttonBrush(backgroundColor);
        const QRectF joinButtonRect = layout.joinButtonRect.translated(blockRect.topLeft());
        // Rounded rect
        painter->setPen(buttonPen);
        painter->setBrush(buttonBrush);
        painter->drawRoundedRect(joinButtonRect, 5, 5);
        painter->setBrush(origBrush);
        painter->setPen(origPen);
        const QRectF r = joinButtonRect.adjusted((joinButtonRect.width() - layout.joinButtonTextSize.width()) / 2, 0, 0, 0);
        painter->drawText(r, layout.joinButtonText);
    }

    // Draw avatars!
    const QPen origPen = painter->pen();
    const QBrush origBrush = painter->brush();
    const QPen buttonPen(option.palette.color(QPalette::Highlight).darker());
    QColor backgroundColor = option.palette.color(QPalette::Highlight);
    backgroundColor.setAlpha(60);
    const QBrush buttonBrush(backgroundColor);

    for (const UserLayout &userLayout : layout.usersLayout) {
        const QRectF avatarRect = userLayout.userAvatarRect.translated(blockRect.topLeft());
        // Rounded rect
        painter->setPen(buttonPen);
        painter->setBrush(buttonBrush);
        painter->drawRoundedRect(avatarRect.adjusted(0, 0, -1, -1), 5, 5);
        painter->setBrush(origBrush);
        painter->setPen(origPen);
    }
    // drawDescription(block, messageRect, painter, nextY, index, option);
}

QSize MessageDelegateHelperConferenceVideo::sizeHint(const Block &block, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const
{
    Q_UNUSED(index)
    const ConferenceCallLayout layout = layoutConferenceCall(block, option, maxWidth);
    int height = layout.titleSize.height() + DelegatePaintUtil::margin();
#if 0
    int descriptionWidth = 0;
    if (!layout.description.isEmpty()) {
        descriptionWidth = layout.descriptionSize.width();
        height += layout.descriptionSize.height() + DelegatePaintUtil::margin();
    }
    return {qMax(qMax(0, layout.titleSize.width()), descriptionWidth), height};
#else
    // Button
    if (layout.canJoin) {
        height += layout.joinButtonRect.height();
    } else if (!layout.usersLayout.isEmpty()) {
        height += 10 + DelegatePaintUtil::margin(); // TODO customize it
    }
    return {qMax(0, layout.titleSize.width()), height};
#endif
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
    Q_UNUSED(index);
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
        if (layout.canJoin) {
            if (layout.joinButtonRect.translated(blocksRect.topLeft()).contains(pos)) {
                if (!block.videoConferenceInfo().url().isEmpty()) {
                    Q_EMIT mRocketChatAccount->openLinkRequested(block.videoConferenceInfo().url());
                    return true;
                }
                return true;
            }
        }
        break;
    }
    default:
        break;
    }

    return false;
}

bool MessageDelegateHelperConferenceVideo::handleHelpEvent(QHelpEvent *helpEvent, QRect blockRect, const Block &block, const QStyleOptionViewItem &option)
{
    const ConferenceCallLayout layout = layoutConferenceCall(block, option, blockRect.width());
    for (const UserLayout &userLayout : layout.usersLayout) {
        if (userLayout.userAvatarRect.translated(blockRect.topLeft()).contains(helpEvent->pos())) {
            QToolTip::showText(helpEvent->globalPos(), userLayout.userName, mListView);
            return true;
        }
    }
    return false;
}

MessageDelegateHelperConferenceVideo::ConferenceCallLayout
MessageDelegateHelperConferenceVideo::layoutConferenceCall(const Block &block, const QStyleOptionViewItem &option, int blockRectWidth) const
{
    Q_UNUSED(blockRectWidth)
    ConferenceCallLayout layout;
    layout.title = block.title();
    layout.titleSize = option.fontMetrics.size(Qt::TextSingleLine, layout.title);
    // layout.descriptionSize = documentDescriptionForIndexSize(block, attachmentsWidth);
    const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);
    layout.infoButtonRect = QRect(layout.titleSize.width() + DelegatePaintUtil::margin(), 0, iconSize, iconSize);
    // Join Button
    layout.canJoin = block.videoConferenceInfo().canJoin();
    if (layout.canJoin) {
        layout.joinButtonText = i18n("Join");
        layout.joinButtonTextSize = option.fontMetrics.size(Qt::TextSingleLine, layout.joinButtonText);
        layout.joinButtonRect =
            QRect(0, layout.infoButtonRect.height() + DelegatePaintUtil::margin(), layout.joinButtonTextSize.width() * 2, layout.joinButtonTextSize.height());
    }
    qreal x = 0;
    const QVector<User> users = block.videoConferenceInfo().users();
    for (const auto &user : users) {
        UserLayout userLayout;
        userLayout.userName = user.userName();
        userLayout.userId = user.userId();
        userLayout.userAvatarRect = QRectF((layout.canJoin ? layout.joinButtonTextSize.width() * 2 + DelegatePaintUtil::margin() : 0) + x,
                                           layout.infoButtonRect.height() + DelegatePaintUtil::margin(),
                                           iconSize,
                                           iconSize);
        layout.usersLayout.append(std::move(userLayout));
        x += iconSize + DelegatePaintUtil::margin();
    }
    return layout;
}

QTextDocument *MessageDelegateHelperConferenceVideo::documentForIndex(const QModelIndex &index) const
{
    Q_UNUSED(index);
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
