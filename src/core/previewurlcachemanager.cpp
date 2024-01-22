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
#if 0 // We can't use it as when a check was done for an account we can't do it for another
    qDebug() << " bool PreviewUrlCacheManager::needToCheck() const ";
    KSharedConfig::Ptr config = KSharedConfig::openConfig();
    const KConfigGroup group(config, QStringLiteral("PreviewUrlCache"));
    const QDate lastCheckedCacheDate = group.readEntry("Last Checked", QDate());
    return lastCheckedCacheDate != QDateTime::currentDateTime().date();
#else
    return true;
#endif
}

void PreviewUrlCacheManager::saveLastCheckedDateTime()
{
#if 0
    KSharedConfig::Ptr config = KSharedConfig::openConfig();
    KConfigGroup group(config, QStringLiteral("PreviewUrlCache"));
    group.writeEntry("Last Checked", QDateTime::currentDateTime().date());
    group.sync();
#endif
}

void PreviewUrlCacheManager::checkCache()
{
    if (needToCheck()) {
        const QDateTime currentDateTime = QDateTime::currentDateTime();
        const QString cachePath = ManagerDataPaths::self()->path(ManagerDataPaths::PreviewUrl, mRocketChatAccount->accountName());
        QDir dir(cachePath);
        const QFileInfoList infoLists = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);
        // qDebug() << " cachePath " << cachePath;
        // qDebug() << " infoLists " << infoLists.count();
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
