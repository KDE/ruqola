/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "importaccountjob.h"
#include "importexportdata/importexportutils.h"
#include "ruqola_importexport_accounts_debug.h"
#include <KLocalizedString>
#include <KZip>
#include <QDir>
#include <QStandardPaths>
#include <QTemporaryDir>
#include <QTemporaryFile>

ImportAccountJob::ImportAccountJob(const QString &fileName, QObject *parent)
    : QObject{parent}
    , mArchive(new KZip(fileName))
{
}

ImportAccountJob::~ImportAccountJob()
{
    if (mArchive && mArchive->isOpen()) {
        mArchive->close();
    }
    delete mArchive;
}

void ImportAccountJob::start()
{
    const bool result = mArchive->open(QIODevice::ReadOnly);
    if (!result) {
        deleteLater();
        Q_EMIT importFailed(i18n("Impossible to open zip file."));
        qCDebug(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << "Impossible to open zip file";
        return;
    }

    const KArchiveDirectory *zipDir = mArchive->directory();
    const KArchiveEntry *accountsEntry = zipDir->entry(QStringLiteral("accounts"));
    QStringList accountInfos;
    if (accountsEntry && accountsEntry->isFile()) {
        const auto accountsFile = static_cast<const KArchiveFile *>(accountsEntry);
        QTemporaryDir accountFileTmp;
        if (!accountsFile->copyTo(accountFileTmp.path())) {
            qCWarning(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << " Impossible to copy to " << accountFileTmp.path();
            Q_EMIT importFailed(i18n("Impossible to copy file"));
            deleteLater();
            return;
        }
        // qDebug() << " accountFileTmp->fileName()" << accountFileTmp.path();
        QFile file(accountFileTmp.path() + QStringLiteral("/accounts"));
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qCWarning(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << "Impossible to open file";
            Q_EMIT importFailed(i18n("Impossible to open file"));
            deleteLater();
            return;
        }

        QTextStream in(&file);
        while (!in.atEnd()) {
            const QString line = in.readLine();
            accountInfos.append(line);
        }
        qCDebug(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << " list of accounts " << accountInfos;
    }
    for (const auto &accountName : std::as_const(accountInfos)) {
        importAccount(accountName);
    }
    Q_EMIT importDone();
    deleteLater();
}

void ImportAccountJob::importAccount(QString accountName)
{
    const QString oldAccountName = accountName;
    accountName = verifyExistingAccount(accountName);
    qCDebug(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << "accountName " << accountName;
    {
        // config files
        const QString configPath = oldAccountName + QLatin1Char('/') + ImportExportUtils::configPath();
        const KArchiveEntry *configPathEntry = mArchive->directory()->entry(configPath);
        if (configPathEntry && configPathEntry->isDirectory()) {
            const auto configDirectory = static_cast<const KArchiveDirectory *>(configPathEntry);
            const QStringList lst = configDirectory->entries();
            const QString newConfigPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QStringLiteral("/ruqola/") + accountName;
            if (!QDir().mkpath(newConfigPath)) {
                qCWarning(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << "Impossible to create directory " << newConfigPath;
            }
            for (const QString &file : lst) {
                const KArchiveEntry *filePathEntry = mArchive->directory()->entry(configPath + QStringLiteral("/%1").arg(file));
                if (filePathEntry && filePathEntry->isFile()) {
                    const auto filePath = static_cast<const KArchiveFile *>(filePathEntry);
                    if (!filePath->copyTo(newConfigPath)) {
                        qCWarning(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << "Impossible to copy to ";
                    }
                } else {
                    qCWarning(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << "Impossible to import file ";
                }
            }
        }
    }
    {
        // local files
        const QString localPath = oldAccountName + QStringLiteral("/logs/");
        const KArchiveEntry *localPathEntry = mArchive->directory()->entry(localPath);
        if (localPathEntry && localPathEntry->isDirectory()) {
            const auto localDirectory = static_cast<const KArchiveDirectory *>(localPathEntry);
            const QStringList lst = localDirectory->entries();
            const QString newLocalPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QStringLiteral("/logs/") + accountName;
            if (!QDir().mkpath(newLocalPath)) {
                qCWarning(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << "Impossible to create directory " << newLocalPath;
            }
            for (const QString &file : lst) {
                const KArchiveEntry *filePathEntry = mArchive->directory()->entry(localPath + QStringLiteral("/%1").arg(file));
                if (filePathEntry) {
                    if (filePathEntry->isFile()) {
                        const auto filePath = static_cast<const KArchiveFile *>(filePathEntry);
                        if (!filePath->copyTo(newLocalPath)) {
                            qCWarning(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << "Impossible to copy logs file ";
                        }
                    } else if (filePathEntry->isDirectory()) {
                        const auto filePath = static_cast<const KArchiveDirectory *>(filePathEntry);
                        if (!filePath->copyTo(newLocalPath)) {
                            qCWarning(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << "Impossible to copy logs directory ";
                        }
                    } else {
                        qCWarning(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << "Impossible to import file ";
                    }
                }
            }
        }
    }
    {
        const QString cachePath = oldAccountName + QStringLiteral("/cache/");
        const KArchiveEntry *cachePathEntry = mArchive->directory()->entry(cachePath);
        if (cachePathEntry && cachePathEntry->isDirectory()) {
            const auto cacheDirectory = static_cast<const KArchiveDirectory *>(cachePathEntry);
            const QStringList lst = cacheDirectory->entries();
            const QString newCachePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + QLatin1Char('/') + accountName;
            if (!QDir().mkpath(newCachePath)) {
                qCWarning(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << "Impossible to create directory " << newCachePath;
            }
            for (const QString &file : lst) {
                const KArchiveEntry *filePathEntry = mArchive->directory()->entry(cachePath + QStringLiteral("/%1").arg(file));
                if (filePathEntry->isDirectory()) {
                    const auto filePath = static_cast<const KArchiveDirectory *>(filePathEntry);
                    if (!filePath->copyTo(newCachePath + QStringLiteral("/%1").arg(file))) {
                        qCWarning(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << "Impossible to copy logs directory ";
                    }
                } else {
                    qCWarning(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) <<" Missing import file ? " << lst;
                }
            }
        }
    }
}

void ImportAccountJob::copyToDirectory(const KArchiveDirectory *subfolderDir, const QString &dest)
{
    if (!subfolderDir->copyTo(dest)) {
        qCDebug(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << "directory cannot copy to " << dest;
    }
}

QString ImportAccountJob::verifyExistingAccount(QString accountName) const
{
    int i = 1;
    QString orginalAccountName = accountName;
    while (QDir(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QStringLiteral("/ruqola/") + accountName).exists()) {
        accountName = QStringLiteral("%1_%2").arg(orginalAccountName).arg(QString::number(i));
        ++i;
    }
    return accountName;
}

#include "moc_importaccountjob.cpp"
