/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "reactionstest.h"
#include "messages/reactions.h"
#include "ruqola_autotest_helper.h"
#include <QDebug>
#include <QJsonObject>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
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
    QTest::addColumn<Reactions *>("expectedReactions");
    {
        Reactions *reactionsRef = new Reactions;
        Reaction react;
        react.setReactionName(u":ok_hand:"_s);
        react.setUserNames(QStringList() << u"foo"_s << u"bla"_s << u"bli"_s);
        reactionsRef->setReactions({react});
        QTest::addRow("reactions") << u"reactions"_s << reactionsRef;
    }
    {
        Reactions *reactionsRef = new Reactions;
        Reaction react;
        react.setReactionName(u":mrs_claus:"_s);
        react.setUserNames(QStringList() << u"bla"_s);
        Reaction react2;
        react2.setReactionName(u":right_facing_fist:"_s);
        react2.setUserNames(QStringList() << u"bli"_s);
        reactionsRef->setReactions({react, react2});
        QTest::addRow("reactions2") << u"reactions2"_s << reactionsRef;
    }
}

void ReactionsTest::shouldParseReactions()
{
    QFETCH(QString, name);
    QFETCH(Reactions *, expectedReactions);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/json/"_L1 + name + ".json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);
    Reactions originalReactions;
    originalReactions.parseReactions(obj);
    const bool emojiIsEqual = (originalReactions == *expectedReactions);
    if (!emojiIsEqual) {
        qDebug() << "originalReactions " << originalReactions;
        qDebug() << "expectedReactions " << expectedReactions;
    }
    QVERIFY(emojiIsEqual);
}

#include "moc_reactionstest.cpp"
