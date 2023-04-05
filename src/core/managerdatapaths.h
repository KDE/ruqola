/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include <QHash>
#include <QString>

class LIBRUQOLACORE_EXPORT ManagerDataPaths
{
public:
    enum PathType {
        Config = 0,
        Cache = 1,
        Picture = 2,
        Video = 3,
    };
    ManagerDataPaths();
    static ManagerDataPaths *self();

    Q_REQUIRED_RESULT QString path(PathType type, const QString &accountName, const QString &subdirectory = QString()) const;

    Q_REQUIRED_RESULT QString accountConfigFileName(const QString &accountName) const;
    Q_REQUIRED_RESULT QString accountAvatarConfigPath(const QString &accountName) const;

private:
    LIBRUQOLACORE_NO_EXPORT void initializePaths();
    QHash<ManagerDataPaths::PathType, QString> mPathTypeHash;
    Q_REQUIRED_RESULT QString accountConfigPath(const QString &accountName) const;
};
