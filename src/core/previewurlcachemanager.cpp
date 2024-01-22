/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "previewurlcachemanager.h"
#include "managerdatapaths.h"
#include "rocketchataccount.h"
#include <QDir>
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
    const QDateTime currentDateTime = QDateTime::currentDateTime();
    const QString cachePath = ManagerDataPaths::self()->path(ManagerDataPaths::PreviewUrl, mRocketChatAccount->accountName());
    QDir dir(cachePath);
    const QFileInfoList infoLists = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);
    for (const QFileInfo &info : infoLists) {
        if (info.birthTime().addDays(mEmbedCacheExpirationDays) < currentDateTime) {
            // TODO remove it => redownload it.
            // TODO store info when we check cache => don't call it each time that we relaunch ruqola in same day.
        }
    }
    // TODO When done reactivate QTimer!
    // Reactivate check each day
    QTimer::singleShot(24h, this, &PreviewUrlCacheManager::checkCache);
}

#include "moc_previewurlcachemanager.cpp"
