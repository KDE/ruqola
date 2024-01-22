/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "previewurlcachemanager.h"
#include "managerdatapaths.h"
#include "rocketchataccount.h"
#include "ruqola_debug.h"
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
    return mRocketChatAccount->settings()->lastCheckedPreviewUrlCacheDate() != QDateTime::currentDateTime().date();
}

void PreviewUrlCacheManager::saveLastCheckedDateTime()
{
    mRocketChatAccount->settings()->setLastCheckedPreviewUrlCacheDate(QDateTime::currentDateTime().date());
}

void PreviewUrlCacheManager::checkCache()
{
    if (needToCheck()) {
        const QDateTime currentDateTime = QDateTime::currentDateTime();
        const QString cachePath = ManagerDataPaths::self()->path(ManagerDataPaths::PreviewUrl, mRocketChatAccount->accountName());
        QDir dir(cachePath);
        const QFileInfoList infoLists = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);
        qDebug() << " cachePath " << cachePath;
        qDebug() << " infoLists " << infoLists.count();
        for (const QFileInfo &info : infoLists) {
            // qDebug() << " info" << info;
            if (info.birthTime().addDays(mEmbedCacheExpirationDays) < currentDateTime) {
                if (!QFile::remove(info.path())) {
                    qCWarning(RUQOLA_LOG) << "Impossible to remove " << info.path();
                }
            }
        }
        saveLastCheckedDateTime();
    }

    // Reactivate check each day
    QTimer::singleShot(24h, this, &PreviewUrlCacheManager::checkCache);
}

#include "moc_previewurlcachemanager.cpp"
