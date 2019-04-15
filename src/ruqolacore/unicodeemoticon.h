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

#ifndef UNICODEEMOTICON_H
#define UNICODEEMOTICON_H

#include <QObject>
#include <QDebug>
#include <QString>
#include "libruqola_private_export.h"

class LIBRUQOLACORE_TESTS_EXPORT UnicodeEmoticon
{
    Q_GADGET
public:
    UnicodeEmoticon();

    Q_REQUIRED_RESULT QString identifier() const;
    void setIdentifier(const QString &identifier);

    Q_REQUIRED_RESULT QString unicode() const;
    void setUnicode(uint unicode);

    Q_REQUIRED_RESULT QString category() const;
    void setCategory(const QString &category);

private:
    QString mIdentifier;
    QString mUnicode;
    QString mCategory;
};
Q_DECLARE_METATYPE(UnicodeEmoticon)
LIBRUQOLACORE_EXPORT QDebug operator <<(QDebug d, const UnicodeEmoticon &t);

#endif // UNICODEEMOTICON_H
