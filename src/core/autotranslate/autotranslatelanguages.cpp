/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autotranslatelanguages.h"
#include "ruqola_debug.h"
#include <QJsonArray>
#include <QJsonObject>

AutotranslateLanguages::AutotranslateLanguages() = default;

QList<AutotranslateLanguage> AutotranslateLanguages::autotranslateLanguage() const
{
    return mAutotranslateLanguages;
}

void AutotranslateLanguages::setAutotranslateLanguages(const QList<AutotranslateLanguage> &autotranslateLanguage)
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
    if (index < 0 || index > mAutotranslateLanguages.count()) {
        qCWarning(RUQOLA_LOG) << "Invalid index " << index;
        return {};
    }
    return mAutotranslateLanguages.at(index);
}

void AutotranslateLanguages::parseLanguages(const QJsonObject &obj)
{
    clear();
    // qDebug() << " obj " << obj;
    const QJsonArray array = obj[QLatin1StringView("languages")].toArray();
    for (const QJsonValue &current : array) {
        const QJsonObject languageObject = current.toObject();
        AutotranslateLanguage lang;
        lang.setLanguage(languageObject.value(QLatin1StringView("language")).toString());
        lang.setDisplayLanguage(languageObject.value(QLatin1StringView("name")).toString());
        mAutotranslateLanguages.append(std::move(lang));
    }
}

QDebug operator<<(QDebug d, const AutotranslateLanguages &t)
{
    const QList<AutotranslateLanguage> autotranslateLanguages = t.autotranslateLanguage();
    for (const AutotranslateLanguage &lang : autotranslateLanguages) {
        d << "autotranslate :" << lang << "\n";
    }
    return d;
}
