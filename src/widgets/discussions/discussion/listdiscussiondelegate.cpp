/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "listdiscussiondelegate.h"

#include "config-ruqola.h"
#if USE_SIZEHINT_CACHE_SUPPORT
#include "ruqola_sizehint_cache_debug.h"
#endif
#include <KColorScheme>
#include <KLocalizedString>
#include <QAbstractItemView>
#include <QListView>
#include <QPainter>
#include <QToolTip>
#include <utility>

#include "colorsandmessageviewstyle.h"
#include "common/delegatepaintutil.h"
#include "delegateutils/messagedelegateutils.h"
#include "delegateutils/textselectionimpl.h"
#include "misc/avatarcachemanager.h"
#include "model/discussionsmodel.h"
#include "rocketchataccount.h"

using namespace Qt::Literals::StringLiterals;
ListDiscussionDelegate::ListDiscussionDelegate(QListView *view, RocketChatAccount *account, QObject *parent)
    : MessageListDelegateBase(view, parent)
    , mRocketChatAccount(account)
    , mAvatarCacheManager(new AvatarCacheManager(Utils::AvatarType::User, this))
{
    mAvatarCacheManager->setCurrentRocketChatAccount(mRocketChatAccount);
}

ListDiscussionDelegate::~ListDiscussionDelegate() = default;

// [date]
// text
// number of discussion + last date
// add text for opening discussion.
void ListDiscussionDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    drawBackground(painter, option, index);

    const Layout layout = doLayout(option, index);

    // Draw the pixmap
    if (!layout.avatarPixmap.isNull()) {
#if USE_ROUNDED_RECT_PIXMAP
        DelegatePaintUtil::createClipRoundedRectangle(painter, QRectF(layout.avatarPos, layout.avatarPixmap.deviceIndependentSize()), layout.avatarPixmap);
#else
        painter->drawPixmap(layout.avatarPos, layout.avatarPixmap);
#endif
    }

    // Draw the sender
    const QFont oldFont = painter->font();
    painter->setFont(layout.senderFont);
    painter->drawText(layout.senderRect.x(), layout.baseLine, layout.senderText);
    painter->setFont(oldFont);

    // Draw Text
    if (layout.textRect.isValid()) {
        auto *doc = documentForModelIndex(index, layout.textRect.width());
        if (doc) {
            MessageDelegateUtils::drawSelection(doc,
                                                layout.textRect,
                                                layout.textRect.top(),
                                                painter,
                                                index,
                                                option,
                                                mTextSelectionImpl->textSelection(),
                                                {},
                                                {},
                                                false);
        }
    }

    // Draw the number of message + timestamp (below the sender)
    const QString messageStr = i18np("%1 message", "%1 messages", layout.numberOfMessages) + u' ' + layout.lastMessageTimeText;
    DelegatePaintUtil::drawLighterText(painter, messageStr, QPoint(layout.textRect.left(), layout.lastMessageTimeY + painter->fontMetrics().ascent()));

    painter->setPen(option.palette.link().color());
    painter->drawText(layout.textRect.x(), layout.openDiscussionTextY + painter->fontMetrics().ascent(), layout.openDiscussionText);

    // debug
    // painter->drawRect(option.rect.adjusted(0, 0, -1, -1));
    painter->restore();
}

QSize ListDiscussionDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
#if USE_SIZEHINT_CACHE_SUPPORT
    const QByteArray identifier = cacheIdentifier(index);
    auto it = mSizeHintCache.find(identifier);
    if (it != mSizeHintCache.end()) {
        const QSize result = it->value;
        qCDebug(RUQOLA_SIZEHINT_CACHE_LOG) << "ListDiscussionDelegate: SizeHint found in cache: " << result;
        return result;
    }
#endif
    // Note: option.rect in this method is huge (as big as the viewport)
    const Layout layout = doLayout(option, index);

    int additionalHeight = 0;
    // A little bit of margin below the very last item, it just looks better
    if (index.row() == index.model()->rowCount() - 1) {
        additionalHeight += 4;
    }

    // contents is text + number of messages/timestamp + "open discussion" line
    const int contentsHeight = qRound(layout.openDiscussionTextY + layout.openDiscussionTextHeight) - option.rect.y();
    const int senderAndAvatarHeight = qMax<int>(layout.senderRect.y() + layout.senderRect.height() - option.rect.y(),
                                                layout.avatarPos.y() + MessageDelegateUtils::dprAwareSize(layout.avatarPixmap).height() - option.rect.y());

    //    qDebug() << "senderAndAvatarHeight" << senderAndAvatarHeight << "text" << layout.textRect.height() << "total contents" << contentsHeight;
    //    qDebug() << "=> returning" << qMax(senderAndAvatarHeight, contentsHeight) + additionalHeight;

    const QSize size = {option.rect.width(), qMax(senderAndAvatarHeight, contentsHeight) + additionalHeight};
#if USE_SIZEHINT_CACHE_SUPPORT
    if (!size.isEmpty()) {
        mSizeHintCache.insert(identifier, size);
    }
#endif
    return size;
}

bool ListDiscussionDelegate::helpEvent(QHelpEvent *helpEvent, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (!helpEvent || !view || !index.isValid()) {
        return QItemDelegate::helpEvent(helpEvent, view, option, index);
    }

    if (helpEvent->type() != QEvent::ToolTip) {
        return false;
    }

    const Layout layout = doLayout(option, index);
    const auto *doc = documentForModelIndex(index, layout.textRect.width());
    if (!doc) {
        return false;
    }

    const QPoint relativePos = adaptMousePosition(helpEvent->pos(), layout.textRect, option);
    QString formattedTooltip;
    if (MessageDelegateUtils::generateToolTip(doc, relativePos, formattedTooltip)) {
        QToolTip::showText(helpEvent->globalPos(), formattedTooltip, view);
        return true;
    }
    return false;
}

bool ListDiscussionDelegate::mouseEvent(QEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    const QEvent::Type eventType = event->type();
    if (eventType == QEvent::MouseButtonRelease) {
        auto mev = static_cast<QMouseEvent *>(event);
        const Layout layout = doLayout(option, index);

        // Only follow the link when the press started on it too, otherwise a text selection
        // ending over the link would open the discussion.
        const QPersistentModelIndex pressedIndex = std::exchange(mOpenDiscussionPressedIndex, {});
        if (pressedIndex == index && openDiscussionRect(layout, option).contains(mev->pos())) {
            const QByteArray discussionRoomId = index.data(DiscussionsModel::DiscussionRoomId).toByteArray();
            Q_EMIT openDiscussion(discussionRoomId);
            return true;
        }
        if (handleMouseEvent(mev, layout.textRect, option, index)) {
            return true;
        }
    } else if (eventType == QEvent::MouseButtonPress || eventType == QEvent::MouseMove || eventType == QEvent::MouseButtonDblClick) {
        auto mev = static_cast<QMouseEvent *>(event);
        if (mev->buttons() & Qt::LeftButton) {
            const Layout layout = doLayout(option, index);
            if (eventType == QEvent::MouseButtonPress || eventType == QEvent::MouseButtonDblClick) {
                mOpenDiscussionPressedIndex = openDiscussionRect(layout, option).contains(mev->pos()) ? QPersistentModelIndex(index) : QPersistentModelIndex();
            }
            if (handleMouseEvent(mev, layout.textRect, option, index)) {
                return true;
            }
        }
    }
    return false;
}

QRect ListDiscussionDelegate::openDiscussionRect(const Layout &layout, const QStyleOptionViewItem &option) const
{
    const int width = option.fontMetrics.horizontalAdvance(layout.openDiscussionText);
    return QRect(layout.textRect.x(), qRound(layout.openDiscussionTextY), width, qRound(layout.openDiscussionTextHeight));
}

QPoint ListDiscussionDelegate::adaptMousePosition(const QPoint &pos, QRect textRect, [[maybe_unused]] const QStyleOptionViewItem &option)
{
    const QPoint relativePos = pos - textRect.topLeft();
    return relativePos;
}

QPixmap ListDiscussionDelegate::makeAvatarPixmap(const QWidget *widget, const QModelIndex &index, int maxHeight) const
{
    Utils::AvatarInfo info;
    info.avatarType = Utils::AvatarType::User;
    info.identifier = index.data(DiscussionsModel::UserName).toString();

    return mAvatarCacheManager->makeAvatarPixmap(widget, info, maxHeight);
}

ListDiscussionDelegate::Layout ListDiscussionDelegate::doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Layout layout;
    const QString userName = index.data(DiscussionsModel::UserName).toString();
    const int margin = MessageDelegateUtils::basicMargin();
    layout.senderText = u'@' + userName;
    layout.senderFont = option.font;
    layout.senderFont.setBold(true);

    // Message (using the rest of the available width)
    const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);
    const QFontMetricsF senderFontMetrics(layout.senderFont);
    const qreal senderAscent = senderFontMetrics.ascent();
    const QSizeF senderTextSize = senderFontMetrics.size(Qt::TextSingleLine, layout.senderText);

    const QPixmap pix = makeAvatarPixmap(option.widget, index, senderTextSize.height());
    if (!pix.isNull()) {
        layout.avatarPixmap = pix;
    }

    const int senderX = option.rect.x() + MessageDelegateUtils::dprAwareSize(layout.avatarPixmap).width() + 2 * margin;

    const int textLeft = senderX + senderTextSize.width() + margin;
    const int widthAfterMessage = iconSize + margin + margin / 2;
    const int maxWidth = qMax(30, option.rect.width() - textLeft - widthAfterMessage);

    layout.baseLine = 0;
    const QSize textSize = textSizeHint(index, maxWidth, option, &layout.baseLine);

    const int textVMargin = 3; // adjust this for "compactness"
    QRect usableRect = option.rect;
    layout.textRect = QRect(textLeft, usableRect.top() + textVMargin, maxWidth, textSize.height() + textVMargin);
    layout.baseLine += layout.textRect.top(); // make it absolute

    layout.senderRect = QRectF(senderX, layout.baseLine - senderAscent, senderTextSize.width(), senderTextSize.height());
    // Align top of avatar with top of sender rect
    layout.avatarPos = QPointF(option.rect.x() + margin, layout.senderRect.y());

    layout.lastMessageTimeText = index.data(DiscussionsModel::LastMessage).toString();
    layout.lastMessageTimeY = layout.textRect.bottom();

    layout.numberOfMessages = index.data(DiscussionsModel::NumberOfMessages).toInt();

    layout.openDiscussionText = i18n("Open Discussion");
    layout.openDiscussionTextY = layout.lastMessageTimeY + option.fontMetrics.height();
    layout.openDiscussionTextHeight = option.fontMetrics.height();

    return layout;
}

QByteArray ListDiscussionDelegate::cacheIdentifier(const QModelIndex &index) const
{
    const QByteArray discussionRoomId = index.data(DiscussionsModel::DiscussionRoomId).toByteArray();
    Q_ASSERT(!discussionRoomId.isEmpty());
    return discussionRoomId;
}

QTextDocument *ListDiscussionDelegate::documentForModelIndex(const QModelIndex &index, int width) const
{
    Q_ASSERT(index.isValid());
    const QByteArray messageId = cacheIdentifier(index);
    const QString messageStr = index.data(DiscussionsModel::Description).toString();
    return documentForDelegate(mRocketChatAccount, messageId, messageStr, width);
}

bool ListDiscussionDelegate::maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    const Layout layout = doLayout(option, index);
    if (MessageListDelegateBase::maybeStartDrag(event, layout.textRect, option, index)) {
        return true;
    }
    return false;
}

RocketChatAccount *ListDiscussionDelegate::rocketChatAccount([[maybe_unused]] const QModelIndex &index) const
{
    return mRocketChatAccount;
}

#include "moc_listdiscussiondelegate.cpp"
