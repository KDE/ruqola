/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "previewurlcachemanager.h"
#include "rocketchataccount.h"
#include "rocketchataccountsettings.h"
#include "ruqola_previewurlcache_debug.h"
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
        return mRocketChatAccount->settings()->lastCheckedPreviewUrlCacheDate() != mCurrentDate;
    }
    return true;
}

void PreviewUrlCacheManager::saveLastCheckedDateTime()
{
    if (mRocketChatAccount) {
        mRocketChatAccount->settings()->setLastCheckedPreviewUrlCacheDate(mCurrentDate);
    }
}

void PreviewUrlCacheManager::checkCache()
{
    if (needToCheck()) {
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
            if (info.lastModified().date().addDays(mEmbedCacheExpirationDays) < mCurrentDate) {
                const QString filePath{info.filePath()};
                if (!QFile::remove(filePath)) {
                    qCWarning(RUQOLA_PREVIEWURLCACHE_LOG) << "Impossible to remove " << filePath;
                }
            }
        }
        saveLastCheckedDateTime();
    }

    // Reactivate check each day
    QTimer::singleShot(24h, this, &PreviewUrlCacheManager::checkCache);
}

QDate PreviewUrlCacheManager::currentDate() const
{
    return mCurrentDate;
}

void PreviewUrlCacheManager::setCurrentDate(const QDate &newCurrentDateTime)
{
    mCurrentDate = newCurrentDateTime;
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
