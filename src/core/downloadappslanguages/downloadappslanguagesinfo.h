/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include <QJsonObject>
#include <QMap>
class LIBRUQOLACORE_EXPORT DownloadAppsLanguagesInfo
{
public:
    DownloadAppsLanguagesInfo();
    [[nodiscard]] bool parse(const QJsonObject &language, const QString &id);
    [[nodiscard]] QMap<QString, QMap<QString, QString>> languageMap() const;
    [[nodiscard]] bool isEmpty() const;

private:
    // QMap<lang, <id, value>>
    QMap<QString, QMap<QString, QString>> mLanguageMap;
};
