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
    QVERIFY(j.aliases().isEmpty());
    QCOMPARE(j.updatedAt(), 0);
    QVERIFY(j.emojiIdentifier().isEmpty());
}

void EmojiTest::shouldAssignValue()
{
    Emoji j;
    const QString ext{QStringLiteral("foo")};
    const QString id{QStringLiteral("bla")};
    const QString name{QStringLiteral("bli")};
    const QStringList aliases{QStringLiteral(":foo:"), QStringLiteral(":bla:")};
    const qint64 updatedAt = 500;

    j.setExtension(ext);
    j.setName(name);
    j.setIdentifier(id);
    j.setAliases(aliases);
    j.setUpdatedAt(updatedAt);

    const QString emojiId = QLatin1Char(':') + name + QLatin1Char(':');
    j.setEmojiIdentifier(emojiId);

    QCOMPARE(j.extension(), ext);
    QCOMPARE(j.identifier(), id);
    QCOMPARE(j.name(), name);
    QCOMPARE(j.aliases(), aliases);
    QCOMPARE(j.emojiIdentifier(), emojiId);
    QCOMPARE(j.updatedAt(), updatedAt);
}

void EmojiTest::shouldCopyValue()
{
    Emoji j;
    const QString ext{QStringLiteral("foo")};
    const QString id{QStringLiteral("bla")};
    const QString name{QStringLiteral("bli")};
    const QStringList aliases{QStringLiteral(":foo:"), QStringLiteral(":bla:")};
    const qint64 updatedAt = 500;

    j.setExtension(ext);
    j.setName(name);
    j.setIdentifier(id);
    j.setAliases(aliases);
    j.setUpdatedAt(updatedAt);
    const QString emojiId = QLatin1Char(':') + name + QLatin1Char(':');
    j.setEmojiIdentifier(emojiId);

    Emoji copy = j;
    QCOMPARE(copy, j);
}

void EmojiTest::shouldClearCachedHtml()
{
    Emoji emojiRef;
    emojiRef.setExtension(QStringLiteral("gif"));
    emojiRef.setName(QStringLiteral("clapping"));
    emojiRef.setIdentifier(QStringLiteral("scSbxNPzm9xWrNqCG"));
    emojiRef.setAliases(QStringList{QStringLiteral(":clap:")});
    emojiRef.setEmojiIdentifier(QStringLiteral(":clapping:"));
    emojiRef.setUpdatedAt(50);

    const QString cachedHtml = emojiRef.html(QStringLiteral("www.kde.org"));
    QVERIFY(!cachedHtml.isEmpty());
    QCOMPARE(emojiRef.cachedHtml(), cachedHtml);
    emojiRef.clearCachedHtml();
    QVERIFY(emojiRef.cachedHtml().isEmpty());
    QCOMPARE(emojiRef.html(QStringLiteral("www.kde.org")), cachedHtml);
    QCOMPARE(emojiRef.cachedHtml(), cachedHtml);
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
        emojiRef.setEmojiIdentifier(QStringLiteral(":troll:"));
        emojiRef.setUpdatedAt(1485546740427);

        QTest::addRow("emoji") << QStringLiteral("emoji") << emojiRef;
    }
    {
        Emoji emojiRef;
        emojiRef.setExtension(QStringLiteral("gif"));
        emojiRef.setName(QStringLiteral("clapping"));
        emojiRef.setIdentifier(QStringLiteral("scSbxNPzm9xWrNqCG"));
        emojiRef.setAliases(QStringList{QStringLiteral(":clap:")});
        emojiRef.setEmojiIdentifier(QStringLiteral(":clapping:"));
        emojiRef.setUpdatedAt(1514915356313);
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
    originalEmoji.parseEmoji(obj, false);
    const bool emojiIsEqual = (originalEmoji == expectedEmoji);
    if (!emojiIsEqual) {
        qDebug() << "originalEmoji " << originalEmoji;
        qDebug() << "ExpectedEmoji " << expectedEmoji;
    }
    QVERIFY(emojiIsEqual);
}

void EmojiTest::shouldGenerateHtml_data()
{
    QTest::addColumn<Emoji>("emoji");
    QTest::addColumn<QString>("serverUrl");
    QTest::addColumn<QString>("html");

    {
        Emoji emojiRef;
        emojiRef.setExtension(QStringLiteral("gif"));
        emojiRef.setName(QStringLiteral("clapping"));
        emojiRef.setIdentifier(QStringLiteral("scSbxNPzm9xWrNqCG"));
        emojiRef.setAliases(QStringList{QStringLiteral("clap")});
        emojiRef.setEmojiIdentifier(QStringLiteral(":clapping:"));
        emojiRef.setUpdatedAt(1514915356313);
        QTest::addRow("emoji") << emojiRef << QStringLiteral("www.kde.org") << QStringLiteral("<img height='22' width='22' src='http://www.kde.org/emoji-custom/clapping.gif'/>");
    }
}

void EmojiTest::shouldGenerateHtml()
{
    QFETCH(Emoji, emoji);
    QFETCH(QString, serverUrl);
    QFETCH(QString, html);
    QCOMPARE(emoji.html(serverUrl), html);
    QCOMPARE(emoji.cachedHtml(), html);
}
