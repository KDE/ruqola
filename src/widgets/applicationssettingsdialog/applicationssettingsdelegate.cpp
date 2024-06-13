/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsdelegate.h"
#include "config-ruqola.h"
#include "model/appsmarketplacemodel.h"
#if USE_SIZEHINT_CACHE_SUPPORT
#include "ruqola_sizehint_cache_debug.h"
#endif

ApplicationsSettingsDelegate::ApplicationsSettingsDelegate(QObject *parent)
    : QItemDelegate{parent}
{
}

ApplicationsSettingsDelegate::~ApplicationsSettingsDelegate() = default;

void ApplicationsSettingsDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
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
    // TODO
    return layout;
}

QByteArray ApplicationsSettingsDelegate::cacheIdentifier(const QModelIndex &index) const
{
    const QByteArray str = index.data(AppsMarketPlaceModel::AppId).toString().toLatin1();
    return str;
}
#include "moc_applicationssettingsdelegate.cpp"
