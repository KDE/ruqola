/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsdelegate.h"
#include "common/delegatepaintutil.h"
#include "config-ruqola.h"
#include "delegateutils/messagedelegateutils.h"
#include "model/appsmarketplacemodel.h"
#include "rocketchataccount.h"
#include "textconverter.h"
#if USE_SIZEHINT_CACHE_SUPPORT
#include "ruqola_sizehint_cache_debug.h"
#endif
#include <QPainter>
#include <QTextDocument>

ApplicationsSettingsDelegate::ApplicationsSettingsDelegate(QObject *parent)
    : QItemDelegate{parent}
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
        // TODO DelegatePaintUtil::createClipRoundedRectangle(painter, QRectF(layout.avatarPos, layout.appPixmap.size()), layout.avatarPos, layout.appPixmap);
#else
        // TODO painter->drawPixmap(layout.avatarPos, layout.avatarPixmap);
#endif
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
    // return documentForDelegate(mRocketChatAccount, appsId, description, width);
    return {};
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

#include "moc_applicationssettingsdelegate.cpp"
