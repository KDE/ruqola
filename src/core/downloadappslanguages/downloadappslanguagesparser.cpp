/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "downloadappslanguagesparser.h"

#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
DownloadAppsLanguagesParser::DownloadAppsLanguagesParser() = default;

void DownloadAppsLanguagesParser::parse(const QJsonObject &obj)
{
    const QJsonArray array = obj.value("apps"_L1).toArray();
    for (int i = 0, total = array.size(); i < total; ++i) {
        DownloadAppsLanguagesInfo info;
        const QJsonObject languageJsonObject = array.at(i).toObject();
        const QString id = languageJsonObject["id"_L1].toString();
        if (info.parse(languageJsonObject, id)) {
            mMap.insert(id, info);
        }
    }
}

QMap<QString, DownloadAppsLanguagesInfo> DownloadAppsLanguagesParser::map() const
{
    return mMap;
}
