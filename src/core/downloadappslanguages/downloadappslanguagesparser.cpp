/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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
        const QString id = languageJsonObject[QStringLiteral("id")].toString();
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
