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
}

#include "moc_exportaccountjob.cpp"
