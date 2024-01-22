/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "previewurlcachemanager.h"
#include "rocketchataccount.h"
#include <QTimer>
#include <chrono>
using namespace std::chrono_literals;

PreviewUrlCacheManager::PreviewUrlCacheManager(RocketChatAccount *account, QObject *parent)
    : QObject{parent}
    , mRocketChatAccount(account)
{
}

PreviewUrlCacheManager::~PreviewUrlCacheManager() = default;

int PreviewUrlCacheManager::embedCacheExpirationDays() const
{
    return mEmbedCacheExpirationDays;
}

void PreviewUrlCacheManager::setEmbedCacheExpirationDays(int newEmbedCacheExpirationDays)
{
    if (mEmbedCacheExpirationDays != newEmbedCacheExpirationDays) {
        mEmbedCacheExpirationDays = newEmbedCacheExpirationDays;
        checkCache();
    }
}

void PreviewUrlCacheManager::checkCache()
{
    // TODO When done reactivate QTimer!
    // Reactivate check each day
    QTimer::singleShot(24h, this, &PreviewUrlCacheManager::checkCache);
}

#include "moc_previewurlcachemanager.cpp"
