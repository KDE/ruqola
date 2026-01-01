/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "downloadappslanguagesparser.h"

#include <QJsonArray>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
DownloadAppsLanguagesParser::DownloadAppsLanguagesParser() = default;

QMap<QString, DownloadAppsLanguagesInfo> DownloadAppsLanguagesParser::parse(const QJsonObject &obj) const
{
    QMap<QString, DownloadAppsLanguagesInfo> map;
    const QJsonArray array = obj.value("apps"_L1).toArray();
    for (int i = 0, total = array.size(); i < total; ++i) {
        DownloadAppsLanguagesInfo info;
        const QJsonObject languageJsonObject = array.at(i).toObject();
        const QString id = languageJsonObject["id"_L1].toString();
        if (info.parse(languageJsonObject, id)) {
            map.insert(id, info);
        }
    }
    return map;
}
