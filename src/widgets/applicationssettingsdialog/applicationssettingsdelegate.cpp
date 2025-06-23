/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsdelegate.h"
using namespace Qt::Literals::StringLiterals;

#include "common/delegatepaintutil.h"
#include "config-ruqola.h"
#include "delegateutils/messagedelegateutils.h"
#include "delegateutils/textselectionimpl.h"
#include "model/appsmarketplacemodel.h"
#include "rocketchataccount.h"
#if USE_SIZEHINT_CACHE_SUPPORT
#include "ruqola_sizehint_cache_debug.h"
#endif
#include "colorsandmessageviewstyle.h"
#include <KLocalizedString>
#include <QPainter>
#include <QTextDocument>
#include <QTreeView>

namespace
{
constexpr uint bordermargin = 5;
}
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

    const int margin = MessageDelegateUtils::basicMargin();
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(ColorsAndMessageViewStyle::self().schemeView().background(KColorScheme::AlternateBackground).color());
    const int offset = static_cast<int>(static_cast<double>(margin) / 2.0);
    painter->drawRoundedRect(option.rect.adjusted(offset, offset, -offset, 0), 5, 5);
    painter->restore();
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
    if (layout.premium) {
        painter->setBrush(ColorsAndMessageViewStyle::self().schemeView().foreground(KColorScheme::PositiveText).color());
        painter->setPen(Qt::NoPen);
        painter->drawRoundedRect(layout.premiumRect.adjusted(-5, 0, 5, 0), bordermargin, bordermargin);
        painter->setPen(Qt::white);
        painter->drawText(layout.premiumRect, layout.premiumText);
    }

    if (layout.status != Status::Unknown) {
        if (layout.status == Status::Enabled) {
            painter->setBrush(ColorsAndMessageViewStyle::self().schemeView().foreground(KColorScheme::PositiveText).color());
        } else {
            painter->setBrush(ColorsAndMessageViewStyle::self().schemeView().foreground(KColorScheme::NeutralText).color());
        }
        painter->setPen(Qt::NoPen);
        painter->drawRoundedRect(layout.statusRect.adjusted(-5, 0, 5, 0), bordermargin, bordermargin);
        painter->setPen(Qt::white);
        painter->drawText(layout.statusRect, layout.statusText);
    } else if (layout.requested) {
        painter->setBrush(ColorsAndMessageViewStyle::self().schemeView().foreground(KColorScheme::NeutralText).color());
        painter->setPen(Qt::NoPen);
        painter->drawRoundedRect(layout.requestedRect.adjusted(-5, 0, 5, 0), bordermargin, bordermargin);
        painter->setPen(Qt::white);
        painter->drawText(layout.requestedRect, layout.requestedText);
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
    const QSize size = {option.rect.width(), layout.textRect.height() + static_cast<int>(MessageDelegateUtils::basicMargin())};
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
    layout.premium = index.data(AppsMarketPlaceModel::IsEnterpriseOnly).toBool();
    layout.premiumText = layout.premium ? i18n("Premium") : QString();
    const QFontMetricsF senderFontMetrics(option.font);
    const QSizeF premiumTextSize = senderFontMetrics.size(Qt::TextSingleLine, layout.premiumText);
    const int iconWidth = premiumTextSize.height() * 2;
    const int margin = MessageDelegateUtils::basicMargin();
    QRect usableRect = option.rect;
    const int topPos = usableRect.top() + margin;

    const auto pix = index.data(AppsMarketPlaceModel::Pixmap).value<QPixmap>();
    if (!pix.isNull()) {
        const QPixmap scaledPixmap = pix.scaled(iconWidth, iconWidth, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        layout.appPixmap = scaledPixmap;
        layout.appPixmapPos = QPointF(option.rect.x() + margin, topPos);
    }

    const int maxWidth = qMax(iconWidth, option.rect.width() - iconWidth - 2 * margin - static_cast<int>(premiumTextSize.width()));

    qreal baseLine = 0;

    auto *doc = documentForModelIndex(index, maxWidth);
    const QSize textSize = MessageDelegateUtils::textSizeHint(doc, &baseLine);

    layout.textRect = QRect(iconWidth + 2 * margin, topPos, maxWidth, qMax(textSize.height(), iconWidth + margin) /* + textVMargin*/);
    layout.premiumRect = QRectF(usableRect.right() - premiumTextSize.width() - 2 * margin, topPos, premiumTextSize.width(), premiumTextSize.height());

    // Status
    const AppsMarketPlaceInstalledInfo::Status status = index.data(AppsMarketPlaceModel::Status).value<AppsMarketPlaceInstalledInfo::Status>();
    if (status == AppsMarketPlaceInstalledInfo::Status::AutoEnabled || status == AppsMarketPlaceInstalledInfo::Status::ManuallyEnabled) {
        layout.status = Status::Enabled;
        layout.statusText = i18n("Enabled");
    } else if (status == AppsMarketPlaceInstalledInfo::Status::Initialized || status == AppsMarketPlaceInstalledInfo::Status::ManuallyDisabled
               || status == AppsMarketPlaceInstalledInfo::Status::Disabled) {
        layout.status = Status::Disabled;
        layout.statusText = i18n("Disabled");
    } else {
        layout.status = Status::Unknown;
    }
    QSizeF requestedTextSize;
    if (layout.status == Status::Unknown) {
        // Requested
        const int requested = index.data(AppsMarketPlaceModel::RequestedApps).toInt();
        layout.requested = requested > 0;
        layout.requestedText = layout.requested ? i18np("%1 request", "%1 requests", requested) : QString();
        requestedTextSize = senderFontMetrics.size(Qt::TextSingleLine, layout.requestedText);
        layout.requestedRect = QRectF(usableRect.right() - premiumTextSize.width() - requestedTextSize.width() - 2 * margin,
                                      topPos,
                                      requestedTextSize.width(),
                                      requestedTextSize.height());
    }
    const QSizeF statusTextSize = senderFontMetrics.size(Qt::TextSingleLine, layout.statusText);

    layout.statusRect =
        QRectF(usableRect.right() - requestedTextSize.width() - statusTextSize.width() - 2 * margin, topPos, statusTextSize.width(), statusTextSize.height());

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

    QString fullDescription = u"*%1*"_s.arg(appName);
    if (!shortDescription.isEmpty()) {
        fullDescription += u"\n*%2*"_s.arg(shortDescription);
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

QString ApplicationsSettingsDelegate::selectedText() const
{
    return mTextSelectionImpl->textSelection()->selectedText(TextSelection::Format::Text);
}

bool ApplicationsSettingsDelegate::hasSelection() const
{
    return mTextSelectionImpl->textSelection()->hasSelection();
}

#include "moc_applicationssettingsdelegate.cpp"
