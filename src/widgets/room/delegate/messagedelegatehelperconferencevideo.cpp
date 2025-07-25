/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagedelegatehelperconferencevideo.h"
using namespace Qt::Literals::StringLiterals;

#include "common/delegatepaintutil.h"
#include "conferencecalldialog/conferenceinfodialog.h"
#include "config-ruqola.h"
#include "misc/avatarcachemanager.h"
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
    , mInfoIcon(QIcon::fromTheme(u"documentinfo"_s))
    , mAvatarCacheManager(new AvatarCacheManager(Utils::AvatarType::User, this))
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
    const int positionY = blockRect.y() + option.fontMetrics.ascent();
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
        painter->drawText(r, i18n("Join"));
    }

    // Draw avatars!
    for (const UserLayout &userLayout : layout.usersLayout) {
        const QRectF avatarRect = userLayout.userAvatarRect.translated(blockRect.topLeft());
#if USE_ROUNDED_RECT_PIXMAP
        DelegatePaintUtil::createClipRoundedRectangle(painter,
                                                      QRectF(avatarRect.topLeft(), avatarRect.toRect().size()),
                                                      avatarRect.topLeft(),
                                                      userLayout.avatarPixmap);
#else
        painter->drawPixmap(avatarRect.toRect(), userLayout.avatarPixmap);
#endif
    }
}

QSize MessageDelegateHelperConferenceVideo::sizeHint(const Block &block, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const
{
    Q_UNUSED(index)
    const ConferenceCallLayout layout = layoutConferenceCall(block, option, maxWidth);
    int height = layout.titleSize.height() + DelegatePaintUtil::margin();
    // Button
    if (layout.canJoin) {
        height += layout.joinButtonRect.height();
    } else if (!layout.usersLayout.isEmpty()) {
        height += 10 + DelegatePaintUtil::margin(); // TODO customize it
    }
    return {qMax(0, layout.titleSize.width()), height};
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
    const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);
    layout.infoButtonRect = QRect(layout.titleSize.width() + DelegatePaintUtil::margin(), 0, iconSize, iconSize);
    // Join Button
    layout.canJoin = block.videoConferenceInfo().canJoin();
    if (layout.canJoin) {
        layout.joinButtonTextSize = option.fontMetrics.size(Qt::TextSingleLine, i18n("Join"));
        layout.joinButtonRect =
            QRect(0, layout.infoButtonRect.height() + DelegatePaintUtil::margin(), layout.joinButtonTextSize.width() * 2, layout.joinButtonTextSize.height());
    }
    // Users
    qreal x = 0;
    const QList<User> users = block.videoConferenceInfo().users();
    for (const auto &user : users) {
        UserLayout userLayout;
        userLayout.userName = user.userName();
        userLayout.avatarPixmap = makeAvatarPixmap(userLayout.userName, option.widget, layout.titleSize.height());
        userLayout.userAvatarRect = QRectF((layout.canJoin ? layout.joinButtonTextSize.width() * 2 + DelegatePaintUtil::margin() : 0) + x,
                                           layout.infoButtonRect.height() + DelegatePaintUtil::margin(),
                                           iconSize,
                                           iconSize);
        layout.usersLayout.append(std::move(userLayout));
        x += iconSize + DelegatePaintUtil::margin();
    }
    return layout;
}

void MessageDelegateHelperConferenceVideo::setRocketChatAccount(RocketChatAccount *newRocketChatAccount)
{
    mAvatarCacheManager->setCurrentRocketChatAccount(newRocketChatAccount);
    MessageBlockDelegateHelperBase::setRocketChatAccount(newRocketChatAccount);
}

QPixmap MessageDelegateHelperConferenceVideo::makeAvatarPixmap(const QString &identifier, const QWidget *widget, int maxHeight) const
{
    Utils::AvatarInfo info;
    info.avatarType = Utils::AvatarType::User;
    info.identifier = identifier;
    return mAvatarCacheManager->makeRoundedAvatarPixmap(widget, info, maxHeight);
}
