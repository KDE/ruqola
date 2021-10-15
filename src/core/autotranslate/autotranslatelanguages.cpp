/*
   Copyright (c) 2019-2021 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "autotranslatelanguages.h"

#include <QJsonArray>
#include <QJsonObject>

AutotranslateLanguages::AutotranslateLanguages()
{
}

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
