/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "emojitest.h"
#include "emoji.h"
#include <QJsonObject>
#include <QTest>
#include <qjsondocument.h>

QTEST_GUILESS_MAIN(EmojiTest)

EmojiTest::EmojiTest(QObject *parent)
    : QObject(parent)
{
}

void EmojiTest::shouldHaveDefaultValue()
{
    Emoji j;
    QVERIFY(j.extension().isEmpty());
    QVERIFY(j.identifier().isEmpty());
    QVERIFY(j.name().isEmpty());
}

void EmojiTest::shouldAssignValue()
{
    Emoji j;
    const QString ext{QStringLiteral("foo")};
    const QString id{QStringLiteral("bla")};
    const QString name{QStringLiteral("bli")};
    j.setExtension(ext);
    j.setName(name);
    j.setIdentifier(id);
    QCOMPARE(j.extension(), ext);
    QCOMPARE(j.identifier(), id);
    QCOMPARE(j.name(), name);
}

void EmojiTest::shouldParseEmoji_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<Emoji>("expectedEmoji");
    {
        Emoji emojiRef;
        emojiRef.setExtension(QStringLiteral("jpg"));
        emojiRef.setName(QStringLiteral("troll"));
        emojiRef.setIdentifier(QStringLiteral("2cgzHwKP6Cq3iZCob"));

        QTest::addRow("emoji") << QStringLiteral("emoji") << emojiRef;
    }
    {
        Emoji emojiRef;
        emojiRef.setExtension(QStringLiteral("gif"));
        emojiRef.setName(QStringLiteral("clapping"));
        emojiRef.setIdentifier(QStringLiteral("scSbxNPzm9xWrNqCG"));
        emojiRef.setAliases(QStringList{QStringLiteral("clap")});
        QTest::addRow("emojialias") << QStringLiteral("emojialias") << emojiRef;
    }
}

void EmojiTest::shouldParseEmoji()
{
    QFETCH(QString, name);
    QFETCH(Emoji, expectedEmoji);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QStringLiteral("/json/") + name + QStringLiteral(".json");
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject obj = doc.object();
    Emoji originalEmoji;
    originalEmoji.parseEmoji(obj);
    const bool emojiIsEqual = (originalEmoji == expectedEmoji);
    if (!emojiIsEqual) {
        qDebug() << "originalEmoji " << originalEmoji;
        qDebug() << "ExpectedEmoji " << expectedEmoji;
    }
    QVERIFY(emojiIsEqual);
}
