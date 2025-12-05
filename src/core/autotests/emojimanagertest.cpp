/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emojimanagertest.h"
using namespace Qt::Literals::StringLiterals;

#include "emoticons/customemoji.h"
#include "emoticons/emojimanager.h"
#include "rocketchataccount.h"
#include "ruqola_autotest_helper.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QSignalSpy>
#include <QTest>

#include <TextEmoticonsCore/UnicodeEmoticonManager>
QTEST_GUILESS_MAIN(EmojiManagerTest)

EmojiManagerTest::EmojiManagerTest(QObject *parent)
    : QObject(parent)
{
}

void EmojiManagerTest::shouldHaveDefaultValue()
{
    EmojiManager manager(nullptr);
    QVERIFY(manager.serverUrl().isEmpty());
    QCOMPARE(manager.count(), 3820);
    QVERIFY(manager.customEmojiList().isEmpty());
}

void EmojiManagerTest::shouldParseEmoji_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("number");
    QTest::addRow("emojiparent") << u"emojiparent"_s << 3827;
}

void EmojiManagerTest::shouldParseEmoji()
{
    QFETCH(QString, name);
    QFETCH(int, number);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/json/restapi/"_L1 + name + ".json"_L1;
    auto obj = AutoTestHelper::loadJsonObject(originalJsonFile);
    EmojiManager manager(nullptr);
    manager.loadCustomEmoji(obj);
    QCOMPARE(manager.count(), number);
}

void EmojiManagerTest::shouldDeleteEmojiCustom_data()
{
    QTest::addColumn<QString>("initialListName");
    QTest::addColumn<int>("number");
    QTest::addColumn<QString>("deleteName");
    QTest::addColumn<QList<CustomEmoji>>("original");
    QTest::addColumn<QList<CustomEmoji>>("customEmoji");
    {
        QList<CustomEmoji> emojiList;
        CustomEmoji val;
        val.setName(u"kdab"_s);
        val.setIdentifier("RyBauhQqnoE5WeJvZ"_ba);
        val.setExtension(u"png"_s);
        val.setEmojiIdentifier(u":kdab:"_s);
        val.setUpdatedAt(1529303015003);
        emojiList.append(std::move(val));

        CustomEmoji val1;
        val1.setName(u"vader"_s);
        val1.setIdentifier("fAiQmJnJPAaEFmps6"_ba);
        val1.setExtension(u"png"_s);
        val1.setEmojiIdentifier(u":vader:"_s);
        val1.setUpdatedAt(1560497261506);
        val1.setAliases({u":darth:"_s});
        emojiList.append(val1);

        QList<CustomEmoji> emojiListAfterDeleting;
        emojiListAfterDeleting.append(val1);

        QTest::addRow("delete1") << u"emojiparent2"_s << 3822 << u"emojicustomdelete1"_s << emojiList << emojiListAfterDeleting;
    }
    {
        QList<CustomEmoji> emojiList;
        CustomEmoji val;
        val.setName(u"kdab"_s);
        val.setIdentifier("RyBauhQqnoE5WeJvZ"_ba);
        val.setExtension(u"png"_s);
        val.setEmojiIdentifier(u":kdab:"_s);
        val.setUpdatedAt(1529303015003);

        CustomEmoji val1;
        val1.setName(u"vader"_s);
        val1.setIdentifier("fAiQmJnJPAaEFmps6"_ba);
        val1.setExtension(u"png"_s);
        val1.setEmojiIdentifier(u":vader:"_s);
        val1.setUpdatedAt(1560497261506);
        val1.setAliases({u":darth:"_s});

        emojiList.append(std::move(val));
        emojiList.append(std::move(val1));
        // We can't delete emoji which is not in liste.
        QTest::addRow("delete2") << u"emojiparent2"_s << 3822 << u"emojicustomdelete2"_s << emojiList << emojiList;
    }
}

void EmojiManagerTest::shouldDeleteEmojiCustom()
{
    QFETCH(QString, initialListName);
    QFETCH(int, number);
    QFETCH(QString, deleteName);
    QFETCH(QList<CustomEmoji>, original);
    QFETCH(QList<CustomEmoji>, customEmoji);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/json/restapi/"_L1 + initialListName + ".json"_L1;
    const auto obj = AutoTestHelper::loadJsonObject(originalJsonFile);
    EmojiManager manager(nullptr);
    manager.loadCustomEmoji(obj);
    QCOMPARE(manager.count(), number);
    QCOMPARE(manager.customEmojiList(), original);

    const QString deleteJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/json/restapi/"_L1 + deleteName + ".json"_L1;
    const auto objDelete = AutoTestHelper::loadJsonArrayObject(deleteJsonFile);
    manager.deleteEmojiCustom(objDelete);
    // qDebug() << " manager.customEmojiList() " << manager.customEmojiList();
    QCOMPARE(manager.customEmojiList(), customEmoji);
}

void EmojiManagerTest::shouldAddEmojiCustom_data()
{
    QTest::addColumn<QString>("initialListName");
    QTest::addColumn<int>("number");
    QTest::addColumn<QString>("addName");
    QTest::addColumn<QList<CustomEmoji>>("original");
    QTest::addColumn<QList<CustomEmoji>>("customEmoji");

    {
        QList<CustomEmoji> emojiList;
        CustomEmoji val;
        val.setName(u"kdab"_s);
        val.setIdentifier("RyBauhQqnoE5WeJvZ"_ba);
        val.setExtension(u"png"_s);
        val.setEmojiIdentifier(u":kdab:"_s);
        val.setUpdatedAt(1529303015003);

        CustomEmoji val1;
        val1.setName(u"vader"_s);
        val1.setIdentifier("fAiQmJnJPAaEFmps6"_ba);
        val1.setExtension(u"png"_s);
        val1.setEmojiIdentifier(u":vader:"_s);
        val1.setUpdatedAt(1560497261506);
        val1.setAliases({u":darth:"_s});

        emojiList.append(std::move(val));
        emojiList.append(std::move(val1));

        QList<CustomEmoji> emojiListAfterDeleting = emojiList;
        CustomEmoji val2;
        val2.setName(u"ruqola"_s);
        val2.setIdentifier("HdN28k4PQ6J9xLkZ8"_ba);
        val2.setExtension(u"png"_s);
        val2.setEmojiIdentifier(u":ruqola:"_s);
        val2.setUpdatedAt(1631885946222);
        val2.setAliases({u":roo:"_s});
        emojiList.append(std::move(val2));
        QTest::addRow("emojiparent2") << u"emojiparent2"_s << 3822 << u"addemojicustom1"_s << emojiListAfterDeleting << emojiList;
    }
}

void EmojiManagerTest::shouldAddEmojiCustom()
{
    QFETCH(QString, initialListName);
    QFETCH(int, number);
    QFETCH(QString, addName);
    QFETCH(QList<CustomEmoji>, original);
    QFETCH(QList<CustomEmoji>, customEmoji);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/json/restapi/"_L1 + initialListName + ".json"_L1;
    auto obj = AutoTestHelper::loadJsonObject(originalJsonFile);
    EmojiManager manager(nullptr);
    QSignalSpy customEmojiChanged(&manager, &EmojiManager::customEmojiChanged);
    manager.loadCustomEmoji(obj);
    QCOMPARE(manager.count(), number);

    QCOMPARE(manager.customEmojiList(), original);

    const QString addJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/json/restapi/"_L1 + addName + ".json"_L1;
    const auto objAdd = AutoTestHelper::loadJsonArrayObject(addJsonFile);
    manager.addUpdateEmojiCustomList(objAdd);
    QCOMPARE(customEmojiChanged.count(), 1);
    QCOMPARE(customEmojiChanged.at(0).at(0).toBool(), true);

    // qDebug() << " BEFORE " << customEmoji;
    // qDebug() << " manager.customEmojiList() " << manager.customEmojiList();
    //    QCOMPARE(manager.customEmojiList(), customEmoji);
}

void EmojiManagerTest::shouldUpdateEmojiCustom_data()
{
    QTest::addColumn<QString>("initialListName");
    QTest::addColumn<int>("number");
    QTest::addColumn<QString>("addName");
    QTest::addColumn<QList<CustomEmoji>>("original");
    QTest::addColumn<QList<CustomEmoji>>("customEmoji");
    {
        QList<CustomEmoji> emojiList;
        CustomEmoji val;
        val.setName(u"kdab"_s);
        val.setIdentifier("RyBauhQqnoE5WeJvZ"_ba);
        val.setExtension(u"png"_s);
        val.setEmojiIdentifier(u":kdab:"_s);
        val.setUpdatedAt(1529303015003);

        CustomEmoji val1;
        val1.setName(u"vader"_s);
        val1.setIdentifier("fAiQmJnJPAaEFmps6"_ba);
        val1.setExtension(u"png"_s);
        val1.setEmojiIdentifier(u":vader:"_s);
        val1.setUpdatedAt(1560497261506);
        val1.setAliases({u":darth:"_s});

        emojiList.append(val);
        emojiList.append(std::move(val1));

        QList<CustomEmoji> emojiListAfterDeleting;
        CustomEmoji val2;
        val2.setName(u"ruqola"_s);
        val2.setIdentifier("RyBauhQqnoE5WeJvZ"_ba);
        val2.setExtension(u"png"_s);
        val2.setEmojiIdentifier(u":ruqola:"_s);
        val2.setUpdatedAt(1631885946222);
        val2.setAliases({u"rooss"_s});
        emojiListAfterDeleting.append(val);
        emojiListAfterDeleting.append(std::move(val2));
        QTest::addRow("emojiparent2") << u"emojiparent2"_s << 3822 << u"updateemojicustom1"_s << emojiList << emojiList;
    }
}

void EmojiManagerTest::shouldUpdateEmojiCustom()
{
    QFETCH(QString, initialListName);
    QFETCH(int, number);
    QFETCH(QString, addName);
    QFETCH(QList<CustomEmoji>, original);
    QFETCH(QList<CustomEmoji>, customEmoji);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/json/restapi/"_L1 + initialListName + ".json"_L1;
    auto obj = AutoTestHelper::loadJsonObject(originalJsonFile);
    EmojiManager manager(nullptr);
    QSignalSpy customEmojiChanged(&manager, &EmojiManager::customEmojiChanged);
    manager.loadCustomEmoji(obj);

    QCOMPARE(manager.count(), number);
    QCOMPARE(manager.customEmojiList(), original);

    const QString addJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/json/restapi/"_L1 + addName + ".json"_L1;
    const auto objAdd = AutoTestHelper::loadJsonArrayObject(addJsonFile);
    manager.addUpdateEmojiCustomList(objAdd);
    QCOMPARE(customEmojiChanged.count(), 1);
    QCOMPARE(customEmojiChanged.at(0).at(0).toBool(), false);
}

void EmojiManagerTest::shouldSupportUnicodeEmojis()
{
    // Load list of unicode emoticon
    TextEmoticonsCore::UnicodeEmoticonManager::self();
    EmojiManager manager(nullptr);
    QString grinning;
    grinning += QChar(0xd800 + 61);
    grinning += QChar(0xDC00 + 512);
    // A basic emoji that was already there in the initial emoji.json from fairchat
    QCOMPARE(manager.unicodeEmoticonForEmoji(u":grinning:"_s).unicode(), grinning);
    // The one that made me use https://raw.githubusercontent.com/joypixels/emoji-toolkit/master/emoji.json instead
    QVERIFY(manager.unicodeEmoticonForEmoji(u":zany_face:"_s).isValid());
    // A "shortname alternate"
    QCOMPARE(manager.unicodeEmoticonForEmoji(u":water_polo_tone5:"_s).key(), u"1f93d-1f3ff"_s);
    // One with multiple values below 0x10000, to catch += vs = bug.
    QCOMPARE(manager.unicodeEmoticonForEmoji(u":woman_climbing_tone4:"_s).unicode().length(), 7);
    QCOMPARE(manager.unicodeEmoticonForEmoji(u":man_health_worker_tone1:"_s).unicode().length(), 7);

    // a smiley with shortname alternates and ascii versions
    QString slightSmile;
    slightSmile += QChar(0xD83D);
    slightSmile += QChar(0xDE42);
    QCOMPARE(manager.unicodeEmoticonForEmoji(u":slight_smile:"_s).unicode(), slightSmile);
    // alternate
    QCOMPARE(manager.unicodeEmoticonForEmoji(u":slightly_smiling_face:"_s).unicode(), slightSmile);
    // ascii
    QCOMPARE(manager.unicodeEmoticonForEmoji(u":)"_s).unicode(), slightSmile);
    QCOMPARE(manager.unicodeEmoticonForEmoji(u":-)"_s).unicode(), slightSmile);

    QCOMPARE(manager.categories().count(), 9);
    QCOMPARE(manager.categories().at(0).category(), u"people"_s);
    QCOMPARE(manager.emojisForCategory(u"people"_s).count(), 2158);
}

void EmojiManagerTest::shouldOrderUnicodeEmojis()
{
    // Load list of unicode emoticon
    TextEmoticonsCore::UnicodeEmoticonManager::self();
    EmojiManager manager(nullptr);
    const QList<TextEmoticonsCore::UnicodeEmoticon> list = manager.unicodeEmojiList();
    auto hasCategory = [](const QString &category) {
        return [category](const TextEmoticonsCore::UnicodeEmoticon &emo) {
            return emo.category() == category;
        };
    };
    // Check what's the first emoji in the category "symbols"
    auto it = std::find_if(list.begin(), list.end(), hasCategory(u"symbols"_s));
    QVERIFY(it != list.end());
    const TextEmoticonsCore::UnicodeEmoticon firstSymbol = *it;
    QCOMPARE(firstSymbol.order(), 3207);
    QCOMPARE(firstSymbol.category(), u"symbols"_s);
    QCOMPARE(firstSymbol.identifier(), u":pink_heart:"_s);

    // Check what's the first emoji in the category "regional"
    it = std::find_if(list.begin(), list.end(), hasCategory(u"regional"_s));
    QVERIFY(it != list.end());
    const TextEmoticonsCore::UnicodeEmoticon firstRegional = *it;
    QCOMPARE(firstRegional.identifier(), u":regional_indicator_z:"_s); // letters are reversed, weird
}

void EmojiManagerTest::shouldGenerateHtml()
{
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/json/restapi/emojiparent.json"_L1;
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject obj = doc.object();
    EmojiManager manager(nullptr);
    manager.loadCustomEmoji(obj);
    // No serverUrl set.
    QCOMPARE(manager.replaceEmojiIdentifier(u":foo:"_s), u":foo:"_s);

    const QString serverUrl = u"www.kde.org"_s;
    manager.setServerUrl(serverUrl);

    // :foo: doesn't exist
    QCOMPARE(manager.replaceEmojiIdentifier(u":foo:"_s), u":foo:"_s);
    QCOMPARE(manager.customEmojiFileName(u":foo:"_s), QString());

    //    // Existing emoji
    //    QCOMPARE(manager.replaceEmojiIdentifier(u":vader:"_s), QStringLiteral("<img height='22' width='22'
    //    src='http://www.kde.org/emoji-custom/vader.png' title=':vader:'/>")); QCOMPARE(manager.customEmojiFileName(u":vader:"_s),
    //    u"/emoji-custom/vader.png"_s);

    //    // Alias
    //    QCOMPARE(manager.replaceEmojiIdentifier(u":darth:"_s), QStringLiteral("<img height='22' width='22'
    //    src='http://www.kde.org/emoji-custom/vader.png' title=':vader:'/>")); QCOMPARE(manager.customEmojiFileName(u":darth:"_s),
    //    u"/emoji-custom/vader.png"_s);
}

void EmojiManagerTest::shouldChangeServerUrl()
{
    //    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/json/restapi/emojiparent.json"_L1;
    //    QFile f(originalJsonFile);
    //    QVERIFY(f.open(QIODevice::ReadOnly));
    //    const QByteArray content = f.readAll();
    //    f.close();
    //    const QJsonDocument doc = QJsonDocument::fromJson(content);
    //    const QJsonObject obj = doc.object();
    //    EmojiManager manager(nullptr);
    //    manager.loadCustomEmoji(obj);
    //    QString serverUrl = u"www.kde.org"_s;
    //    manager.setServerUrl(serverUrl);

    //    //It exists
    //    QCOMPARE(manager.replaceEmojiIdentifier(u":vader:"_s), QStringLiteral("<img height='22' width='22' src='http://%1/emoji-custom/vader.png'
    //    title=':vader:'/>").arg(serverUrl));

    //    //Change server url => clear cache
    //    serverUrl = u"www.bla.org"_s;
    //    manager.setServerUrl(serverUrl);
    //    QCOMPARE(manager.replaceEmojiIdentifier(u":vader:"_s), QStringLiteral("<img height='22' width='22' src='http://%1/emoji-custom/vader.png'
    //    title=':vader:'/>").arg(serverUrl));
}

void EmojiManagerTest::shouldNormalizeReactions_data()
{
    QTest::addColumn<QString>("emoji");
    QTest::addColumn<QString>("normalizedEmoji");

    QTest::addRow(":)") << ":)"
                        << ":slight_smile:";
    QTest::addRow(":slight_simle:") << ":slight_smile:"
                                    << ":slight_smile:";
    QString slightSmile;
    slightSmile += QChar(0xD83D);
    slightSmile += QChar(0xDE42);
    QTest::addRow("unicode-smile") << slightSmile << ":slight_smile:";
    QTest::addRow(":vader:") << ":vader:"
                             << ":vader:";
}

void EmojiManagerTest::shouldNormalizeReactions()
{
    QFETCH(QString, emoji);
    QFETCH(QString, normalizedEmoji);

    EmojiManager manager(nullptr);
    QCOMPARE(manager.normalizedReactionEmoji(emoji), normalizedEmoji);
}

void EmojiManagerTest::replaceAsciiEmoji_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<bool>("replaced");

    QTest::addRow("1") << "hello :)" << true;
    QTest::addRow("2") << "Item::Lock" << false;
    QTest::addRow("3") << ":)" << true;
    QTest::addRow("3") << ":)what" << true;
}

void EmojiManagerTest::replaceAsciiEmoji()
{
    QFETCH(QString, input);
    QFETCH(bool, replaced);
    QString original = input;

    TextEmoticonsCore::UnicodeEmoticonManager::self();
    RocketChatAccount account;
    account.ownUserPreferences().setConvertAsciiEmoji(true);
    EmojiManager manager(&account);
    manager.setServerUrl(u"blah blah"_s);
    manager.replaceEmojis(&input);

    QCOMPARE(input != original, replaced);
}

#include "moc_emojimanagertest.cpp"
