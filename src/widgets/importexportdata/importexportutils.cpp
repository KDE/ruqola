/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "importexportutils.h"

QString ImportExportUtils::configPath()
{
    return QStringLiteral("config");
}

QString ImportExportUtils::localPath()
{
    return QStringLiteral("local");
}

QString ImportExportUtils::logsPath()
{
    return QStringLiteral("logs");
}

QString ImportExportUtils::databasePath()
{
    return QStringLiteral("database");
}

QDebug operator<<(QDebug d, const ImportExportUtils::AccountImportExportInfo &t)
{
    d << "path " << t.path;
    d << "accountName " << t.accountName;
    return d;
}
