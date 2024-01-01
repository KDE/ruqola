/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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

#include "moc_reactiontest.cpp"
