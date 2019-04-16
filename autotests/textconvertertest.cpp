/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

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

#include "textconvertertest.h"
#include "textconverter.h"
#include "emoticons/emojimanager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QTest>

QTEST_GUILESS_MAIN(TextConverterTest)

TextConverterTest::TextConverterTest(QObject *parent)
    : QObject(parent)
{
}

void TextConverterTest::shouldConvertText_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");
    QTest::newRow("empty") << QString() << QString();
    QTest::newRow("simpletext") << QStringLiteral("foo") << QStringLiteral("foo");
    QTest::newRow("customemojiwithoutmanager") << QStringLiteral(":foo:") << QStringLiteral(":foo:");
}

void TextConverterTest::shouldConvertText()
{
    QFETCH(QString, input);
    QFETCH(QString, output);
    TextConverter w;
    QCOMPARE(w.convertMessageText(input, {}, QString()), output);
}

void TextConverterTest::shouldConvertTextWithEmoji_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");
    QTest::addColumn<QString>("serverUrl");
    QTest::newRow("empty") << QString() << QString() << QStringLiteral("www.kde.org");
    QTest::newRow("bold") << QStringLiteral("*foo*") << QStringLiteral("<b>*foo*</b>") << QStringLiteral("www.kde.org");
    QTest::newRow("underline") << QStringLiteral("_foo_") << QStringLiteral("<u>_foo_</u>") << QStringLiteral("www.kde.org");
    //TODO error
    QTest::newRow("underline2") << QStringLiteral("_personal: theming related tasks_") << QStringLiteral("<u>_personal: theming related tasks_</u>") << QStringLiteral("www.kde.org");
    QTest::newRow("simpletext") << QStringLiteral("foo") << QStringLiteral("foo") << QStringLiteral("www.kde.org");
    QTest::newRow("customemojiwithmanager") << QStringLiteral(":foo:") << QStringLiteral(":foo:") << QStringLiteral("www.kde.org");
    QTest::newRow("customemojiwithmanager1") << QStringLiteral(":totoro:")
                                             << QStringLiteral("<img height='22' width='22' src='http://www.kde.org/emoji-custom/totoro.gif'/>")
                                             << QStringLiteral("www.kde.org");
    QTest::newRow("customemojiwithmanager2") << QStringLiteral(":totoro::totoro:")
                                             << QStringLiteral(
        "<img height='22' width='22' src='http://www.kde.org/emoji-custom/totoro.gif'/><img height='22' width='22' src='http://www.kde.org/emoji-custom/totoro.gif'/>")
                                             << QStringLiteral("www.kde.org");

    //Use server with http://
    QTest::newRow("customemojiwithmanager3") << QStringLiteral(":totoro::totoro:")
                                             << QStringLiteral(
        "<img height='22' width='22' src='http://www.kde.org/emoji-custom/totoro.gif'/><img height='22' width='22' src='http://www.kde.org/emoji-custom/totoro.gif'/>")
                                             << QStringLiteral("http://www.kde.org");

    //alias support
    QTest::newRow("customemojiwithmanager4-with-alias") << QStringLiteral(":clap::clapping:")
                                                        << QStringLiteral(
        "<img height='22' width='22' src='http://www.kde.org/emoji-custom/clapping.gif'/><img height='22' width='22' src='http://www.kde.org/emoji-custom/clapping.gif'/>")
                                                        << QStringLiteral("http://www.kde.org");

    QTest::newRow("quotedcode1") << QStringLiteral("```foo```") << QStringLiteral("<pre>foo<br></pre>") << QStringLiteral("www.kde.org");
    QTest::newRow("quotedcode2") << QStringLiteral("bla\n```foo```bli") << QStringLiteral("bla\n<pre>foo<br></pre>bli") << QStringLiteral("www.kde.org");
    QTest::newRow("quotedcode3") << QStringLiteral("bla\n```foo```") << QStringLiteral("bla\n<pre>foo<br></pre>") << QStringLiteral("www.kde.org");
    QTest::newRow("quotedcode4") << QStringLiteral("```foo```\nff") << QStringLiteral("<pre>foo<br></pre>\nff") << QStringLiteral("www.kde.org");
}

void TextConverterTest::shouldConvertTextWithEmoji()
{
    QFETCH(QString, input);
    QFETCH(QString, output);
    QFETCH(QString, serverUrl);

    //Load emoji
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QStringLiteral("/json/") + QStringLiteral("emojiparent") + QStringLiteral(".json");
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject obj = doc.object();
    EmojiManager manager;
    manager.loadCustomEmoji(obj, false);
    manager.setServerUrl(serverUrl);

    TextConverter w(&manager);

    QCOMPARE(w.convertMessageText(input, {}, QString()), output);
}
