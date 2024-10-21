/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "downloadappslanguagesinfo.h"

#include <QVariant>

using namespace Qt::Literals::StringLiterals;
DownloadAppsLanguagesInfo::DownloadAppsLanguagesInfo() = default;

bool DownloadAppsLanguagesInfo::parse(const QJsonObject &language, const QString &id)
{
    const QJsonObject languagesObj = language["languages"_L1].toObject();
    if (languagesObj.isEmpty()) {
        return false;
    }
    const QStringList keys = languagesObj.keys();
    for (const QString &lang : keys) {
        const QVariantMap map = languagesObj.value(lang).toObject().toVariantMap();
        QMap<QString, QVariant>::const_iterator i = map.constBegin();
        QMap<QString, QString> translatedMap;
        while (i != map.constEnd()) {
            translatedMap.insert(QStringLiteral("app-%1.%2").arg(id, i.key()), i.value().toString());
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
