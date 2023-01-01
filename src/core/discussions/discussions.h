/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "discussion.h"
#include "libruqola_private_export.h"
#include <QDebug>
#include <QVector>

class LIBRUQOLACORE_TESTS_EXPORT Discussions
{
public:
    Discussions();

    Q_REQUIRED_RESULT QVector<Discussion> discussions() const;
    void setDiscussions(const QVector<Discussion> &discussions);

    void parseDiscussions(const QJsonObject &array);

    void parseMoreDiscussions(const QJsonObject &discussionsObj);

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

    void append(const Discussion &discussion);

private:
    void parseDiscussionsObj(const QJsonObject &discussionsObj);
    QVector<Discussion> mDiscussion;
    int mDiscussionsCount = 0;
    int mOffset = 0;
    int mTotal = 0;
};
Q_DECLARE_METATYPE(Discussions)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Discussions &t);
