/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqola_private_export.h"
#include <QJsonObject>
#include <QMap>
class LIBRUQOLACORE_TESTS_EXPORT DownloadAppsLanguagesInfo
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
