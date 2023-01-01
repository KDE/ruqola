/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

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
    void setFilename(const QString &filename);
    Q_REQUIRED_RESULT bool parse();

    Q_REQUIRED_RESULT QMap<QString, DownloadAppsLanguagesInfo> map() const;

private:
    Q_DISABLE_COPY(DownloadAppsLanguagesParser)
    QMap<QString, DownloadAppsLanguagesInfo> mMap;
    QString mFileName;
};
