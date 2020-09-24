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

#include "customemojitest.h"
#include "emoticons/customemoji.h"
#include <QJsonObject>
#include <QTest>
#include <QJsonDocument>

QTEST_GUILESS_MAIN(CustomEmojiTest)

CustomEmojiTest::CustomEmojiTest(QObject *parent)
    : QObject(parent)
{
}

void CustomEmojiTest::shouldHaveDefaultValue()
{
    CustomEmoji j;
    QVERIFY(j.extension().isEmpty());
    QVERIFY(j.identifier().isEmpty());
    QVERIFY(j.name().isEmpty());
    QVERIFY(j.aliases().isEmpty());
    QCOMPARE(j.updatedAt(), 0);
    QVERIFY(j.emojiIdentifier().isEmpty());
    QCOMPARE(j.isAnimatedImage(), false);
}

void CustomEmojiTest::shouldAssignValue()
{
    CustomEmoji j;
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

void CustomEmojiTest::shouldCopyValue()
{
    CustomEmoji j;
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

    CustomEmoji copy = j;
    QCOMPARE(copy, j);
}

//void CustomEmojiTest::shouldClearCachedHtml()
//{
//    CustomEmoji emojiRef;
//    emojiRef.setExtension(QStringLiteral("gif"));
//    emojiRef.setName(QStringLiteral("clapping"));
//    emojiRef.setIdentifier(QStringLiteral("scSbxNPzm9xWrNqCG"));
//    emojiRef.setAliases(QStringList{QStringLiteral(":clap:")});
//    emojiRef.setEmojiIdentifier(QStringLiteral(":clapping:"));
//    emojiRef.setUpdatedAt(50);

//    const QString cachedHtml = emojiRef.generateHtmlFromCustomEmoji(QStringLiteral("www.kde.org"));
//    QVERIFY(!cachedHtml.isEmpty());
//    QCOMPARE(emojiRef.cachedHtml(), cachedHtml);
//    emojiRef.clearCachedHtml();
//    QVERIFY(emojiRef.cachedHtml().isEmpty());
//    QCOMPARE(emojiRef.generateHtmlFromCustomEmoji(QStringLiteral("www.kde.org")), cachedHtml);
//    QCOMPARE(emojiRef.cachedHtml(), cachedHtml);
//}

void CustomEmojiTest::shouldParseEmoji_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<CustomEmoji>("expectedEmoji");
    {
        CustomEmoji emojiRef;
        emojiRef.setExtension(QStringLiteral("jpg"));
        emojiRef.setName(QStringLiteral("troll"));
        emojiRef.setIdentifier(QStringLiteral("2cgzHwKP6Cq3iZCob"));
        emojiRef.setEmojiIdentifier(QStringLiteral(":troll:"));
        emojiRef.setUpdatedAt(1485546740427);

        QTest::addRow("emoji") << QStringLiteral("emoji") << emojiRef;
    }
}

void CustomEmojiTest::shouldParseEmoji()
{
    QFETCH(QString, name);
    QFETCH(CustomEmoji, expectedEmoji);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/json/restapi/") + name + QLatin1String(".json");
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject obj = doc.object();
    CustomEmoji originalEmoji;
    originalEmoji.parseEmoji(obj);
    const bool emojiIsEqual = (originalEmoji == expectedEmoji);
    if (!emojiIsEqual) {
        qDebug() << "originalEmoji " << originalEmoji;
        qDebug() << "ExpectedEmoji " << expectedEmoji;
    }
    QVERIFY(emojiIsEqual);
}

//void CustomEmojiTest::shouldGenerateHtml_data()
//{
//    QTest::addColumn<CustomEmoji>("emoji");
//    QTest::addColumn<QString>("serverUrl");
//    QTest::addColumn<QString>("expectedFileName");
//    QTest::addColumn<QString>("expectedUrl");
//    QTest::addColumn<QString>("html");

//    {
//        CustomEmoji emojiRef;
//        emojiRef.setExtension(QStringLiteral("gif"));
//        emojiRef.setName(QStringLiteral("clapping"));
//        emojiRef.setIdentifier(QStringLiteral("scSbxNPzm9xWrNqCG"));
//        emojiRef.setAliases(QStringList{QStringLiteral("clap")});
//        emojiRef.setEmojiIdentifier(QStringLiteral(":clapping:"));
//        emojiRef.setUpdatedAt(1514915356313);
//        QTest::addRow("emoji") << emojiRef << QStringLiteral("www.kde.org")
//                               << QStringLiteral("/emoji-custom/clapping.gif")
//                               << QStringLiteral("http://www.kde.org/emoji-custom/clapping.gif")
//                               << QStringLiteral("<img height='22' width='22' src='http://www.kde.org/emoji-custom/clapping.gif' title=':clapping:'/>");
//    }
//}

//void CustomEmojiTest::shouldGenerateHtml()
//{
//    QFETCH(CustomEmoji, emoji);
//    QFETCH(QString, serverUrl);
//    QFETCH(QString, expectedFileName);
//    QFETCH(QString, expectedUrl);
//    QFETCH(QString, html);

//    QCOMPARE(emoji.emojiFileName(), expectedFileName);
//    QCOMPARE(emoji.emojiUrl(serverUrl), expectedUrl);
//}
