/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

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

#include "emoticon.h"
#include <QString>
Emoticon::Emoticon()
{
}

QString Emoticon::identifier() const
{
    return mIdentifier;
}

void Emoticon::setIdentifier(const QString &name)
{
    mIdentifier = name;
}

QString Emoticon::unicode() const
{
    return mUnicode;
}

void Emoticon::setUnicode(uint unicode)
{
    mUnicode = QString::fromUcs4(&unicode, 1);
}

QString Emoticon::category() const
{
    return mCategory;
}

void Emoticon::setCategory(const QString &category)
{
    mCategory = category;
}

QDebug operator <<(QDebug d, const Emoticon &t)
{
    d << "Name : " << t.identifier();
    d << "Unicode: " << t.unicode();
    d << "Category: " << t.category();
    return d;
}
