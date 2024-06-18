/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsdelegate.h"
#include "common/delegatepaintutil.h"
#include "config-ruqola.h"
#include "delegateutils/messagedelegateutils.h"
#include "delegateutils/textselectionimpl.h"
#include "model/appsmarketplacemodel.h"
#include "rocketchataccount.h"
#if USE_SIZEHINT_CACHE_SUPPORT
#include "ruqola_sizehint_cache_debug.h"
#endif
#include <QPainter>
#include <QTextDocument>
#include <QTreeView>

ApplicationsSettingsDelegate::ApplicationsSettingsDelegate(RocketChatAccount *account, QAbstractItemView *view, QObject *parent)
    : MessageListDelegateBase{view, parent}
    , mRocketChatAccount(account)
{
}

ApplicationsSettingsDelegate::~ApplicationsSettingsDelegate() = default;

// Use big icon 2 lines
// Short description + apps name
// Primium
void ApplicationsSettingsDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    drawBackground(painter, option, index);

    const Layout layout = doLayout(option, index);
    // Draw the pixmap
    if (!layout.appPixmap.isNull()) {
#if USE_ROUNDED_RECT_PIXMAP
        DelegatePaintUtil::createClipRoundedRectangle(painter, QRectF(layout.appPixmapPos, layout.appPixmap.size()), layout.appPixmapPos, layout.appPixmap);
#else
        painter->drawPixmap(layout.appPixmapPos, layout.appPixmap);
#endif
    }

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
    painter->restore();
}

QSize ApplicationsSettingsDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
#if USE_SIZEHINT_CACHE_SUPPORT
    const QByteArray identifier = cacheIdentifier(index);
    auto it = mSizeHintCache.find(identifier);
    if (it != mSizeHintCache.end()) {
        const QSize result = it->value;
        qCDebug(RUQOLA_SIZEHINT_CACHE_LOG) << "ApplicationsSettingsDelegate: SizeHint found in cache: " << result;
        return result;
    }
#endif

    // Note: option.rect in this method is huge (as big as the viewport)
    const ApplicationsSettingsDelegate::Layout layout = doLayout(option, index);
    const QSize size = {option.rect.width(), layout.textRect.height()};
#if USE_SIZEHINT_CACHE_SUPPORT
    if (!size.isEmpty()) {
        mSizeHintCache.insert(identifier, size);
    }
#endif
    return size;
}

ApplicationsSettingsDelegate::Layout ApplicationsSettingsDelegate::doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    ApplicationsSettingsDelegate::Layout layout;
    const auto pix = index.data(AppsMarketPlaceModel::Pixmap).value<QPixmap>();
    const int iconWidth = 40;
    const int margin = MessageDelegateUtils::basicMargin();
    if (!pix.isNull()) {
        const QPixmap scaledPixmap = pix.scaled(iconWidth, iconWidth, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        layout.appPixmap = scaledPixmap;
        layout.appPixmapPos = QPointF(option.rect.x() + margin, option.rect.top());
    }
    layout.premium = index.data(AppsMarketPlaceModel::IsEnterpriseOnly).toBool();

    QRect usableRect = option.rect;
    const int maxWidth = qMax(iconWidth, option.rect.width() - iconWidth - 2 * margin);

    qreal baseLine = 0;

    auto *doc = documentForModelIndex(index, maxWidth);
    const QSize textSize = MessageDelegateUtils::textSizeHint(doc, &baseLine);

    layout.textRect = QRect(iconWidth + 2 * margin, usableRect.top(), maxWidth, qMax(textSize.height(), iconWidth + margin) /* + textVMargin*/);

    return layout;
}

QByteArray ApplicationsSettingsDelegate::cacheIdentifier(const QModelIndex &index) const
{
    const QByteArray identifier = index.data(AppsMarketPlaceModel::AppId).toByteArray();
    Q_ASSERT(!identifier.isEmpty());
    return identifier;
}

QTextDocument *ApplicationsSettingsDelegate::documentForModelIndex(const QModelIndex &index, int width) const
{
    Q_ASSERT(index.isValid());
    const QByteArray appsId = cacheIdentifier(index);
    const QString shortDescription = index.data(AppsMarketPlaceModel::ShortDescription).toString();
    const QString appName = index.data(AppsMarketPlaceModel::AppName).toString();

    QString fullDescription = QStringLiteral("*%1*").arg(appName);
    if (!shortDescription.isEmpty()) {
        fullDescription += QStringLiteral("\n*%2*").arg(shortDescription);
    }

    return documentForDelegate(mRocketChatAccount, appsId, fullDescription, width);
}

RocketChatAccount *ApplicationsSettingsDelegate::rocketChatAccount(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return mRocketChatAccount;
}

bool ApplicationsSettingsDelegate::mouseEvent(QEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
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

bool ApplicationsSettingsDelegate::maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    const Layout layout = doLayout(option, index);
    if (MessageListDelegateBase::maybeStartDrag(event, layout.textRect, option, index)) {
        return true;
    }
    return false;
}

#include "moc_applicationssettingsdelegate.cpp"
