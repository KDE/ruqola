/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QDebug>
#include <QString>
namespace ImportExportUtils
{
struct AccountImportExportInfo {
    QString path;
    QString accountName;
};

Q_REQUIRED_RESULT QString configPath();
Q_REQUIRED_RESULT QString localPath();
Q_REQUIRED_RESULT QString logsPath();
Q_REQUIRED_RESULT QString databasePath();
};
QDebug operator<<(QDebug d, const ImportExportUtils::AccountImportExportInfo &t);
Q_DECLARE_TYPEINFO(ImportExportUtils::AccountImportExportInfo, Q_MOVABLE_TYPE);
