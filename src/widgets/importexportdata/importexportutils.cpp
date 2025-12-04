/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "importexportutils.h"
#include <QDebug>
using namespace Qt::Literals::StringLiterals;

QString ImportExportUtils::configPath()
{
    return u"config"_s;
}

QString ImportExportUtils::localPath()
{
    return u"local"_s;
}

QString ImportExportUtils::cachePath()
{
    return u"cache"_s;
}

QString ImportExportUtils::logsPath()
{
    return u"logs"_s;
}

QString ImportExportUtils::databasePath()
{
    return u"database"_s;
}

QDebug operator<<(QDebug d, const ImportExportUtils::AccountImportExportInfo &t)
{
    d.space() << "path " << t.path;
    d.space() << "accountName " << t.accountName;
    return d;
}
