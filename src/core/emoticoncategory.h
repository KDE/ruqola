/*
   Copyright (c) 2019-2020 Laurent Montel <montel@kde.org>

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

#ifndef EMOTICONCATEGORY_H
#define EMOTICONCATEGORY_H

#include "libruqolacore_export.h"
#include <QString>
#include <QDebug>

class LIBRUQOLACORE_EXPORT EmoticonCategory
{
public:
    EmoticonCategory();
    Q_REQUIRED_RESULT QString name() const;
    void setName(const QString &name);

    Q_REQUIRED_RESULT QString category() const;
    void setCategory(const QString &category);

private:
    QString mName;
    QString mCategory;
};
Q_DECLARE_METATYPE(EmoticonCategory)
Q_DECLARE_TYPEINFO(EmoticonCategory, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator <<(QDebug d, const EmoticonCategory &t);

#endif // EMOTICONCATEGORY_H
