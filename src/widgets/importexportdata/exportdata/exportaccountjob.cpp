/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

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
    : QObject{parent}
    , mArchive(new KZip(fileName))
{
}

ExportAccountJob::~ExportAccountJob()
{
    if (mArchive && mArchive->isOpen()) {
        mArchive->close();
    }
    delete mArchive;
}

void ExportAccountJob::start()
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
    QTemporaryFile tmp;
    tmp.open();
    QTextStream text(&tmp);
    QStringList listOfAccounts;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    text.setCodec("UTF-8");
#endif
    for (const auto &account : mListAccounts) {
        exportAccount(account);
        listOfAccounts.append(account.accountName);
    }
    text << listOfAccounts.join(QLatin1Char('\n'));
    tmp.close();
    mArchive->addLocalFile(tmp.fileName(), QStringLiteral("accounts"));
    Q_EMIT exportDone();
    deleteLater();
}

QVector<ImportExportUtils::AccountImportExportInfo> ExportAccountJob::listAccounts() const
{
    return mListAccounts;
}

void ExportAccountJob::exportAccount(const ImportExportUtils::AccountImportExportInfo &info)
{
    exportConfig(info);

    exportCache(info);

    exportLogs(info);
}

void ExportAccountJob::exportConfig(const ImportExportUtils::AccountImportExportInfo &info)
{
    // config files
    const QString configPath = info.accountName + QLatin1Char('/') + ImportExportUtils::configPath();
    qCDebug(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << " configPath " << configPath;
    storeDirectory(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QStringLiteral("/ruqola/") + info.accountName, configPath);
}

void ExportAccountJob::exportCache(const ImportExportUtils::AccountImportExportInfo &info)
{
    // cache files
    const QString cachePath = info.accountName + QLatin1Char('/') + ImportExportUtils::cachePath();
    qCDebug(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << " cachePath " << cachePath;
    const QString storeCachePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + QLatin1Char('/') + info.accountName + QLatin1Char('/');
    qCDebug(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << "QStandardPaths::writableLocation(QStandardPaths::CacheLocation) " << storeCachePath;
    storeDirectory(storeCachePath, cachePath);
}

void ExportAccountJob::exportLogs(const ImportExportUtils::AccountImportExportInfo &info)
{
    // local files
    const QString localPath = info.accountName + QStringLiteral("/logs/");
    qCDebug(RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG) << " localPath " << localPath;
    storeDirectory(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QStringLiteral("/logs/") + info.accountName, localPath);
}

void ExportAccountJob::setListAccounts(const QVector<ImportExportUtils::AccountImportExportInfo> &newListAccounts)
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
