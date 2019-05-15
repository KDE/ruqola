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


#ifndef DISCUSSIONS_H
#define DISCUSSIONS_H

#include "libruqola_private_export.h"
#include "discussion.h"
#include <QVector>
#include <QDebug>

class LIBRUQOLACORE_TESTS_EXPORT Discussions
{
public:
    Discussions();

    Q_REQUIRED_RESULT QVector<Discussion> discussions() const;
    void setDiscussions(const QVector<Discussion> &discussions);

    void parseDiscussions(const QJsonObject &array);

    Q_REQUIRED_RESULT bool isEmpty() const;
    void clear();
    Q_REQUIRED_RESULT int count() const;
    Q_REQUIRED_RESULT Discussion at(int index) const;
    Q_REQUIRED_RESULT int discussionsCount() const;
    void setDiscussionsCount(int discussionsCount);

    Q_REQUIRED_RESULT int offset() const;
    void setOffset(int offset);

    Q_REQUIRED_RESULT int total() const;
    void setTotal(int total);

private:
    QVector<Discussion> mDiscussion;
    int mDiscussionsCount = 0;
    int mOffset = 0;
    int mTotal = 0;
};
Q_DECLARE_METATYPE(Discussions)
LIBRUQOLACORE_EXPORT QDebug operator <<(QDebug d, const Discussions &t);

#endif // DISCUSSIONS_H
