/*
   SPDX-FileCopyrightText: 2018-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emojimanagertest.h"
#include "emoticons/customemoji.h"
#include "emoticons/emojimanager.h"
#include "emoticons/unicodeemoticonmanager.h"
#include "rocketchataccount.h"
#include "ruqola_autotest_helper.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QSignalSpy>
#include <QTest>
QTEST_GUILESS_MAIN(EmojiManagerTest)

EmojiManagerTest::EmojiManagerTest(QObject *parent)
    : QObject(parent)
{
}

void EmojiManagerTest::shouldHaveDefaultValue()
{
    EmojiManager manager(nullptr);
    QVERIFY(manager.serverUrl().isEmpty());
    QCOMPARE(manager.count(), 3225);
    QVERIFY(manager.customEmojiList().isEmpty());
}

void EmojiManagerTest::shouldParseEmoji_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("number");
    QTest::addRow("emojiparent") << QStringLiteral("emojiparent") << 3232;
}

void EmojiManagerTest::shouldParseEmoji()
{
    QFETCH(QString, name);
    QFETCH(int, number);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/json/restapi/") + name + QLatin1String(".json");
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
    QTest::addColumn<QVector<CustomEmoji>>("original");
    QTest::addColumn<QVector<CustomEmoji>>("customEmoji");
    {
        QVector<CustomEmoji> emojiList;
        CustomEmoji val;
        val.setName(QStringLiteral("kdab"));
        val.setIdentifier(QStringLiteral("RyBauhQqnoE5WeJvZ"));
        val.setExtension(QStringLiteral("png"));
        val.setEmojiIdentifier(QStringLiteral(":kdab:"));
        val.setUpdatedAt(1529303015003);
        emojiList.append(val);

        CustomEmoji val1;
        val1.setName(QStringLiteral("vader"));
        val1.setIdentifier(QStringLiteral("fAiQmJnJPAaEFmps6"));
        val1.setExtension(QStringLiteral("png"));
        val1.setEmojiIdentifier(QStringLiteral(":vader:"));
        val1.setUpdatedAt(1560497261506);
        val1.setAliases({QStringLiteral(":darth:")});
        emojiList.append(val1);

        QVector<CustomEmoji> emojiListAfterDeleting;
        emojiListAfterDeleting.append(val1);

        QTest::addRow("delete1") << QStringLiteral("emojiparent2") << 3227 << QStringLiteral("emojicustomdelete1") << emojiList << emojiListAfterDeleting;
    }
    {
        QVector<CustomEmoji> emojiList;
        CustomEmoji val;
        val.setName(QStringLiteral("kdab"));
        val.setIdentifier(QStringLiteral("RyBauhQqnoE5WeJvZ"));
        val.setExtension(QStringLiteral("png"));
        val.setEmojiIdentifier(QStringLiteral(":kdab:"));
        val.setUpdatedAt(1529303015003);

        CustomEmoji val1;
        val1.setName(QStringLiteral("vader"));
        val1.setIdentifier(QStringLiteral("fAiQmJnJPAaEFmps6"));
        val1.setExtension(QStringLiteral("png"));
        val1.setEmojiIdentifier(QStringLiteral(":vader:"));
        val1.setUpdatedAt(1560497261506);
        val1.setAliases({QStringLiteral(":darth:")});

        emojiList.append(val);
        emojiList.append(val1);
        // We can't delete emoji which is not in liste.
        QTest::addRow("delete2") << QStringLiteral("emojiparent2") << 3227 << QStringLiteral("emojicustomdelete2") << emojiList << emojiList;
    }
}

void EmojiManagerTest::shouldDeleteEmojiCustom()
{
    QFETCH(QString, initialListName);
    QFETCH(int, number);
    QFETCH(QString, deleteName);
    QFETCH(QVector<CustomEmoji>, original);
    QFETCH(QVector<CustomEmoji>, customEmoji);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/json/restapi/") + initialListName + QLatin1String(".json");
    const auto obj = AutoTestHelper::loadJsonObject(originalJsonFile);
    EmojiManager manager(nullptr);
    manager.loadCustomEmoji(obj);
    QCOMPARE(manager.count(), number);
    QCOMPARE(manager.customEmojiList(), original);

    const QString deleteJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/json/restapi/") + deleteName + QLatin1String(".json");
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
    QTest::addColumn<QVector<CustomEmoji>>("original");
    QTest::addColumn<QVector<CustomEmoji>>("customEmoji");

    {
        QVector<CustomEmoji> emojiList;
        CustomEmoji val;
        val.setName(QStringLiteral("kdab"));
        val.setIdentifier(QStringLiteral("RyBauhQqnoE5WeJvZ"));
        val.setExtension(QStringLiteral("png"));
        val.setEmojiIdentifier(QStringLiteral(":kdab:"));
        val.setUpdatedAt(1529303015003);

        CustomEmoji val1;
        val1.setName(QStringLiteral("vader"));
        val1.setIdentifier(QStringLiteral("fAiQmJnJPAaEFmps6"));
        val1.setExtension(QStringLiteral("png"));
        val1.setEmojiIdentifier(QStringLiteral(":vader:"));
        val1.setUpdatedAt(1560497261506);
        val1.setAliases({QStringLiteral(":darth:")});

        emojiList.append(val);
        emojiList.append(val1);

        QVector<CustomEmoji> emojiListAfterDeleting = emojiList;
        CustomEmoji val2;
        val2.setName(QStringLiteral("ruqola"));
        val2.setIdentifier(QStringLiteral("HdN28k4PQ6J9xLkZ8"));
        val2.setExtension(QStringLiteral("png"));
        val2.setEmojiIdentifier(QStringLiteral(":ruqola:"));
        val2.setUpdatedAt(1631885946222);
        val2.setAliases({QStringLiteral(":roo:")});
        emojiList.append(val2);
        QTest::addRow("emojiparent2") << QStringLiteral("emojiparent2") << 3227 << QStringLiteral("addemojicustom1") << emojiListAfterDeleting << emojiList;
    }
}

void EmojiManagerTest::shouldAddEmojiCustom()
{
    QFETCH(QString, initialListName);
    QFETCH(int, number);
    QFETCH(QString, addName);
    QFETCH(QVector<CustomEmoji>, original);
    QFETCH(QVector<CustomEmoji>, customEmoji);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/json/restapi/") + initialListName + QLatin1String(".json");
    auto obj = AutoTestHelper::loadJsonObject(originalJsonFile);
    EmojiManager manager(nullptr);
    QSignalSpy customEmojiChanged(&manager, &EmojiManager::customEmojiChanged);
    manager.loadCustomEmoji(obj);
    QCOMPARE(manager.count(), number);

    QCOMPARE(manager.customEmojiList(), original);

    const QString addJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/json/restapi/") + addName + QLatin1String(".json");
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
    QTest::addColumn<QVector<CustomEmoji>>("original");
    QTest::addColumn<QVector<CustomEmoji>>("customEmoji");
    {
        QVector<CustomEmoji> emojiList;
        CustomEmoji val;
        val.setName(QStringLiteral("kdab"));
        val.setIdentifier(QStringLiteral("RyBauhQqnoE5WeJvZ"));
        val.setExtension(QStringLiteral("png"));
        val.setEmojiIdentifier(QStringLiteral(":kdab:"));
        val.setUpdatedAt(1529303015003);

        CustomEmoji val1;
        val1.setName(QStringLiteral("vader"));
        val1.setIdentifier(QStringLiteral("fAiQmJnJPAaEFmps6"));
        val1.setExtension(QStringLiteral("png"));
        val1.setEmojiIdentifier(QStringLiteral(":vader:"));
        val1.setUpdatedAt(1560497261506);
        val1.setAliases({QStringLiteral(":darth:")});

        emojiList.append(val);
        emojiList.append(val1);

        QVector<CustomEmoji> emojiListAfterDeleting;
        CustomEmoji val2;
        val2.setName(QStringLiteral("ruqola"));
        val2.setIdentifier(QStringLiteral("RyBauhQqnoE5WeJvZ"));
        val2.setExtension(QStringLiteral("png"));
        val2.setEmojiIdentifier(QStringLiteral(":ruqola:"));
        val2.setUpdatedAt(1631885946222);
        val2.setAliases({QStringLiteral("rooss")});
        emojiListAfterDeleting.append(val);
        emojiListAfterDeleting.append(val2);
        QTest::addRow("emojiparent2") << QStringLiteral("emojiparent2") << 3227 << QStringLiteral("updateemojicustom1") << emojiList << emojiList;
    }
}

void EmojiManagerTest::shouldUpdateEmojiCustom()
{
    QFETCH(QString, initialListName);
    QFETCH(int, number);
    QFETCH(QString, addName);
    QFETCH(QVector<CustomEmoji>, original);
    QFETCH(QVector<CustomEmoji>, customEmoji);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/json/restapi/") + initialListName + QLatin1String(".json");
    auto obj = AutoTestHelper::loadJsonObject(originalJsonFile);
    EmojiManager manager(nullptr);
    QSignalSpy customEmojiChanged(&manager, &EmojiManager::customEmojiChanged);
    manager.loadCustomEmoji(obj);

    QCOMPARE(manager.count(), number);
    QCOMPARE(manager.customEmojiList(), original);

    const QString addJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/json/restapi/") + addName + QLatin1String(".json");
    const auto objAdd = AutoTestHelper::loadJsonArrayObject(addJsonFile);
    manager.addUpdateEmojiCustomList(objAdd);
    QCOMPARE(customEmojiChanged.count(), 1);
    QCOMPARE(customEmojiChanged.at(0).at(0).toBool(), false);
}

void EmojiManagerTest::shouldSupportUnicodeEmojis()
{
    // Load list of unicode emoticon
    UnicodeEmoticonManager::self();
    EmojiManager manager(nullptr);
    QString grinning;
    grinning += QChar(0xd800 + 61);
    grinning += QChar(0xDC00 + 512);
    // A basic emoji that was already there in the initial emoji.json from fairchat
    QCOMPARE(manager.unicodeEmoticonForEmoji(QStringLiteral(":grinning:")).unicode(), grinning);
    // The one that made me use https://raw.githubusercontent.com/joypixels/emoji-toolkit/master/emoji.json instead
    QVERIFY(manager.unicodeEmoticonForEmoji(QStringLiteral(":zany_face:")).isValid());
    // A "shortname alternate"
    QCOMPARE(manager.unicodeEmoticonForEmoji(QStringLiteral(":water_polo_tone5:")).key(), QStringLiteral("1f93d-1f3ff"));
    // One with multiple values below 0x10000, to catch += vs = bug.
    QCOMPARE(manager.unicodeEmoticonForEmoji(QStringLiteral(":woman_climbing_tone4:")).unicode().length(), 7);
    QCOMPARE(manager.unicodeEmoticonForEmoji(QStringLiteral(":man_health_worker_tone1:")).unicode().length(), 7);

    // a smiley with shortname alternates and ascii versions
    QString slightSmile;
    slightSmile += QChar(0xD83D);
    slightSmile += QChar(0xDE42);
    QCOMPARE(manager.unicodeEmoticonForEmoji(QStringLiteral(":slight_smile:")).unicode(), slightSmile);
    // alternate
    QCOMPARE(manager.unicodeEmoticonForEmoji(QStringLiteral(":slightly_smiling_face:")).unicode(), slightSmile);
    // ascii
    QCOMPARE(manager.unicodeEmoticonForEmoji(QStringLiteral(":)")).unicode(), slightSmile);
    QCOMPARE(manager.unicodeEmoticonForEmoji(QStringLiteral(":-)")).unicode(), slightSmile);

    QCOMPARE(manager.categories().count(), 9);
    QCOMPARE(manager.categories().at(0).category(), QStringLiteral("people"));
    QCOMPARE(manager.emojisForCategory(QStringLiteral("people")).count(), 1656);
}

void EmojiManagerTest::shouldOrderUnicodeEmojis()
{
    // Load list of unicode emoticon
    UnicodeEmoticonManager::self();
    EmojiManager manager(nullptr);
    const QVector<UnicodeEmoticon> list = manager.unicodeEmojiList();
    auto hasCategory = [](const QString &category) {
        return [category](const UnicodeEmoticon &emo) {
            return emo.category() == category;
        };
    };
    // Check what's the first emoji in the category "symbols"
    auto it = std::find_if(list.begin(), list.end(), hasCategory(QStringLiteral("symbols")));
    QVERIFY(it != list.end());
    const UnicodeEmoticon firstSymbol = *it;
    QCOMPARE(firstSymbol.order(), 1);
    QCOMPARE(firstSymbol.category(), QStringLiteral("symbols"));
    QCOMPARE(firstSymbol.identifier(), QStringLiteral(":heart:"));

    // Check what's the first emoji in the category "regional"
    it = std::find_if(list.begin(), list.end(), hasCategory(QStringLiteral("regional")));
    QVERIFY(it != list.end());
    const UnicodeEmoticon firstRegional = *it;
    QCOMPARE(firstRegional.identifier(), QStringLiteral(":regional_indicator_z:")); // letters are reversed, weird
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
    EmojiManager manager(nullptr);
    manager.loadCustomEmoji(obj);
    // No serverUrl set.
    QCOMPARE(manager.replaceEmojiIdentifier(QStringLiteral(":foo:")), QStringLiteral(":foo:"));

    const QString serverUrl = QStringLiteral("www.kde.org");
    manager.setServerUrl(serverUrl);

    // :foo: doesn't exist
    QCOMPARE(manager.replaceEmojiIdentifier(QStringLiteral(":foo:")), QStringLiteral(":foo:"));
    QCOMPARE(manager.customEmojiFileName(QStringLiteral(":foo:")), QString());

    //    // Existing emoji
    //    QCOMPARE(manager.replaceEmojiIdentifier(QStringLiteral(":vader:")), QStringLiteral("<img height='22' width='22'
    //    src='http://www.kde.org/emoji-custom/vader.png' title=':vader:'/>")); QCOMPARE(manager.customEmojiFileName(QStringLiteral(":vader:")),
    //    QStringLiteral("/emoji-custom/vader.png"));

    //    // Alias
    //    QCOMPARE(manager.replaceEmojiIdentifier(QStringLiteral(":darth:")), QStringLiteral("<img height='22' width='22'
    //    src='http://www.kde.org/emoji-custom/vader.png' title=':vader:'/>")); QCOMPARE(manager.customEmojiFileName(QStringLiteral(":darth:")),
    //    QStringLiteral("/emoji-custom/vader.png"));
}

void EmojiManagerTest::shouldChangeServerUrl()
{
    //    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/json/restapi/emojiparent.json");
    //    QFile f(originalJsonFile);
    //    QVERIFY(f.open(QIODevice::ReadOnly));
    //    const QByteArray content = f.readAll();
    //    f.close();
    //    const QJsonDocument doc = QJsonDocument::fromJson(content);
    //    const QJsonObject obj = doc.object();
    //    EmojiManager manager(nullptr);
    //    manager.loadCustomEmoji(obj);
    //    QString serverUrl = QStringLiteral("www.kde.org");
    //    manager.setServerUrl(serverUrl);

    //    //It exists
    //    QCOMPARE(manager.replaceEmojiIdentifier(QStringLiteral(":vader:")), QStringLiteral("<img height='22' width='22' src='http://%1/emoji-custom/vader.png'
    //    title=':vader:'/>").arg(serverUrl));

    //    //Change server url => clear cache
    //    serverUrl = QStringLiteral("www.bla.org");
    //    manager.setServerUrl(serverUrl);
    //    QCOMPARE(manager.replaceEmojiIdentifier(QStringLiteral(":vader:")), QStringLiteral("<img height='22' width='22' src='http://%1/emoji-custom/vader.png'
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

    UnicodeEmoticonManager::self();
    RocketChatAccount account;
    account.ownUserPreferences().setConvertAsciiEmoji(true);
    EmojiManager manager(&account);
    manager.setServerUrl(QStringLiteral("blah blah"));
    manager.replaceEmojis(&input);

    QCOMPARE(input != original, replaced);
}
