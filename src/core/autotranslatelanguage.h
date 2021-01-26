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

#ifndef AUTOTRANSLATELANGUAGE_H
#define AUTOTRANSLATELANGUAGE_H

#include "libruqola_private_export.h"
#include <QDebug>

class LIBRUQOLACORE_TESTS_EXPORT AutotranslateLanguage
{
public:
    AutotranslateLanguage();
    Q_REQUIRED_RESULT QString language() const;
    void setLanguage(const QString &language);

    Q_REQUIRED_RESULT QString displayLanguage() const;
    void setDisplayLanguage(const QString &displayLanguage);

private:
    QString mLanguage;
    QString mDisplayLanguage;
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const AutotranslateLanguage &t);
Q_DECLARE_METATYPE(AutotranslateLanguage)
Q_DECLARE_TYPEINFO(AutotranslateLanguage, Q_MOVABLE_TYPE);
#endif // AUTOTRANSLATELANGUAGE_H
