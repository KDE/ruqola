/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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
#include "messages/message.h"
#include "emoticons/emojimanager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QTest>

#include <KColorScheme>

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
    QTest::newRow("simpletext") << QStringLiteral("foo") << QStringLiteral("<div>foo</div>");
    QTest::newRow("customemojiwithoutmanager") << QStringLiteral(":foo:") << QStringLiteral("<div>:foo:</div>");
}

void TextConverterTest::shouldConvertText()
{
    QFETCH(QString, input);
    QFETCH(QString, output);
    TextConverter w;
    output.prepend(QLatin1String("<qt>"));
    output.append(QLatin1String("</qt>"));
    QCOMPARE(w.convertMessageText(input, QString(), {}), output);
}

void TextConverterTest::shouldConvertTextWithEmoji_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");
    QTest::addColumn<QString>("serverUrl");
    QTest::newRow("empty") << QString() << QString() << QStringLiteral("www.kde.org");
    QTest::newRow("bold") << QStringLiteral("*foo*") << QStringLiteral("<div><b>*foo*</b></div>") << QStringLiteral("www.kde.org");
    QTest::newRow("underline") << QStringLiteral("_foo_") << QStringLiteral("<div><u>_foo_</u></div>") << QStringLiteral("www.kde.org");
    //TODO error
    QTest::newRow("underline2") << QStringLiteral("_personal: theming related tasks_") << QStringLiteral("<div><u>_personal: theming related tasks_</u></div>") << QStringLiteral("www.kde.org");
    QTest::newRow("simpletext") << QStringLiteral("foo") << QStringLiteral("<div>foo</div>") << QStringLiteral("www.kde.org");
    QTest::newRow("customemojiwithmanager") << QStringLiteral(":foo:") << QStringLiteral("<div>:foo:</div>") << QStringLiteral("www.kde.org");
    QTest::newRow("customemojiwithmanager1") << QStringLiteral(":vader:")
                                             << QStringLiteral("<div><img height='22' width='22' src='http://www.kde.org/emoji-custom/vader.png' title=':vader:'/></div>")
                                             << QStringLiteral("www.kde.org");
    QTest::newRow("customemojiwithmanager2") << QStringLiteral(":vader::vader:")
                                             << QStringLiteral(
        "<div><img height='22' width='22' src='http://www.kde.org/emoji-custom/vader.png' title=':vader:'/><img height='22' width='22' src='http://www.kde.org/emoji-custom/vader.png' title=':vader:'/></div>")
                                             << QStringLiteral("www.kde.org");

    //Use server with http://
    QTest::newRow("customemojiwithmanager3") << QStringLiteral("the icon for vader is :vader::vader:")
                                             << QStringLiteral(
        "<div>the icon for vader is <img height='22' width='22' src='http://www.kde.org/emoji-custom/vader.png' title=':vader:'/><img height='22' width='22' src='http://www.kde.org/emoji-custom/vader.png' title=':vader:'/></div>")
                                             << QStringLiteral("http://www.kde.org");

    //alias support
    QTest::newRow("customemojiwithmanager4-with-alias") << QStringLiteral(":darth::vader:")
                                                        << QStringLiteral(
        "<div><img height='22' width='22' src='http://www.kde.org/emoji-custom/vader.png' title=':vader:'/><img height='22' width='22' src='http://www.kde.org/emoji-custom/vader.png' title=':vader:'/></div>")
                                                        << QStringLiteral("http://www.kde.org");

    QTest::newRow("quotedcode1") << QStringLiteral("bla```foo```blub")
                                 << QStringLiteral("<div>bla</div><table><tr><td style='background-color:$BGCOLOR$; padding: 5px; border: 1px solid $BORDERCOLOR$'><code>foo</code></td></tr></table><div>blub</div>") << QStringLiteral("www.kde.org");
    QTest::newRow("quotedcode2") << QStringLiteral("bla\n```foo```bli")
                                 << QStringLiteral("<div>bla</div><table><tr><td style='background-color:$BGCOLOR$; padding: 5px; border: 1px solid $BORDERCOLOR$'><code>foo</code></td></tr></table><div>bli</div>") << QStringLiteral("www.kde.org");
    QTest::newRow("quotedcode3") << QStringLiteral("bla\n```foo```")
                                 << QStringLiteral("<div>bla</div><table><tr><td style='background-color:$BGCOLOR$; padding: 5px; border: 1px solid $BORDERCOLOR$'><code>foo</code></td></tr></table>") << QStringLiteral("www.kde.org");
    QTest::newRow("quotedcode4") << QStringLiteral("```foo```\nff")
                                 << QStringLiteral("<table><tr><td style='background-color:$BGCOLOR$; padding: 5px; border: 1px solid $BORDERCOLOR$'><code>foo</code></td></tr></table><div>ff</div>") << QStringLiteral("www.kde.org");
    QTest::newRow("quotedcode5") << QStringLiteral("bla\n```\nfoo\n```\nff")
                                 << QStringLiteral("<div>bla</div><table><tr><td style='background-color:$BGCOLOR$; padding: 5px; border: 1px solid $BORDERCOLOR$'><code>foo</code></td></tr></table><div>ff</div>") << QStringLiteral("www.kde.org");
    QTest::newRow("quotedcode6") << QStringLiteral("*foo*\n```\nfoo\n```\n*bar*\n```blub```\n*asdf*")
                                 << QStringLiteral(
        "<div><b>*foo*</b></div><table><tr><td style='background-color:$BGCOLOR$; padding: 5px; border: 1px solid $BORDERCOLOR$'><code>foo</code></td></tr></table><div><b>*bar*</b></div><table><tr><td style='background-color:$BGCOLOR$; padding: 5px; border: 1px solid $BORDERCOLOR$'><code>blub</code></td></tr></table><div><b>*asdf*</b></div>")
                                 << QStringLiteral("www.kde.org");
    QTest::newRow("quotedcode7") << QStringLiteral(":vader:\n```\n:vader:\n```\n:vader:")
                                 << QStringLiteral(
        "<div><img height='22' width='22' src='http://www.kde.org/emoji-custom/vader.png' title=':vader:'/></div><table><tr><td style='background-color:$BGCOLOR$; padding: 5px; border: 1px solid $BORDERCOLOR$'><code>:vader:</code></td></tr></table><div><img height='22' width='22' src='http://www.kde.org/emoji-custom/vader.png' title=':vader:'/></div>")
                                 << QStringLiteral("www.kde.org");
}

void TextConverterTest::shouldConvertTextWithEmoji()
{
    QFETCH(QString, input);
    QFETCH(QString, output);
    QFETCH(QString, serverUrl);

    {
        KColorScheme scheme;
        const auto codeBackgroundColor = scheme.background(KColorScheme::AlternateBackground).color().name();
        const auto codeBorderColor = scheme.foreground(KColorScheme::InactiveText).color().name();

        output.prepend(QLatin1String("<qt>"));
        output.append(QLatin1String("</qt>"));
        output.replace(QLatin1String("$BGCOLOR$"), codeBackgroundColor);
        output.replace(QLatin1String("$BORDERCOLOR$"), codeBorderColor);
    }

    //Load emoji
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/json/restapi/emojiparent.json");
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject obj = doc.object();
    EmojiManager manager;
    manager.loadCustomEmoji(obj);
    manager.setServerUrl(serverUrl);

    TextConverter w(&manager);
    QCOMPARE(w.convertMessageText(input, QString(), {}), output);
}
