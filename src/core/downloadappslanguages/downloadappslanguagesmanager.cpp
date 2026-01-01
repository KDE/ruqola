/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "downloadappslanguagesmanager.h"
#include "apps/appinfojob.h"
#include "connection.h"
#include "downloadappslanguagesparser.h"
#include "rocketchataccount.h"
#include "ruqola_debug.h"
using namespace Qt::Literals::StringLiterals;
DownloadAppsLanguagesManager::DownloadAppsLanguagesManager(RocketChatAccount *account, QObject *parent)
    : QObject(parent)
    , mRocketChatAccount(account)
{
}

DownloadAppsLanguagesManager::~DownloadAppsLanguagesManager() = default;

void DownloadAppsLanguagesManager::parse()
{
    if (mRocketChatAccount) {
        auto job = new RocketChatRestApi::AppInfoJob(this);
        job->setAppInfoType(RocketChatRestApi::AppInfoJob::AppInfoType::Languages);
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::AppInfoJob::appInfoDone, this, &DownloadAppsLanguagesManager::slotFileDownloaded);
        if (!job->start()) {
            qCWarning(RUQOLA_LOG) << "Impossible to start AppInfoJob";
            Q_EMIT fileLanguagesParseFailed();
        }
    }
}

void DownloadAppsLanguagesManager::slotFileDownloaded(const QJsonObject &json)
{
    DownloadAppsLanguagesParser parser;
    mLanguageMap = parser.parse(json);
    Q_EMIT fileLanguagesParseSuccess();
}

QMap<QString, DownloadAppsLanguagesInfo> DownloadAppsLanguagesManager::languagesAppsMap() const
{
    return mLanguageMap;
}

QString DownloadAppsLanguagesManager::translatedString(const QString &language, const QString &id)
{
    QMap<QString, DownloadAppsLanguagesInfo>::const_iterator i = mLanguageMap.constBegin();
    while (i != mLanguageMap.constEnd()) {
        if (id.contains(i.key())) {
            const DownloadAppsLanguagesInfo info = i.value();
            const QMap<QString, QMap<QString, QString>> mapLangId = info.languageMap();
            QMap<QString, QString> mapId = mapLangId.value(language);
            if (mapId.isEmpty()) {
                mapId = mapLangId.value("en"_L1);
                if (mapId.isEmpty()) {
                    return {};
                }
            }
            return mapId.value(id);
        }
        ++i;
    }
    return {};
}

#include "moc_downloadappslanguagesmanager.cpp"
