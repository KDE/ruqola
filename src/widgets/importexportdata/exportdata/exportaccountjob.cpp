/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exportaccountjob.h"
#include "ruqola_importexport_accounts_debug.h"
#include <KLocalizedString>
#include <KZip>
#include <QDir>
#include <QStandardPaths>
#include <QTemporaryFile>

ExportAccountJob::ExportAccountJob(const QString &fileName, QObject *parent)
    : QThread{parent}
    , mArchive(new KZip(fileName))
{
    connect(this, &ExportAccountJob::exportCacheData, this, &ExportAccountJob::exportCache);
    connect(this, &ExportAccountJob::exportLogsData, this, &ExportAccountJob::exportLogs);
}

ExportAccountJob::~ExportAccountJob()
{
    if (mArchive && mArchive->isOpen()) {
        mArchive->close();
    }
    delete mArchive;
}

void ExportAccountJob::run()
{
    if (!canStart()) {
        deleteLater();
        Q_EMIT exportFailed(i18n("Impossible to export data."));
        qCDebug(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << " Account list is empty! ";
        return;
    }
    const bool result = mArchive->open(QIODevice::WriteOnly);
    if (!result) {
        deleteLater();
        Q_EMIT exportFailed(i18n("Impossible to create zip file."));
        qCDebug(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << "Impossible to open zip file";
        return;
    }

    exportAccount();
}

void ExportAccountJob::exportAccount()
{
    if (mAccountIndex < mListAccounts.count()) {
        const auto account = mListAccounts.at(mAccountIndex);
        mAccountNames.append(account.accountName);
        exportAccount(account);
    } else {
        finishExportAccount();
    }
}

void ExportAccountJob::finishExportAccount()
{
    QTemporaryFile tmp;
    tmp.open();
    QTextStream text(&tmp);
    text << mAccountNames.join(QLatin1Char('\n'));
    tmp.close();
    mArchive->addLocalFile(tmp.fileName(), QStringLiteral("accounts"));

    Q_EMIT exportInfo(i18n("Export Done.") + QLatin1Char('\n'));
    Q_EMIT exportDone();
    deleteLater();
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
    const QString configPath = info.accountName + QLatin1Char('/') + ImportExportUtils::configPath();
    qCDebug(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << " configPath " << configPath;
    storeDirectory(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QStringLiteral("/ruqola/") + info.accountName, configPath);
    Q_EMIT exportInfo(i18n("Account %1: export config done.", info.accountName));
    Q_EMIT exportCacheData(info);
}

void ExportAccountJob::exportCache(const ImportExportUtils::AccountImportExportInfo &info)
{
    // cache files
    const QString cachePath = info.accountName + QLatin1Char('/') + ImportExportUtils::cachePath();
    qCDebug(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << " cachePath " << cachePath;
    const QString storeCachePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + QLatin1Char('/') + info.accountName + QLatin1Char('/');
    qCDebug(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << "QStandardPaths::writableLocation(QStandardPaths::CacheLocation) " << storeCachePath;
    storeDirectory(storeCachePath, cachePath);
    Q_EMIT exportInfo(i18n("Account %1: export cache done.", info.accountName));
    Q_EMIT exportLogsData(info);
}

void ExportAccountJob::exportLogs(const ImportExportUtils::AccountImportExportInfo &info)
{
    // local files
    const QString localPath = info.accountName + QLatin1Char('/') + ImportExportUtils::logsPath();
    qCDebug(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << " localPath " << localPath;
    storeDirectory(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QStringLiteral("/logs/") + info.accountName, localPath);
    Q_EMIT exportInfo(i18n("Account %1: export logs done.", info.accountName));
    mAccountIndex++;
    exportAccount();
}

void ExportAccountJob::exportDatabase(const ImportExportUtils::AccountImportExportInfo &info)
{
    // TODO
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
    // qDebug() << " directoryToStore " << directoryToStore;
    if (directoryToStore.exists()) {
        const bool addFolder = mArchive->addLocalDirectory(directoryToStore.path(), subfolderPath);
        if (!addFolder) {
            Q_EMIT exportFailed(i18n("Directory \"%1\" cannot be added to backup file.", directoryToStore.path()));
        }
    }
}

#include "moc_exportaccountjob.cpp"
