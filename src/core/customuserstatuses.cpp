/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#include "customuserstatuses.h"

CustomUserStatuses::CustomUserStatuses()
{

}

CustomUserStatuses::~CustomUserStatuses()
{

}

bool CustomUserStatuses::isEmpty() const
{
    return mCustomUserses.isEmpty();
}

void CustomUserStatuses::clear()
{
    mCustomUserses.clear();
}

int CustomUserStatuses::count() const
{
    return mCustomUserses.count();
}

CustomUserStatus CustomUserStatuses::at(int index) const
{
    return mCustomUserses.at(index);
}

int CustomUserStatuses::offset() const
{
    return mOffset;
}

void CustomUserStatuses::setOffset(int offset)
{
    mOffset = offset;
}

int CustomUserStatuses::total() const
{
    return mTotal;
}

void CustomUserStatuses::setTotal(int total)
{
    mTotal = total;
}

QVector<CustomUserStatus> CustomUserStatuses::customUserses() const
{
    return mCustomUserses;
}

void CustomUserStatuses::setCustomUserses(const QVector<CustomUserStatus> &customUserses)
{
    mCustomUserses = customUserses;
}

QDebug operator <<(QDebug d, const CustomUserStatuses &t)
{
    d << "total " << t.total();
    d << "offset " << t.offset();
    d << "customStatus Count " << t.count() << "\n";
    for (int i = 0, total = t.customUserses().count(); i < total; ++i) {
        d << t.customUserses().at(i) << "\n";
    }
    return d;
}
