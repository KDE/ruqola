/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "libruqola_private_export.h"
#include <QJsonObject>
#include <QMap>
class LIBRUQOLACORE_TESTS_EXPORT DownloadAppsLanguagesInfo
{
public:
    DownloadAppsLanguagesInfo();
    Q_REQUIRED_RESULT bool parse(const QJsonObject &language, const QString &id);
    Q_REQUIRED_RESULT QMap<QString, QMap<QString, QString>> languageMap() const;
    Q_REQUIRED_RESULT bool isEmpty() const;

private:
    // QMap<lang, <id, value>>
    QMap<QString, QMap<QString, QString>> mLanguageMap;
};

