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

#include "downloadappslanguagesinfo.h"


DownloadAppsLanguagesInfo::DownloadAppsLanguagesInfo()
{
}

bool DownloadAppsLanguagesInfo::parse(const QJsonObject &language, const QString &id)
{
    const QJsonObject languagesObj = language[QStringLiteral("languages")].toObject();
    if (languagesObj.isEmpty()) {
        return false;
    }
    const QStringList keys = languagesObj.keys();
    for (const QString &lang : keys) {
        const QVariantMap map = languagesObj.value(lang).toObject().toVariantMap();
        QMap<QString, QVariant>::const_iterator i = map.constBegin();
        QMap<QString, QString> translatedMap;
        while (i != map.constEnd()) {
            translatedMap.insert(QStringLiteral("apps-%1-%2").arg(id, i.key()), i.value().toString());
            ++i;
        }
        mLanguageMap.insert(lang, translatedMap);
    }
    return true;
}

QMap<QString, QMap<QString, QString>> DownloadAppsLanguagesInfo::languageMap() const
{
    return mLanguageMap;
}

bool DownloadAppsLanguagesInfo::isEmpty() const
{
    return mLanguageMap.isEmpty();
}
