/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "importaccountjob.h"

#include "importexportdata/importexportutils.h"
#include "localdatabase/localdatabaseutils.h"
#include "ruqola_importexport_accounts_debug.h"
#include <KLocalizedString>
#include <KZip>
#include <QDir>
#include <QStandardPaths>
#include <QTemporaryDir>
#include <QTimer>

using namespace Qt::Literals::StringLiterals;
ImportAccountJob::ImportAccountJob(const QString &fileName, QObject *parent)
    : QThread{parent}
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

void ImportAccountJob::run()
{
    const bool result = mArchive->open(QIODevice::ReadOnly);
    if (!result) {
        deleteLater();
        Q_EMIT importFailed(i18n("Impossible to open zip file."));
        qCDebug(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << "Impossible to open zip file";
        return;
    }

    const KArchiveDirectory *zipDir = mArchive->directory();
    const KArchiveEntry *accountsEntry = zipDir->entry(u"accounts"_s);
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
        QFile file(accountFileTmp.path() + u"/accounts"_s);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qCWarning(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << "Impossible to open file";
            Q_EMIT importFailed(i18n("Impossible to open file"));
            deleteLater();
            return;
        }

        QTextStream in(&file);
        while (!in.atEnd()) {
            const QString line = in.readLine();
            mAccountInfos.append(line);
        }
        qCDebug(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << " list of accounts " << mAccountInfos;
    }
    QTimer::singleShot(0, this, &ImportAccountJob::importAccounts);
}

void ImportAccountJob::importAccounts()
{
    if (mAccountIndex < mAccountInfos.count()) {
        const auto account = mAccountInfos.at(mAccountIndex);
        importAccount(account);
    } else {
        finishImportAccounts();
    }
}

void ImportAccountJob::finishImportAccounts()
{
    Q_EMIT importDone();
    Q_EMIT importInfo(i18n("Import Done.") + u'\n');
    deleteLater();
}

void ImportAccountJob::importAccount(QString accountName)
{
    const QString oldAccountName = accountName;
    accountName = verifyExistingAccount(accountName);
    qCDebug(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << "accountName " << accountName;
    {
        // config files
        const QString configPath = oldAccountName + u'/' + ImportExportUtils::configPath();
        const KArchiveEntry *configPathEntry = mArchive->directory()->entry(configPath);
        if (configPathEntry && configPathEntry->isDirectory()) {
            const auto configDirectory = static_cast<const KArchiveDirectory *>(configPathEntry);
            const QStringList lst = configDirectory->entries();
            const QString newConfigPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + u"/ruqola/"_s + accountName;
            if (!QDir().mkpath(newConfigPath)) {
                qCWarning(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << "Impossible to create directory " << newConfigPath;
            }
            for (const QString &file : lst) {
                const KArchiveEntry *filePathEntry = mArchive->directory()->entry(configPath + u"/%1"_s.arg(file));
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
        const QString localPath = oldAccountName + u'/' + ImportExportUtils::logsPath();
        const KArchiveEntry *localPathEntry = mArchive->directory()->entry(localPath);
        if (localPathEntry && localPathEntry->isDirectory()) {
            const auto localDirectory = static_cast<const KArchiveDirectory *>(localPathEntry);
            const QStringList lst = localDirectory->entries();
            const QString newLocalPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + u"/logs/"_s + accountName;
            if (!QDir().mkpath(newLocalPath)) {
                qCWarning(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << "Impossible to create directory " << newLocalPath;
            }
            for (const QString &file : lst) {
                const KArchiveEntry *filePathEntry = mArchive->directory()->entry(localPath + u"/%1"_s.arg(file));
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
        const QString cachePath = oldAccountName + u'/' + ImportExportUtils::cachePath();
        const KArchiveEntry *cachePathEntry = mArchive->directory()->entry(cachePath);
        if (cachePathEntry && cachePathEntry->isDirectory()) {
            const auto cacheDirectory = static_cast<const KArchiveDirectory *>(cachePathEntry);
            const QStringList lst = cacheDirectory->entries();
            const QString newCachePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + u'/' + accountName;
            if (!QDir().mkpath(newCachePath)) {
                qCWarning(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << "Impossible to create directory " << newCachePath;
            }
            for (const QString &file : lst) {
                const KArchiveEntry *filePathEntry = mArchive->directory()->entry(cachePath + u"/%1"_s.arg(file));
                if (filePathEntry->isDirectory()) {
                    const auto filePath = static_cast<const KArchiveDirectory *>(filePathEntry);
                    if (!filePath->copyTo(newCachePath + u"/%1"_s.arg(file))) {
                        qCWarning(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << "Impossible to copy logs directory ";
                    }
                } else {
                    qCWarning(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << " Missing import file ? " << lst;
                }
            }
        }
    }
    {
        const QString databasePath = oldAccountName + u'/' + ImportExportUtils::databasePath();
        const KArchiveEntry *databasePathEntry = mArchive->directory()->entry(databasePath);
        if (databasePathEntry && databasePathEntry->isDirectory()) {
            const auto databaseDirectory = static_cast<const KArchiveDirectory *>(databasePathEntry);
            copyDatabase(databaseDirectory,
                         accountName,
                         databasePath,
                         LocalDatabaseUtils::databasePath(LocalDatabaseUtils::DatabasePath::Messages),
                         LocalDatabaseUtils::localMessagesDatabasePath(),
                         false);
            copyDatabase(databaseDirectory,
                         accountName,
                         databasePath,
                         LocalDatabaseUtils::databasePath(LocalDatabaseUtils::DatabasePath::Rooms),
                         LocalDatabaseUtils::localRoomsDatabasePath(),
                         true);
            copyDatabase(databaseDirectory,
                         accountName,
                         databasePath,
                         LocalDatabaseUtils::databasePath(LocalDatabaseUtils::DatabasePath::RoomPendingTypedInfo),
                         LocalDatabaseUtils::localRoomPendingTypedInfoDatabasePath(),
                         true);
            copyDatabase(databaseDirectory,
                         accountName,
                         databasePath,
                         LocalDatabaseUtils::databasePath(LocalDatabaseUtils::DatabasePath::RoomSubscriptions),
                         LocalDatabaseUtils::localRoomSubscriptionsDatabasePath(),
                         true);
            copyDatabase(databaseDirectory,
                         accountName,
                         databasePath,
                         LocalDatabaseUtils::databasePath(LocalDatabaseUtils::DatabasePath::Global),
                         LocalDatabaseUtils::localGlobalDatabasePath(),
                         true);
            copyDatabase(databaseDirectory,
                         accountName,
                         databasePath,
                         LocalDatabaseUtils::databasePath(LocalDatabaseUtils::DatabasePath::Accounts),
                         LocalDatabaseUtils::localAccountsDatabasePath(),
                         true);
            copyDatabase(databaseDirectory,
                         accountName,
                         databasePath,
                         LocalDatabaseUtils::databasePath(LocalDatabaseUtils::DatabasePath::E2E),
                         LocalDatabaseUtils::localE2EDatabasePath(),
                         true); // TODO verify this one in the future as not implemented yet
        }
    }

    mAccountIndex++;
    QTimer::singleShot(0, this, &ImportAccountJob::importAccounts);
}

void ImportAccountJob::copyDatabase(const KArchiveDirectory *databaseDirectory,
                                    const QString &accountName,
                                    const QString &databasePath,
                                    const QString &subfolder,
                                    const QString &dest,
                                    bool renameFiles)
{
    // TODO rename file
    auto messageDirectory = databaseDirectory->entry(subfolder);
    if (messageDirectory && messageDirectory->isDirectory()) {
        const auto directory = static_cast<const KArchiveDirectory *>(messageDirectory);
        const QStringList messageList = directory->entries();
        const QString newCachePath = dest + u'/' + accountName;
        if (!QDir().mkpath(newCachePath)) {
            qCWarning(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << "Impossible to create directory " << newCachePath;
        }
        for (const QString &file : messageList) {
            const KArchiveEntry *filePathEntry = mArchive->directory()->entry(databasePath + subfolder + u"/%1"_s.arg(file));
            if (filePathEntry->isDirectory()) {
                const auto filePath = static_cast<const KArchiveDirectory *>(filePathEntry);
                if (!filePath->copyTo(newCachePath + u"/%1"_s.arg(file))) {
                    qCWarning(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << "Impossible to copy logs directory ";
                }
            } else {
                qCWarning(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << " Missing import file ? " << messageList;
            }
        }
    }
}

QString ImportAccountJob::verifyExistingAccount(QString accountName) const
{
    int i = 1;
    QString orginalAccountName = accountName;
    while (QDir(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + u"/ruqola/"_s + accountName).exists()) {
        accountName = u"%1_%2"_s.arg(orginalAccountName, QString::number(i));
        ++i;
    }
    return accountName;
}

#include "moc_importaccountjob.cpp"
