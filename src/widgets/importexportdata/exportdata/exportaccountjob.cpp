/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exportaccountjob.h"

#include "localdatabase/localdatabaseutils.h"
#include "ruqola_importexport_accounts_debug.h"
#include <KLocalizedString>
#include <KZip>
#include <QDir>
#include <QStandardPaths>
#include <QTemporaryFile>
#include <utility>

using namespace Qt::Literals::StringLiterals;
ExportAccountJob::ExportAccountJob(const QString &fileName, QObject *parent)
    : QThread{parent}
    , mArchive(new KZip(fileName))
{
    connect(this, &ExportAccountJob::finished, this, &QObject::deleteLater);
}

ExportAccountJob::~ExportAccountJob()
{
    if (isRunning()) {
        wait();
    }
    if (mArchive && mArchive->isOpen()) {
        mArchive->close();
    }
    delete mArchive;
}

void ExportAccountJob::run()
{
    if (!canStart()) {
        Q_EMIT exportFailed(i18n("Impossible to export data."));
        qCDebug(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << " Account list is empty! ";
        return;
    }
    const bool result = mArchive->open(QIODevice::WriteOnly);
    if (!result) {
        Q_EMIT exportFailed(i18n("Impossible to create zip file."));
        qCDebug(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << "Impossible to open zip file";
        return;
    }

    exportAccount();
}

void ExportAccountJob::exportAccount()
{
    for (const auto &account : std::as_const(mListAccounts)) {
        mAccountNames.append(account.accountName);
        exportAccount(account);
    }
    finishExportAccount();
}

void ExportAccountJob::finishExportAccount()
{
    QTemporaryFile tmp;
    if (!tmp.open()) {
        qCWarning(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << "Impossible to create temporary file";
        Q_EMIT exportFailed(i18n("Impossible to export account.") + u'\n');
        return;
    }
    QTextStream text(&tmp);
    text << mAccountNames.join(u'\n');
    tmp.close();
    mArchive->addLocalFile(tmp.fileName(), u"accounts"_s);

    Q_EMIT exportInfo(i18n("Export Done.") + u'\n');
    Q_EMIT exportDone();
}

QList<ImportExportUtils::AccountImportExportInfo> ExportAccountJob::listAccounts() const
{
    return mListAccounts;
}

void ExportAccountJob::exportAccount(const ImportExportUtils::AccountImportExportInfo &info)
{
    exportConfig(info);
}

void ExportAccountJob::exportConfig(const ImportExportUtils::AccountImportExportInfo &info)
{
    // config files
    const QString configPath = info.accountName + u'/' + ImportExportUtils::configPath();
    qCDebug(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << " configPath " << configPath;
    storeDirectory(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + u"/ruqola/"_s + info.accountName, configPath);
    Q_EMIT exportInfo(i18n("<b>Account %1:</b> export config done.", info.accountName));
    exportCache(info);
}

void ExportAccountJob::exportCache(const ImportExportUtils::AccountImportExportInfo &info)
{
    // cache files
    const QString cachePath = info.accountName + u'/' + ImportExportUtils::cachePath();
    qCDebug(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << " cachePath " << cachePath;
    const QString storeCachePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + u'/' + info.accountName + u'/';
    qCDebug(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << "QStandardPaths::writableLocation(QStandardPaths::CacheLocation) " << storeCachePath;
    storeDirectory(storeCachePath, cachePath);
    Q_EMIT exportInfo(i18n("<b>Account %1:</b> export cache done.", info.accountName));
    exportDatabase(info);
}

void ExportAccountJob::exportLogs(const ImportExportUtils::AccountImportExportInfo &info)
{
    // local files
    const QString localPath = info.accountName + u'/' + ImportExportUtils::logsPath();
    qCDebug(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << " localPath " << localPath;
    storeDirectory(LocalDatabaseUtils::localMessageLoggerPath() + info.accountName, localPath);
    Q_EMIT exportInfo(i18n("<b>Account %1:</b> export logs done.", info.accountName));
}

void ExportAccountJob::exportDatabase(const ImportExportUtils::AccountImportExportInfo &info)
{
    const QString localPath = info.accountName + u'/' + ImportExportUtils::databasePath();
    qCDebug(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << " localPath " << localPath;
    storeDirectory(LocalDatabaseUtils::localMessagesDatabasePath() + info.accountName,
                   localPath + u'/' + LocalDatabaseUtils::databasePath(LocalDatabaseUtils::DatabasePath::Messages));
    storeDirectory(LocalDatabaseUtils::localRoomsDatabasePath() + info.accountName,
                   localPath + u'/' + LocalDatabaseUtils::databasePath(LocalDatabaseUtils::DatabasePath::Rooms));
    storeDirectory(LocalDatabaseUtils::localAccountsDatabasePath() + info.accountName,
                   localPath + u'/' + LocalDatabaseUtils::databasePath(LocalDatabaseUtils::DatabasePath::Accounts));
    storeDirectory(LocalDatabaseUtils::localGlobalDatabasePath() + info.accountName,
                   localPath + u'/' + LocalDatabaseUtils::databasePath(LocalDatabaseUtils::DatabasePath::Global));
    storeDirectory(LocalDatabaseUtils::localE2EDatabasePath() + info.accountName,
                   localPath + u'/' + LocalDatabaseUtils::databasePath(LocalDatabaseUtils::DatabasePath::E2E));
    storeDirectory(LocalDatabaseUtils::localE2ERoomsDatabasePath() + info.accountName,
                   localPath + u'/' + LocalDatabaseUtils::databasePath(LocalDatabaseUtils::DatabasePath::E2ERooms));
    storeDirectory(LocalDatabaseUtils::localRoomPendingTypedInfoDatabasePath() + info.accountName,
                   localPath + u'/' + LocalDatabaseUtils::databasePath(LocalDatabaseUtils::DatabasePath::RoomPendingTypedInfo));
    storeDirectory(LocalDatabaseUtils::localRoomSubscriptionsDatabasePath() + info.accountName,
                   localPath + u'/' + LocalDatabaseUtils::databasePath(LocalDatabaseUtils::DatabasePath::RoomSubscriptions));

    Q_EMIT exportInfo(i18n("<b>Account %1:</b> export database done.", info.accountName));
    exportLogs(info);
}

void ExportAccountJob::setListAccounts(const QList<ImportExportUtils::AccountImportExportInfo> &newListAccounts)
{
    mListAccounts = newListAccounts;
}

bool ExportAccountJob::canStart() const
{
    return !mListAccounts.isEmpty();
}

void ExportAccountJob::storeDirectory(const QString &subDirectory, const QString &subfolderPath)
{
    const QDir directoryToStore(subDirectory);
    qCDebug(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << " directoryToStore " << directoryToStore;
    if (directoryToStore.exists()) {
        const bool addFolder = mArchive->addLocalDirectory(directoryToStore.path(), subfolderPath);
        if (!addFolder) {
            Q_EMIT exportFailed(i18n("Directory \"%1\" cannot be added to backup file.", directoryToStore.path()));
        }
    } else {
        qCWarning(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << " directoryToStore " << directoryToStore << "doesn't exit";
    }
}

#include "moc_exportaccountjob.cpp"
