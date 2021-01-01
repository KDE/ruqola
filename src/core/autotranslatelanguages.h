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

#ifndef AUTOTRANSLATELANGUAGES_H
#define AUTOTRANSLATELANGUAGES_H

#include "libruqola_private_export.h"
#include "autotranslatelanguage.h"
#include <QDebug>
#include <QVector>

class LIBRUQOLACORE_TESTS_EXPORT AutotranslateLanguages
{
public:
    AutotranslateLanguages();

    Q_REQUIRED_RESULT QVector<AutotranslateLanguage> autotranslateLanguage() const;
    void setAutotranslateLanguages(const QVector<AutotranslateLanguage> &autotranslateLanguage);

    Q_REQUIRED_RESULT bool isEmpty() const;
    Q_REQUIRED_RESULT int count() const;
    void clear();
    Q_REQUIRED_RESULT AutotranslateLanguage at(int index) const;

    void parseLanguages(const QJsonObject &obj);

private:
    QVector<AutotranslateLanguage> mAutotranslateLanguages;
};

Q_DECLARE_METATYPE(AutotranslateLanguages)
LIBRUQOLACORE_EXPORT QDebug operator <<(QDebug d, const AutotranslateLanguages &t);
#endif // AUTOTRANSLATELANGUAGES_H
