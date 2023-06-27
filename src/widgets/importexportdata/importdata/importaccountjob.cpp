/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "importaccountjob.h"
#include "importexportdata/importexportutils.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <KZip>
#include <QDir>
#include <QStandardPaths>

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
        qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to open zip file";
        return;
    }

    const KArchiveDirectory *zipDir = mArchive->directory();
    const KArchiveEntry *accountsEntry = zipDir->entry(QStringLiteral("accounts"));
    QStringList accountInfos;
    if (accountsEntry && accountsEntry->isFile()) {
        const auto accountsFile = static_cast<const KArchiveFile *>(accountsEntry);
        // TODO read list of accounts
    }
    for (const auto &accountName : std::as_const(accountInfos)) {
        // TODO
    }
    Q_EMIT importDone();
    deleteLater();
}

void ImportAccountJob::importAccount(QString accountName)
{
    accountName = verifyExistingAccount(accountName);
    qDebug() << "accountName " << accountName;
    {
        // config files
        const QString configPath = accountName + QLatin1Char('/') + ImportExportUtils::configPath();
        const KArchiveEntry *configPathEntry = mArchive->directory()->entry(configPath);
        if (configPathEntry && configPathEntry->isDirectory()) {
            const auto configDirectory = static_cast<const KArchiveDirectory *>(configPathEntry);
            const QStringList lst = configDirectory->entries();
            for (const QString &file : lst) {
                const KArchiveEntry *filePathEntry = mArchive->directory()->entry(configPath + QStringLiteral("/%1").arg(file));
                if (filePathEntry && filePathEntry->isFile()) {
                    const auto filePath = static_cast<const KArchiveDirectory *>(filePathEntry);
                    filePath->copyTo(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QStringLiteral("/ruqola/") + accountName);
                } else {
                    qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to import file ";
                }
            }
        }
    }
    {
        // local files
        const QString localPath = accountName + QStringLiteral("/logs/");
        const KArchiveEntry *localPathEntry = mArchive->directory()->entry(localPath);
        if (localPathEntry && localPathEntry->isDirectory()) {
            const auto localPathDirectory = static_cast<const KArchiveDirectory *>(localPathEntry);
            copyToDirectory(localPathDirectory,
                            QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QStringLiteral("/logs/") + accountName); // TODO
            // TODO
        }
    }
    // TODO cache ?
}

void ImportAccountJob::copyToDirectory(const KArchiveDirectory *subfolderDir, const QString &dest)
{
    if (!subfolderDir->copyTo(dest)) {
        qCDebug(RUQOLAWIDGETS_LOG) << "directory cannot copy to " << dest;
    }
}

QString ImportAccountJob::verifyExistingAccount(QString accountName) const
{
    int i = 1;
    while (QDir(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QStringLiteral("/ruqola/") + accountName).exists()) {
        accountName = QStringLiteral("%1_%2").arg(accountName).arg(QString::number(i));
    }
    return accountName;
}

#include "moc_importaccountjob.cpp"
