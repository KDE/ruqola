/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "reactiontest.h"
using namespace Qt::Literals::StringLiterals;

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
    r.setReactionName(u"bla"_s);
    QCOMPARE(r.reactionName(), u"bla"_s);
    r.setUserNames({u"dd"_s, u"dd2"_s});
    QCOMPARE(r.count(), 2);
}

void ReactionTest::shouldShowReactionsToolTip()
{
    Reaction r;
    r.setReactionName(u":foo:"_s);
    QCOMPARE(r.convertedUsersNameAsToolTip(), QString());
    QStringList userNames;
    userNames.append(u"bla"_s);
    r.setUserNames(userNames);
    QCOMPARE(r.convertedUsersNameAsToolTip(), u"bla reacted with :foo:"_s);
    userNames.append(u"blo"_s);
    r.setUserNames(userNames);
    QCOMPARE(r.convertedUsersNameAsToolTip(), u"bla and blo reacted with :foo:"_s);
    userNames.append(u"bli"_s);
    r.setUserNames(userNames);
    QCOMPARE(r.convertedUsersNameAsToolTip(), u"bla, blo and bli reacted with :foo:"_s);
}

#include "moc_reactiontest.cpp"
