/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "reactionstest.h"
#include "messages/reactions.h"
#include "ruqola_autotest_helper.h"
#include <QDebug>
#include <QJsonObject>
#include <QTest>
QTEST_GUILESS_MAIN(ReactionsTest)

ReactionsTest::ReactionsTest(QObject *parent)
    : QObject(parent)
{
}

void ReactionsTest::shouldHaveDefaultValue()
{
    Reactions reacts;
    QVERIFY(reacts.reactions().isEmpty());
    QVERIFY(reacts.isEmpty());
}

void ReactionsTest::shouldParseReactions_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<Reactions>("expectedReactions");
    {
        Reactions reactionsRef;
        Reaction react;
        react.setReactionName(QStringLiteral(":ok_hand:"));
        react.setUserNames(QStringList() << QStringLiteral("foo") << QStringLiteral("bla") << QStringLiteral("bli"));
        reactionsRef.setReactions({react});
        QTest::addRow("reactions") << QStringLiteral("reactions") << reactionsRef;
    }
    {
        Reactions reactionsRef;
        Reaction react;
        react.setReactionName(QStringLiteral(":mrs_claus:"));
        react.setUserNames(QStringList() << QStringLiteral("bla"));
        Reaction react2;
        react2.setReactionName(QStringLiteral(":right_facing_fist:"));
        react2.setUserNames(QStringList() << QStringLiteral("bli"));
        reactionsRef.setReactions({react, react2});
        QTest::addRow("reactions2") << QStringLiteral("reactions2") << reactionsRef;
    }
}

void ReactionsTest::shouldParseReactions()
{
    QFETCH(QString, name);
    QFETCH(Reactions, expectedReactions);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/json/") + name + QLatin1String(".json");
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);
    Reactions originalReactions;
    originalReactions.parseReactions(obj);
    const bool emojiIsEqual = (originalReactions == expectedReactions);
    if (!emojiIsEqual) {
        qDebug() << "originalReactions " << originalReactions;
        qDebug() << "expectedReactions " << expectedReactions;
    }
    QVERIFY(emojiIsEqual);
}

#include "moc_reactionstest.cpp"
