/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

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
    QTest::addRow("emojiparent") << QStringLiteral("emojiparent") << 40;
}

void EmojiManagerTest::shouldParseEmoji()
{
    QFETCH(QString, name);
    QFETCH(int, number);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/json/") + name + QLatin1String(".json");
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject obj = doc.object();
    EmojiManager manager(nullptr, false);
    manager.loadCustomEmoji(obj, false);
    QCOMPARE(manager.count(), number);
}

void EmojiManagerTest::shouldParseEmojiRestApi_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("number");
    QTest::addRow("emojiparent") << QStringLiteral("emojiparent") << 7;
}

void EmojiManagerTest::shouldParseEmojiRestApi()
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
    manager.loadCustomEmoji(obj, true);
    QCOMPARE(manager.count(), number);
}

void EmojiManagerTest::shouldGenerateHtml()
{
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/json/") + QLatin1String("emojiparent") + QLatin1String(".json");
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject obj = doc.object();
    EmojiManager manager;
    manager.loadCustomEmoji(obj, false);
    //No serverUrl set.
    QCOMPARE(manager.replaceEmojiIdentifier(QStringLiteral(":foo:")), QStringLiteral(":foo:"));

    const QString serverUrl = QStringLiteral("www.kde.org");
    manager.setServerUrl(serverUrl);

    //:foo: doesn't exist
    QCOMPARE(manager.replaceEmojiIdentifier(QStringLiteral(":foo:")), QStringLiteral(":foo:"));

    //It exists
    QCOMPARE(manager.replaceEmojiIdentifier(QStringLiteral(":react_rocket:")), QStringLiteral("<img height='22' width='22' src='http://www.kde.org/emoji-custom/react_rocket.png'/>"));

    QCOMPARE(manager.replaceEmojiIdentifier(QStringLiteral(":totoro:")), QStringLiteral("<img height='22' width='22' src='http://www.kde.org/emoji-custom/totoro.gif'/>"));

    //Test aliases
    QCOMPARE(manager.replaceEmojiIdentifier(QStringLiteral(":clap:")), QStringLiteral("<img height='22' width='22' src='http://www.kde.org/emoji-custom/clapping.gif'/>"));
    QCOMPARE(manager.replaceEmojiIdentifier(QStringLiteral(":clapping:")), QStringLiteral("<img height='22' width='22' src='http://www.kde.org/emoji-custom/clapping.gif'/>"));
}

void EmojiManagerTest::shouldChangeServerUrl()
{
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/json/") + QLatin1String("emojiparent") + QLatin1String(".json");
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject obj = doc.object();
    EmojiManager manager(nullptr, false);
    manager.loadCustomEmoji(obj, false);
    QString serverUrl = QStringLiteral("www.kde.org");
    manager.setServerUrl(serverUrl);

    //It exists
    QCOMPARE(manager.replaceEmojiIdentifier(QStringLiteral(":react_rocket:")), QStringLiteral("<img height='22' width='22' src='http://%1/emoji-custom/react_rocket.png'/>").arg(serverUrl));

    //Change server url => clear cache
    serverUrl = QStringLiteral("www.bla.org");
    manager.setServerUrl(serverUrl);
    QCOMPARE(manager.replaceEmojiIdentifier(QStringLiteral(":react_rocket:")), QStringLiteral("<img height='22' width='22' src='http://%1/emoji-custom/react_rocket.png'/>").arg(serverUrl));
}
