/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exportaccountjob.h"
#include "ruqolawidgets_debug.h"
#include <KZip>

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
    // TODO
    deleteLater();
}

QStringList ExportAccountJob::listAccounts() const
{
    return mListAccounts;
}

void ExportAccountJob::setListAccounts(const QStringList &newListAccounts)
{
    mListAccounts = newListAccounts;
}

bool ExportAccountJob::canStart() const
{
    return !mListAccounts.isEmpty();
}

#include "moc_exportaccountjob.cpp"
