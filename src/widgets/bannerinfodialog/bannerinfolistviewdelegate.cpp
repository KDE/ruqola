/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannerinfolistviewdelegate.h"
#include "config-ruqola.h"
#include "model/bannerinfosmodel.h"
#if USE_SIZEHINT_CACHE_SUPPORT
#include "ruqola_sizehint_cache_debug.h"
#endif

#include <KColorScheme>
#include <KLocalizedString>
#include <QAbstractItemView>
#include <QListView>
#include <QPainter>
#include <QToolTip>

#include "delegateutils/messagedelegateutils.h"
#include "delegateutils/textselectionimpl.h"
#include "rocketchataccount.h"

BannerInfoListViewDelegate::BannerInfoListViewDelegate(QListView *view, RocketChatAccount *account, QObject *parent)
    : MessageListDelegateBase(view, parent)
    , mRocketChatAccount(account)
{
}

BannerInfoListViewDelegate::~BannerInfoListViewDelegate() = default;

void BannerInfoListViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    const Layout layout = doLayout(option, index);

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

    // debug
    // painter->drawRect(option.rect.adjusted(0, 0, -1, -1));
    painter->restore();
}

QSize BannerInfoListViewDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
#if USE_SIZEHINT_CACHE_SUPPORT
    const QByteArray identifier = cacheIdentifier(index);
    auto it = mSizeHintCache.find(identifier);
    if (it != mSizeHintCache.end()) {
        const QSize result = it->value;
        qCDebug(RUQOLA_SIZEHINT_CACHE_LOG) << "BannerInfoListViewDelegate: SizeHint found in cache: " << result;
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

    // contents is date + text
    const int contentsHeight = layout.textRect.height() - option.rect.y();
    //    qDebug() << "senderAndAvatarHeight" << senderAndAvatarHeight << "text" << layout.textRect.height() << "total contents" << contentsHeight;
    //    qDebug() << "=> returning" << qMax(senderAndAvatarHeight, contentsHeight) + additionalHeight;

    const QSize size = {option.rect.width(), contentsHeight + additionalHeight};
#if USE_SIZEHINT_CACHE_SUPPORT
    if (!size.isEmpty()) {
        mSizeHintCache.insert(identifier, size);
    }
#endif
    return size;
}

bool BannerInfoListViewDelegate::helpEvent(QHelpEvent *helpEvent, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index)
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
    return true;
}

bool BannerInfoListViewDelegate::mouseEvent(QEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    const QEvent::Type eventType = event->type();
    if (eventType == QEvent::MouseButtonRelease) {
        auto mev = static_cast<QMouseEvent *>(event);
        const Layout layout = doLayout(option, index);

        if (handleMouseEvent(mev, layout.textRect, option, index)) {
            return true;
        }
    } else if (eventType == QEvent::MouseButtonPress || eventType == QEvent::MouseMove || eventType == QEvent::MouseButtonDblClick) {
        auto mev = static_cast<QMouseEvent *>(event);
        if (mev->buttons() & Qt::LeftButton) {
            const Layout layout = doLayout(option, index);
            if (handleMouseEvent(mev, layout.textRect, option, index)) {
                return true;
            }
        }
    }
    return false;
}

QPoint BannerInfoListViewDelegate::adaptMousePosition(const QPoint &pos, QRect textRect, const QStyleOptionViewItem &option)
{
    Q_UNUSED(option);
    const QPoint relativePos = pos - textRect.topLeft();
    return relativePos;
}

BannerInfoListViewDelegate::Layout BannerInfoListViewDelegate::doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Layout layout;
    // Message (using the rest of the available width)
    const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);

    const int senderX = option.rect.x();
    const int margin = MessageDelegateUtils::basicMargin();
    const int textLeft = senderX + margin;
    const int widthAfterMessage = iconSize + margin + margin / 2;
    const int maxWidth = qMax(30, option.rect.width() - textLeft - widthAfterMessage);

    layout.baseLine = 0;
    const QSize textSize = textSizeHint(index, maxWidth, option, &layout.baseLine);

    const int textVMargin = 3; // adjust this for "compactness"
    QRect usableRect = option.rect;
    layout.textRect = QRect(textLeft, usableRect.top() + textVMargin, maxWidth, textSize.height() + textVMargin);
    layout.baseLine += layout.textRect.top(); // make it absolute
    return layout;
}

QByteArray BannerInfoListViewDelegate::cacheIdentifier(const QModelIndex &index) const
{
    const QByteArray identifier = index.data(BannerInfosModel::Identifier).toByteArray();
    Q_ASSERT(!identifier.isEmpty());
    return identifier;
}

QTextDocument *BannerInfoListViewDelegate::documentForModelIndex(const QModelIndex &index, int width) const
{
    Q_ASSERT(index.isValid());
    const QByteArray messageId = cacheIdentifier(index);
    const QString messageBannerStr = index.data(BannerInfosModel::Text).toString();
    return documentForDelegate(mRocketChatAccount, messageId, messageBannerStr, width);
}

bool BannerInfoListViewDelegate::maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    const Layout layout = doLayout(option, index);
    if (MessageListDelegateBase::maybeStartDrag(event, layout.textRect, option, index)) {
        return true;
    }
    return false;
}

RocketChatAccount *BannerInfoListViewDelegate::rocketChatAccount(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return mRocketChatAccount;
}

#include "moc_bannerinfolistviewdelegate.cpp"
