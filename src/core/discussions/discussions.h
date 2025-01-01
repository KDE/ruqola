/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "discussion.h"
#include "libruqola_private_export.h"
#include <QDebug>
#include <QList>

class LIBRUQOLACORE_TESTS_EXPORT Discussions
{
public:
    Discussions();

    [[nodiscard]] QList<Discussion> discussions() const;
    void setDiscussions(const QList<Discussion> &discussions);

    void parseDiscussions(const QJsonObject &array);

    void parseMoreDiscussions(const QJsonObject &discussionsObj);

    [[nodiscard]] bool isEmpty() const;
    void clear();
    [[nodiscard]] int count() const;
    [[nodiscard]] Discussion at(int index) const;
    [[nodiscard]] int discussionsCount() const;
    void setDiscussionsCount(int discussionsCount);

    [[nodiscard]] int offset() const;
    void setOffset(int offset);

    [[nodiscard]] int total() const;
    void setTotal(int total);

    void append(const Discussion &discussion);

private:
    LIBRUQOLACORE_NO_EXPORT void parseDiscussionsObj(const QJsonObject &discussionsObj);
    QList<Discussion> mDiscussion;
    int mDiscussionsCount = 0;
    int mOffset = 0;
    int mTotal = 0;
};
Q_DECLARE_METATYPE(Discussions)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Discussions &t);
