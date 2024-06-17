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
#include "textconverter.h"
#if USE_SIZEHINT_CACHE_SUPPORT
#include "ruqola_sizehint_cache_debug.h"
#endif
#include <QPainter>
#include <QTextDocument>
#include <QTreeView>

ApplicationsSettingsDelegate::ApplicationsSettingsDelegate(RocketChatAccount *account, QTreeView *view, QObject *parent)
    : MessageListDelegateBase{view, parent}
    , mRocketChatAccount(account)
{
}

ApplicationsSettingsDelegate::~ApplicationsSettingsDelegate() = default;

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

    // TODO reimplement it
    QItemDelegate::paint(painter, option, index);
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

    const QSize size = QItemDelegate::sizeHint(option, index) + QSize(0, 4 * option.widget->devicePixelRatioF());
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
    if (!pix.isNull()) {
        // TODO fix size
        const QPixmap scaledPixmap = pix.scaled(10, 10, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        layout.appPixmap = scaledPixmap;
    }
    layout.appShortDescription = index.data(AppsMarketPlaceModel::ShortDescription).toString();
    layout.appName = index.data(AppsMarketPlaceModel::AppName).toString();
    layout.premium = index.data(AppsMarketPlaceModel::IsEnterpriseOnly).toBool();

    QRect usableRect = option.rect;
    layout.textRect = usableRect;
    // layout.textRect = QRect(textLeft, usableRect.top(), maxWidth, textSize.height() + textVMargin);

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
    const QString description = index.data(AppsMarketPlaceModel::Description).toString();
    return documentForDelegate(mRocketChatAccount, appsId, description, width);
}

QTextDocument *
ApplicationsSettingsDelegate::documentForDelegate(RocketChatAccount *rcAccount, const QByteArray &messageId, const QString &messageStr, int width) const
{
    auto it = mDocumentCache.find(messageId);
    if (it != mDocumentCache.end()) {
        auto ret = it->value.get();
        if (width != -1 && !qFuzzyCompare(ret->textWidth(), width)) {
            ret->setTextWidth(width);
        }
        return ret;
    }
    if (messageStr.isEmpty()) {
        return nullptr;
    }
    // Use TextConverter in case it starts with a [](URL) reply marker
    QByteArray needUpdateMessageId; // TODO use it ?
    int maximumRecursiveQuotedText = -1;
    if (rcAccount) {
        maximumRecursiveQuotedText = rcAccount->ruqolaServerConfig()->messageQuoteChainLimit();
    }
    const TextConverter::ConvertMessageTextSettings settings(messageStr,
                                                             rcAccount ? rcAccount->userName() : QString(),
                                                             {},
                                                             rcAccount ? rcAccount->highlightWords() : QStringList(),
                                                             rcAccount ? rcAccount->emojiManager() : nullptr,
                                                             rcAccount ? rcAccount->messageCache() : nullptr,
                                                             {},
                                                             {},
                                                             {},
                                                             maximumRecursiveQuotedText);

    int recursiveIndex = 0;
    const QString contextString = TextConverter::convertMessageText(settings, needUpdateMessageId, recursiveIndex);
    auto doc = MessageDelegateUtils::createTextDocument(false, contextString, width);
    auto ret = doc.get();
    mDocumentCache.insert(messageId, std::move(doc));
    return ret;
}

RocketChatAccount *ApplicationsSettingsDelegate::rocketChatAccount(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return mRocketChatAccount;
}

#include "moc_applicationssettingsdelegate.cpp"
