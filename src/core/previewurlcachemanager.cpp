/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "previewurlcachemanager.h"
#include "managerdatapaths.h"
#include "rocketchataccount.h"
#include "ruqola_previewurlcache_debug.h"
#include <KConfigGroup>
#include <KSharedConfig>
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

bool PreviewUrlCacheManager::needToCheck() const
{
    if (mRocketChatAccount) {
        return mRocketChatAccount->settings()->lastCheckedPreviewUrlCacheDate() != QDateTime::currentDateTime().date();
    }
    return true;
}

void PreviewUrlCacheManager::saveLastCheckedDateTime()
{
    if (mRocketChatAccount) {
        mRocketChatAccount->settings()->setLastCheckedPreviewUrlCacheDate(QDateTime::currentDateTime().date());
    }
}

void PreviewUrlCacheManager::checkCache()
{
    if (needToCheck()) {
        const QDateTime currentDateTime = QDateTime::currentDateTime();
        if (mCachePath.isEmpty()) {
            qCWarning(RUQOLA_PREVIEWURLCACHE_LOG) << "mCachePath is empty it's a bug!!! ";
            return;
        }
        QDir dir(mCachePath);
        const QFileInfoList infoLists = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);
        // qDebug() << " cachePath " << mCachePath;
        // qDebug() << " infoLists-- " << infoLists.count() << infoLists;
        for (const QFileInfo &info : infoLists) {
            // qDebug() << " info " << info << "  info.birthTime() " << info.birthTime();
            if (info.birthTime().addDays(mEmbedCacheExpirationDays) < currentDateTime) {
                if (!QFile::remove(info.path())) {
                    qCWarning(RUQOLA_PREVIEWURLCACHE_LOG) << "Impossible to remove " << info.path();
                }
            }
        }
        saveLastCheckedDateTime();
    }

    // Reactivate check each day
    QTimer::singleShot(24h, this, &PreviewUrlCacheManager::checkCache);
}

QString PreviewUrlCacheManager::cachePath() const
{
    return mCachePath;
}

void PreviewUrlCacheManager::setCachePath(const QString &newCachePath)
{
    mCachePath = newCachePath;
}

#include "moc_previewurlcachemanager.cpp"
