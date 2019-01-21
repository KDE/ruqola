/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

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

#ifndef MENTION_H
#define MENTION_H
#include "libruqola_private_export.h"
#include <QDebug>

class LIBRUQOLACORE_TESTS_EXPORT Mention
{
    Q_GADGET
public:
    Mention();

    Q_REQUIRED_RESULT bool operator ==(const Mention &other) const;

    Q_REQUIRED_RESULT QString message() const;
    void setMessage(const QString &message);

    Q_REQUIRED_RESULT QString messageId() const;
    void setMessageId(const QString &messageId);

private:
    QString mMessage;
    QString mMessageId;
};
Q_DECLARE_METATYPE(Mention)
Q_DECLARE_TYPEINFO(Mention, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator <<(QDebug d, const Mention &t);

#endif // MENTION_H
