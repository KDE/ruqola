/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customemojitest.h"
#include "emoticons/customemoji.h"
#include "ruqola_autotest_helper.h"
#include <QJsonObject>

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
    const QString ext{u"foo"_s};
    const QString id{u"bla"_s};
    const QString name{u"bli"_s};
    const QStringList aliases{u":foo:"_s, u":bla:"_s};
    const qint64 updatedAt = 500;

    j.setExtension(ext);
    j.setName(name);
    j.setIdentifier(id);
    j.setAliases(aliases);
    j.setUpdatedAt(updatedAt);

    const QString emojiId = u':' + name + u':';
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
    const QString ext{u"foo"_s};
    const QString id{u"bla"_s};
    const QString name{u"bli"_s};
    const QStringList aliases{u":foo:"_s, u":bla:"_s};
    const qint64 updatedAt = 500;

    j.setExtension(ext);
    j.setName(name);
    j.setIdentifier(id);
    j.setAliases(aliases);
    j.setUpdatedAt(updatedAt);
    const QString emojiId = u':' + name + u':';
    j.setEmojiIdentifier(emojiId);

    CustomEmoji copy = j;
    QCOMPARE(copy, j);
}

// void CustomEmojiTest::shouldClearCachedHtml()
//{
//    CustomEmoji emojiRef;
//    emojiRef.setExtension(u"gif"_s);
//    emojiRef.setName(u"clapping"_s);
//    emojiRef.setIdentifier(u"scSbxNPzm9xWrNqCG"_s);
//    emojiRef.setAliases(QStringList{u":clap:"_s});
//    emojiRef.setEmojiIdentifier(u":clapping:"_s);
//    emojiRef.setUpdatedAt(50);

//    const QString cachedHtml = emojiRef.generateHtmlFromCustomEmoji(u"www.kde.org"_s);
//    QVERIFY(!cachedHtml.isEmpty());
//    QCOMPARE(emojiRef.cachedHtml(), cachedHtml);
//    emojiRef.clearCachedHtml();
//    QVERIFY(emojiRef.cachedHtml().isEmpty());
//    QCOMPARE(emojiRef.generateHtmlFromCustomEmoji(u"www.kde.org"_s), cachedHtml);
//    QCOMPARE(emojiRef.cachedHtml(), cachedHtml);
//}

void CustomEmojiTest::shouldParseEmoji_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<CustomEmoji>("expectedEmoji");
    {
        CustomEmoji emojiRef;
        emojiRef.setExtension(u"jpg"_s);
        emojiRef.setName(u"troll"_s);
        emojiRef.setIdentifier(u"2cgzHwKP6Cq3iZCob"_s);
        emojiRef.setEmojiIdentifier(u":troll:"_s);
        emojiRef.setUpdatedAt(1485546740427);

        QTest::addRow("emoji") << u"emoji"_s << emojiRef;
    }
}

void CustomEmojiTest::shouldParseEmoji()
{
    QFETCH(QString, name);
    QFETCH(CustomEmoji, expectedEmoji);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + QLatin1StringView("/json/restapi/") + name + QLatin1StringView(".json");
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);
    CustomEmoji originalEmoji;
    originalEmoji.parseEmoji(obj);
    const bool emojiIsEqual = (originalEmoji == expectedEmoji);
    if (!emojiIsEqual) {
        qDebug() << "originalEmoji " << originalEmoji;
        qDebug() << "ExpectedEmoji " << expectedEmoji;
    }
    QVERIFY(emojiIsEqual);
}

// void CustomEmojiTest::shouldGenerateHtml_data()
//{
//    QTest::addColumn<CustomEmoji>("emoji");
//    QTest::addColumn<QString>("serverUrl");
//    QTest::addColumn<QString>("expectedFileName");
//    QTest::addColumn<QString>("expectedUrl");
//    QTest::addColumn<QString>("html");

//    {
//        CustomEmoji emojiRef;
//        emojiRef.setExtension(u"gif"_s);
//        emojiRef.setName(u"clapping"_s);
//        emojiRef.setIdentifier(u"scSbxNPzm9xWrNqCG"_s);
//        emojiRef.setAliases(QStringList{u"clap"_s});
//        emojiRef.setEmojiIdentifier(u":clapping:"_s);
//        emojiRef.setUpdatedAt(1514915356313);
//        QTest::addRow("emoji") << emojiRef << u"www.kde.org"_s
//                               << u"/emoji-custom/clapping.gif"_s
//                               << u"http://www.kde.org/emoji-custom/clapping.gif"_s
//                               << u"<img height='22' width='22' src='http://www.kde.org/emoji-custom/clapping.gif' title=':clapping:'/>"_s;
//    }
//}

// void CustomEmojiTest::shouldGenerateHtml()
//{
//    QFETCH(CustomEmoji, emoji);
//    QFETCH(QString, serverUrl);
//    QFETCH(QString, expectedFileName);
//    QFETCH(QString, expectedUrl);
//    QFETCH(QString, html);

//    QCOMPARE(emoji.emojiFileName(), expectedFileName);
//    QCOMPARE(emoji.emojiUrl(serverUrl), expectedUrl);
//}

#include "moc_customemojitest.cpp"
