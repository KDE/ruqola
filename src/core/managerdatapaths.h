/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
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

    [[nodiscard]] QString path(PathType type, const QString &accountName, const QString &subdirectory = QString()) const;

    [[nodiscard]] QString accountConfigFileName(const QString &accountName) const;
    [[nodiscard]] QString accountAvatarConfigPath(const QString &accountName) const;

private:
    LIBRUQOLACORE_NO_EXPORT void initializePaths();
    QHash<ManagerDataPaths::PathType, QString> mPathTypeHash;
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QString accountConfigPath(const QString &accountName) const;
};
