/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exportaccountjob.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <KZip>
#include <QDir>
#include <QStandardPaths>

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
        qCDebug(RUQOLAWIDGETS_LOG) << " Account list is empty! ";
        return;
    }
    const bool result = mArchive->open(QIODevice::WriteOnly);
    if (!result) {
        deleteLater();
        qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to open zip file";
        return;
    }
    for (const auto &account : mListAccounts) {
        exportAccount(account);
    }
    Q_EMIT exportDone();
    deleteLater();
}

QVector<ImportExportUtils::AccountImportExportInfo> ExportAccountJob::listAccounts() const
{
    return mListAccounts;
}

void ExportAccountJob::exportAccount(const ImportExportUtils::AccountImportExportInfo &info)
{
    // config files
    storeDirectory(QStringLiteral("TODO"), info.accountName + ImportExportUtils::configPath());

    // local files
    storeDirectory(QStringLiteral("TODO"), info.accountName + ImportExportUtils::localPath());

    // TODO
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
    const QDir directoryToStore(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + subDirectory);
    if (directoryToStore.exists()) {
        const bool addFolder = mArchive->addLocalDirectory(directoryToStore.path(), subfolderPath);
        if (!addFolder) {
            Q_EMIT exportFailed(i18n("Directory \"%1\" cannot be added to backup file.", directoryToStore.path()));
        }
    }
}

void ExportAccountJob::storeFile(const QString &subDirectory, const QString &subfolderPath)
{
    const QDir directoryToStore(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + subDirectory);
    if (directoryToStore.exists()) {
        const bool addFolder = mArchive->addLocalFile(directoryToStore.path(), subfolderPath);
        if (!addFolder) {
            Q_EMIT exportFailed(i18n("file \"%1\" cannot be added to backup file.", directoryToStore.path()));
        }
    }
}

#include "moc_exportaccountjob.cpp"
