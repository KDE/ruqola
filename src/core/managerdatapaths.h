/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

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
        PreviewUrl = 4,
        CustomSound = 5,
    };
    static ManagerDataPaths *self();

    [[nodiscard]] QString path(PathType type, const QString &accountName, const QString &subdirectory = QString()) const;

    [[nodiscard]] QString accountConfigFileName(const QString &accountName) const;
    [[nodiscard]] QString accountAvatarConfigPath(const QString &accountName) const;

private:
    ManagerDataPaths();
    LIBRUQOLACORE_NO_EXPORT void initializePaths();
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QString accountConfigPath(const QString &accountName) const;
    QHash<ManagerDataPaths::PathType, QString> mPathTypeHash;
};
