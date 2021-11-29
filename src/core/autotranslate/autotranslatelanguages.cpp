/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autotranslatelanguages.h"

#include <QJsonArray>
#include <QJsonObject>

AutotranslateLanguages::AutotranslateLanguages() = default;

QVector<AutotranslateLanguage> AutotranslateLanguages::autotranslateLanguage() const
{
    return mAutotranslateLanguages;
}

void AutotranslateLanguages::setAutotranslateLanguages(const QVector<AutotranslateLanguage> &autotranslateLanguage)
{
    mAutotranslateLanguages = autotranslateLanguage;
}

bool AutotranslateLanguages::isEmpty() const
{
    return mAutotranslateLanguages.isEmpty();
}

int AutotranslateLanguages::count() const
{
    return mAutotranslateLanguages.count();
}

void AutotranslateLanguages::clear()
{
    mAutotranslateLanguages.clear();
}

AutotranslateLanguage AutotranslateLanguages::at(int index) const
{
    return mAutotranslateLanguages.at(index);
}

void AutotranslateLanguages::parseLanguages(const QJsonObject &obj)
{
    clear();
    const QJsonArray array = obj[QLatin1String("languages")].toArray();
    for (const QJsonValue &current : array) {
        const QJsonObject languageObject = current.toObject();
        AutotranslateLanguage lang;
        lang.setLanguage(languageObject.value(QLatin1String("language")).toString());
        mAutotranslateLanguages.append(lang);
    }
}

QDebug operator<<(QDebug d, const AutotranslateLanguages &t)
{
    const QVector<AutotranslateLanguage> autotranslateLanguages = t.autotranslateLanguage();
    for (const AutotranslateLanguage &lang : autotranslateLanguages) {
        d << "autotranslate :" << lang << "\n";
    }
    return d;
}
