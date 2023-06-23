/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exportaccountjob.h"

ExportAccountJob::ExportAccountJob(QObject *parent)
    : QObject{parent}
{
}

ExportAccountJob::~ExportAccountJob()
{
}

void ExportAccountJob::start()
{
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

#include "moc_exportaccountjob.cpp"
