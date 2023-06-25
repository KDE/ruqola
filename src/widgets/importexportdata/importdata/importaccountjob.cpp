/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "importaccountjob.h"
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
    if (canStart()) {
        deleteLater();
        return;
    }
    // TODO
    deleteLater();
}

bool ImportAccountJob::canStart() const
{
    // TODO
    return true;
}

#include "moc_importaccountjob.cpp"
