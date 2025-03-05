/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "downloadappslanguagesinfo.h"
#include "libruqolacore_export.h"
#include <QMap>
class LIBRUQOLACORE_EXPORT DownloadAppsLanguagesParser
{
public:
    DownloadAppsLanguagesParser();
    [[nodiscard]] QMap<QString, DownloadAppsLanguagesInfo> parse(const QJsonObject &obj) const;
};
