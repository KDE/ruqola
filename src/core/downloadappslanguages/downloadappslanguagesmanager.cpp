/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "downloadappslanguagesmanager.h"
#include "downloadappslanguagesjob.h"
#include "downloadappslanguagesparser.h"
#include "managerdatapaths.h"
#include "ruqola_debug.h"
#include <QFileInfo>
#include <QStandardPaths>
#include <QTextStream>

DownloadAppsLanguagesManager::DownloadAppsLanguagesManager(QObject *parent)
    : QObject(parent)
{
}

DownloadAppsLanguagesManager::~DownloadAppsLanguagesManager() = default;

QString DownloadAppsLanguagesManager::storedFileName() const
{
    return ManagerDataPaths::self()->path(ManagerDataPaths::Config, mAccountName) + QStringLiteral("/languages-%1.json").arg(mServerVersion);
}

void DownloadAppsLanguagesManager::parse(const QString &serverUrl)
{
    if (mFileParsed) {
        Q_EMIT fileLanguagesParseSuccess();
        return;
    }
    if (mAccountName.isEmpty()) {
        qCWarning(RUQOLA_LOG) << "account name is empty. It's a bug";
        Q_EMIT fileLanguagesParseFailed(); // Need to load commandlist
        return;
    }
    if (mServerVersion.isEmpty()) {
        qCWarning(RUQOLA_LOG) << "serverVersion is empty. It's a bug";
        Q_EMIT fileLanguagesParseFailed(); // Need to load commandlist
        return;
    }
    if (QFileInfo::exists(storedFileName())) {
        parseLanguageFile();
    } else {
        auto job = new DownloadAppsLanguagesJob(this);
        job->setServerUrl(serverUrl);
        connect(job, &DownloadAppsLanguagesJob::fileDownloaded, this, &DownloadAppsLanguagesManager::slotFileDownloaded);
        job->start();
    }
}

void DownloadAppsLanguagesManager::slotFileDownloaded(const QByteArray &data)
{
    QFile f(storedFileName());
    if (f.open(QIODevice::WriteOnly)) {
        QTextStream out(&f);
        out << data;
        f.close();
    }
    parseLanguageFile();
}

void DownloadAppsLanguagesManager::parseLanguageFile()
{
    DownloadAppsLanguagesParser parser;
    parser.setFilename(storedFileName());
    if (parser.parse()) {
        mLanguageMap = parser.map();
        mFileParsed = true;
        Q_EMIT fileLanguagesParseSuccess();
    } else {
        Q_EMIT fileLanguagesParseFailed();
    }
}

QString DownloadAppsLanguagesManager::serverVersion() const
{
    return mServerVersion;
}

void DownloadAppsLanguagesManager::setServerVersion(const QString &serverVersion)
{
    mServerVersion = serverVersion;
}

QString DownloadAppsLanguagesManager::accountName() const
{
    return mAccountName;
}

void DownloadAppsLanguagesManager::setAccountName(const QString &accountName)
{
    mAccountName = accountName;
}

bool DownloadAppsLanguagesManager::fileParsed() const
{
    return mFileParsed;
}

QString DownloadAppsLanguagesManager::translatedString(const QString &language, const QString &id)
{
    if (!mFileParsed) {
        qCWarning(RUQOLA_LOG) << "language file is not parsed yet!";
        return {};
    }
    QMap<QString, DownloadAppsLanguagesInfo>::const_iterator i = mLanguageMap.constBegin();
    while (i != mLanguageMap.constEnd()) {
        if (id.contains(i.key())) {
            const DownloadAppsLanguagesInfo info = i.value();
            const QMap<QString, QMap<QString, QString>> mapLangId = info.languageMap();
            QMap<QString, QString> mapId = mapLangId.value(language);
            if (mapId.isEmpty()) {
                mapId = mapLangId.value(QStringLiteral("en"));
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
