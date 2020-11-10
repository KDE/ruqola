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

#ifndef OWNUSERPREFERENCES_H
#define OWNUSERPREFERENCES_H

#include "libruqolacore_export.h"

#include <QJsonObject>
#include <QDebug>

class LIBRUQOLACORE_EXPORT OwnUserPreferences
{
public:
    OwnUserPreferences();
    ~OwnUserPreferences();
    void parsePreferences(const QJsonObject &replyObject);
    Q_REQUIRED_RESULT bool operator ==(const OwnUserPreferences &other) const;
    Q_REQUIRED_RESULT QStringList highlightWords() const;
    void setHighlightWords(const QStringList &highlightWords);

private:
    QStringList mHighlightWords;
};

Q_DECLARE_METATYPE(OwnUserPreferences)
Q_DECLARE_TYPEINFO(OwnUserPreferences, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator <<(QDebug d, const OwnUserPreferences &t);

#endif // OWNUSERPREFERENCES_H
