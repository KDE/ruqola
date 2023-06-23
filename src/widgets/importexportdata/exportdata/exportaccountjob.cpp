/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exportaccountjob.h"
#include "ruqolawidgets_debug.h"

ExportAccountJob::ExportAccountJob(QObject *parent)
    : QObject{parent}
{
}

ExportAccountJob::~ExportAccountJob() = default;

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
