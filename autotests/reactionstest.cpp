/*
   Copyright (C) 2018 Montel Laurent <montel@kde.org>

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

#include "reactionstest.h"
#include "reactions.h"

#include <QJsonDocument>
#include <QTest>
#include <QDebug>
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
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QStringLiteral("/json/") + name + QStringLiteral(".json");
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject obj = doc.object();
    Reactions originalReactions;
    originalReactions.parseReactions(obj);
    const bool emojiIsEqual = (originalReactions == expectedReactions);
    if (!emojiIsEqual) {
        qDebug() << "originalReactions " << originalReactions;
        qDebug() << "expectedReactions " << expectedReactions;
    }
    QVERIFY(emojiIsEqual);
}
