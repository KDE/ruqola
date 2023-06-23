/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "importaccountjob.h"

ImportAccountJob::ImportAccountJob(QObject *parent)
    : QObject{parent}
{
}

ImportAccountJob::~ImportAccountJob()
{
}

void ImportAccountJob::start()
{
    // TODO
}

#include "moc_importaccountjob.cpp"
