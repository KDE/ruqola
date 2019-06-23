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

#ifndef MENTIONS_H
#define MENTIONS_H

#include "libruqola_private_export.h"
#include "mention.h"
#include <QVector>

class LIBRUQOLACORE_TESTS_EXPORT Mentions
{
public:
    Mentions();
    void setMentions(const QVector<Mention> &mentions);
    Q_REQUIRED_RESULT QVector<Mention> mentions() const;

    void parseMentions(const QJsonObject &array);

    Q_REQUIRED_RESULT bool operator ==(const Mentions &other) const;

    Q_REQUIRED_RESULT static QJsonObject serialize(const Mentions &mentions);
    Q_REQUIRED_RESULT static Mentions fromJSon(const QJsonObject &o);

    Q_REQUIRED_RESULT bool isEmpty() const;

    Q_REQUIRED_RESULT Mention at(int index) const;
    void clear();
    Q_REQUIRED_RESULT int count() const;

    Q_REQUIRED_RESULT int mentionsCount() const;
    void setMentionsCount(int mentionsCount);

    Q_REQUIRED_RESULT int offset() const;
    void setOffset(int offset);

    Q_REQUIRED_RESULT int total() const;
    void setTotal(int total);

    void parseMoreMentions(const QJsonObject &threadsObj);

private:
    void parseMentionsObj(const QJsonObject &discussionsObj);
    QVector<Mention> mMentions;
    int mMentionsCount = 0;
    int mOffset = 0;
    int mTotal = 0;
};
Q_DECLARE_METATYPE(Mentions)
LIBRUQOLACORE_EXPORT QDebug operator <<(QDebug d, const Mentions &t);

#endif // MENTIONS_H
