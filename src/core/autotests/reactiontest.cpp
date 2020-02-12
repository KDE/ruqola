/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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

#include "reactiontest.h"
#include "messages/reaction.h"

#include <QTest>
QTEST_GUILESS_MAIN(ReactionTest)

ReactionTest::ReactionTest(QObject *parent)
    : QObject(parent)
{
}

void ReactionTest::shouldHaveDefaultValue()
{
    Reaction r;
    QVERIFY(r.userNames().isEmpty());
    QVERIFY(r.reactionName().isEmpty());
    QCOMPARE(r.count(), 0);
    QVERIFY(!r.isAnimatedImage());
}

void ReactionTest::shouldReturnCount()
{
    Reaction r;
    r.setReactionName(QStringLiteral("bla"));
    QCOMPARE(r.reactionName(), QStringLiteral("bla"));
    r.setUserNames({QStringLiteral("dd"), QStringLiteral("dd2")});
    QCOMPARE(r.count(), 2);
}

void ReactionTest::shouldShowReactionsToolTip()
{
    Reaction r;
    r.setReactionName(QStringLiteral(":foo:"));
    QCOMPARE(r.convertedUsersNameAtToolTip(), QString());
    QStringList userNames;
    userNames.append(QStringLiteral("bla"));
    r.setUserNames(userNames);
    QCOMPARE(r.convertedUsersNameAtToolTip(), QStringLiteral("bla reacted with :foo:"));
    userNames.append(QStringLiteral("blo"));
    r.setUserNames(userNames);
    QCOMPARE(r.convertedUsersNameAtToolTip(), QStringLiteral("bla and blo reacted with :foo:"));
    userNames.append(QStringLiteral("bli"));
    r.setUserNames(userNames);
    QCOMPARE(r.convertedUsersNameAtToolTip(), QStringLiteral("bla, blo and bli reacted with :foo:"));
}
