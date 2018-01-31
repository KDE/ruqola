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

#include "emojimanagertest.h"
#include "emojimanager.h"
#include "emoji.h"
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
    EmojiManager manager;
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
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QStringLiteral("/json/") + name + QStringLiteral(".json");
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject obj = doc.object();
    EmojiManager manager;
    manager.loadEmoji(obj);
    QCOMPARE(manager.count(), number);
}

void EmojiManagerTest::shouldGenerateHtml()
{
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QStringLiteral("/json/") + QStringLiteral("emojiparent") + QStringLiteral(".json");
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject obj = doc.object();
    EmojiManager manager;
    manager.loadEmoji(obj);
    //No serverUrl set.
    QCOMPARE(manager.html(QStringLiteral(":foo:")), QStringLiteral(":foo:"));

    const QString serverUrl = QStringLiteral("www.kde.org");
    manager.setServerUrl(serverUrl);

    //:foo: doesn't exist
    QCOMPARE(manager.html(QStringLiteral(":foo:")), QStringLiteral(":foo:"));

    //It exists
    QCOMPARE(manager.html(QStringLiteral(":react_rocket:")), QStringLiteral("<img height='22' width='22' src='http://www.kde.org/emoji-custom/react_rocket.png'/>"));


    QCOMPARE(manager.html(QStringLiteral(":totoro:")), QStringLiteral("<img height='22' width='22' src='http://www.kde.org/emoji-custom/totoro.gif'/>"));

    //Test aliases
    QCOMPARE(manager.html(QStringLiteral(":clap:")), QStringLiteral("<img height='22' width='22' src='http://www.kde.org/emoji-custom/clapping.gif'/>"));
    QCOMPARE(manager.html(QStringLiteral(":clapping:")), QStringLiteral("<img height='22' width='22' src='http://www.kde.org/emoji-custom/clapping.gif'/>"));
}

void EmojiManagerTest::shouldChangeServerUrl()
{
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QStringLiteral("/json/") + QStringLiteral("emojiparent") + QStringLiteral(".json");
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject obj = doc.object();
    EmojiManager manager;
    manager.loadEmoji(obj);
    QString serverUrl = QStringLiteral("www.kde.org");
    manager.setServerUrl(serverUrl);

    //It exists
    QCOMPARE(manager.html(QStringLiteral(":react_rocket:")), QStringLiteral("<img height='22' width='22' src='http://%1/emoji-custom/react_rocket.png'/>").arg(serverUrl));

    //Change server url => clear cache
    serverUrl = QStringLiteral("www.bla.org");
    manager.setServerUrl(serverUrl);
    QCOMPARE(manager.html(QStringLiteral(":react_rocket:")), QStringLiteral("<img height='22' width='22' src='http://%1/emoji-custom/react_rocket.png'/>").arg(serverUrl));

}
