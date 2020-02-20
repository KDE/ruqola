/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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

#include "emojimanagertest.h"
#include "emoticons/emojimanager.h"
#include "emoticons/emoji.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QTest>
QTEST_GUILESS_MAIN(EmojiManagerTest)

EmojiManagerTest::EmojiManagerTest(QObject *parent)
    : QObject(parent)
{
}

void EmojiManagerTest::shouldHaveDefaultValue()
{
    EmojiManager manager(nullptr, false);
    QVERIFY(manager.serverUrl().isEmpty());
    QCOMPARE(manager.count(), 0);
}

void EmojiManagerTest::shouldParseEmoji_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("number");
    QTest::addRow("emojiparent") << QStringLiteral("emojiparent") << 7;
}

void EmojiManagerTest::shouldParseEmoji()
{
    QFETCH(QString, name);
    QFETCH(int, number);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/json/restapi/") + name + QLatin1String(".json");
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject obj = doc.object();
    EmojiManager manager(nullptr, false);
    manager.loadCustomEmoji(obj);
    QCOMPARE(manager.count(), number);
}

void EmojiManagerTest::shouldSupportUnicodeEmojis()
{
    EmojiManager manager;
    QString grinning;
    grinning += QChar(0xd800+61);
    grinning += QChar(0xDC00+512);
    // A basic emoji that was already there in the initial emoji.json from fairchat
    QCOMPARE(manager.unicodeEmoticonForEmoji(QStringLiteral(":grinning:")).unicode(), grinning);
    // The one that made me use https://raw.githubusercontent.com/joypixels/emoji-toolkit/master/emoji.json instead
    QVERIFY(manager.unicodeEmoticonForEmoji(QStringLiteral(":zany_face:")).isValid());
    // A "shortname alternate"
    QCOMPARE(manager.unicodeEmoticonForEmoji(QStringLiteral(":water_polo_tone5:")).key(), QStringLiteral("1f93d-1f3ff"));
    // One with multiple values below 0x10000, to catch += vs = bug.
    QCOMPARE(manager.unicodeEmoticonForEmoji(QStringLiteral(":woman_climbing_tone4:")).unicode().length(), 7);
    QCOMPARE(manager.unicodeEmoticonForEmoji(QStringLiteral(":man_health_worker_tone1:")).unicode().length(), 7);
}

void EmojiManagerTest::shouldOrderUnicodeEmojis()
{
    EmojiManager manager;
    QMap<QString, QVector<UnicodeEmoticon>> map = manager.unicodeEmojiList();
    QVERIFY(map.contains(QStringLiteral("activity")));
    const QVector<UnicodeEmoticon> symbols = map.value(QStringLiteral("symbols"));
    QVERIFY(!symbols.isEmpty());
    QCOMPARE(symbols.at(0).order(), 1);
    QCOMPARE(symbols.at(0).identifier(), QStringLiteral(":heart:"));
    const QVector<UnicodeEmoticon> regional = map.value(QStringLiteral("regional"));
    QVERIFY(!regional.isEmpty());
    QCOMPARE(regional.at(0).identifier(), QStringLiteral(":regional_indicator_z:")); // letters are reversed, weird
}

void EmojiManagerTest::shouldGenerateHtml()
{
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/json/restapi/emojiparent.json");
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject obj = doc.object();
    EmojiManager manager(nullptr, false);
    manager.loadCustomEmoji(obj);
    //No serverUrl set.
    QCOMPARE(manager.replaceEmojiIdentifier(QStringLiteral(":foo:")), QStringLiteral(":foo:"));

    const QString serverUrl = QStringLiteral("www.kde.org");
    manager.setServerUrl(serverUrl);

    // :foo: doesn't exist
    QCOMPARE(manager.replaceEmojiIdentifier(QStringLiteral(":foo:")), QStringLiteral(":foo:"));
    QCOMPARE(manager.customEmojiFileName(QStringLiteral(":foo:")), QString());

    // Existing emoji
    QCOMPARE(manager.replaceEmojiIdentifier(QStringLiteral(":vader:")), QStringLiteral("<img height='22' width='22' src='http://www.kde.org/emoji-custom/vader.png'/>"));
    QCOMPARE(manager.customEmojiFileName(QStringLiteral(":vader:")), QStringLiteral("/emoji-custom/vader.png"));

    // Alias
    QCOMPARE(manager.replaceEmojiIdentifier(QStringLiteral(":darth:")), QStringLiteral("<img height='22' width='22' src='http://www.kde.org/emoji-custom/vader.png'/>"));
    QCOMPARE(manager.customEmojiFileName(QStringLiteral(":darth:")), QStringLiteral("/emoji-custom/vader.png"));
}

void EmojiManagerTest::shouldChangeServerUrl()
{
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/json/restapi/emojiparent.json");
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject obj = doc.object();
    EmojiManager manager(nullptr, false);
    manager.loadCustomEmoji(obj);
    QString serverUrl = QStringLiteral("www.kde.org");
    manager.setServerUrl(serverUrl);

    //It exists
    QCOMPARE(manager.replaceEmojiIdentifier(QStringLiteral(":vader:")), QStringLiteral("<img height='22' width='22' src='http://%1/emoji-custom/vader.png'/>").arg(serverUrl));

    //Change server url => clear cache
    serverUrl = QStringLiteral("www.bla.org");
    manager.setServerUrl(serverUrl);
    QCOMPARE(manager.replaceEmojiIdentifier(QStringLiteral(":vader:")), QStringLiteral("<img height='22' width='22' src='http://%1/emoji-custom/vader.png'/>").arg(serverUrl));
}
