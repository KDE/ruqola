/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QDebug>
#include <QString>
namespace ImportExportUtils
{
struct AccountImportExportInfo {
    QString path;
    QString accountName;
};

Q_REQUIRED_RESULT LIBRUQOLAWIDGETS_TESTS_EXPORT QString configPath();
Q_REQUIRED_RESULT LIBRUQOLAWIDGETS_TESTS_EXPORT QString localPath();
Q_REQUIRED_RESULT LIBRUQOLAWIDGETS_TESTS_EXPORT QString logsPath();
Q_REQUIRED_RESULT LIBRUQOLAWIDGETS_TESTS_EXPORT QString databasePath();
Q_REQUIRED_RESULT LIBRUQOLAWIDGETS_TESTS_EXPORT QString cachePath();
};
LIBRUQOLAWIDGETS_TESTS_EXPORT QDebug operator<<(QDebug d, const ImportExportUtils::AccountImportExportInfo &t);
Q_DECLARE_TYPEINFO(ImportExportUtils::AccountImportExportInfo, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(ImportExportUtils::AccountImportExportInfo)
