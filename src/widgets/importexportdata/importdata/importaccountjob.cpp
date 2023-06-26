/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "importaccountjob.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <KZip>

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

void ImportAccountJob::importAccount(const QString &accountName)
{
    // TODO
}

void ImportAccountJob::copyToDirectory(const KArchiveEntry *entry, const QString &dest)
{
    const auto subfolderDir = static_cast<const KArchiveDirectory *>(entry);
    if (!subfolderDir->copyTo(dest)) {
        qCDebug(RUQOLAWIDGETS_LOG) << "directory cannot copy to " << dest;
    }
}

#include "moc_importaccountjob.cpp"
