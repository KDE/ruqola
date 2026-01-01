/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autotranslatelanguages.h"
QT_IMPL_METATYPE_EXTERN_TAGGED(AutotranslateLanguages, Ruqola_AutotranslateLanguages)

#include "ruqola_debug.h"
#include <QJsonArray>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
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
    if (index < 0 || index >= mAutotranslateLanguages.count()) {
        qCWarning(RUQOLA_LOG) << "Invalid index " << index;
        return {};
    }
    return mAutotranslateLanguages.at(index);
}

void AutotranslateLanguages::parseLanguages(const QJsonObject &obj)
{
    clear();
    // qDebug() << " obj " << obj;
    const QJsonArray array = obj["languages"_L1].toArray();
    for (const QJsonValue &current : array) {
        const QJsonObject languageObject = current.toObject();
        AutotranslateLanguage lang;
        lang.setLanguage(languageObject.value("language"_L1).toString());
        lang.setDisplayLanguage(languageObject.value("name"_L1).toString());
        mAutotranslateLanguages.append(std::move(lang));
    }
}

QDebug operator<<(QDebug d, const AutotranslateLanguages &t)
{
    const QList<AutotranslateLanguage> autotranslateLanguages = t.autotranslateLanguage();
    for (const AutotranslateLanguage &lang : autotranslateLanguages) {
        d.space() << "autotranslate:" << lang << "\n";
    }
    return d;
}
