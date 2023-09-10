/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "downloadappslanguagesparser.h"
#include "ruqola_debug.h"

#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

DownloadAppsLanguagesParser::DownloadAppsLanguagesParser() = default;

void DownloadAppsLanguagesParser::setFilename(const QString &filename)
{
    mFileName = filename;
}

bool DownloadAppsLanguagesParser::parse()
{
    if (mFileName.isEmpty()) {
        qCWarning(RUQOLA_LOG) << "Filename is empty. Parsing impossible";
        return false;
    }
    if (!QFileInfo::exists(mFileName)) {
        qCWarning(RUQOLA_LOG) << "Filename doesn't exist: " << mFileName;
        return false;
    }
    QFile file(mFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCWarning(RUQOLA_LOG) << "Impossible to read: " << mFileName;
        return false;
    }
    const QByteArray content = file.readAll();
    file.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject obj = doc.object();
    const QJsonArray array = obj.value(QLatin1String("apps")).toArray();
    for (int i = 0, total = array.size(); i < total; ++i) {
        DownloadAppsLanguagesInfo info;
        const QJsonObject languageJsonObject = array.at(i).toObject();
        const QString id = languageJsonObject[QLatin1String("id")].toString();
        if (info.parse(languageJsonObject, id)) {
            mMap.insert(id, info);
        }
    }
    return true;
}

QMap<QString, DownloadAppsLanguagesInfo> DownloadAppsLanguagesParser::map() const
{
    return mMap;
}
