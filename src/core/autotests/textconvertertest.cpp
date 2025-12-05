/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "textconvertertest.h"

#include "emoticons/emojimanager.h"
#include "messages/message.h"
#include "ruqola_autotest_helper.h"
#include "textconverter.h"
#include "utils.h"
#include <QJsonObject>
#include <QRegularExpression>

#include <KColorScheme>
#include <QTest>

QTEST_GUILESS_MAIN(TextConverterTest)
using namespace Qt::Literals::StringLiterals;

namespace
{
QString prepareExpectedOutput(QString output)
{
    KColorScheme scheme;
    const auto codeBackgroundColor = scheme.background(KColorScheme::AlternateBackground).color().name();
    const auto codeBorderColor = scheme.foreground(KColorScheme::InactiveText).color().name();

    const auto userHighlightForegroundColor = scheme.foreground(KColorScheme::PositiveText).color().name();
    const auto userHighlightBackgroundColor = scheme.background(KColorScheme::PositiveBackground).color().name();
    const auto hereAllMentionBackgroundColor = scheme.background(KColorScheme::NeutralBackground).color().name();
    const auto hereAllMentionForegroundColor = scheme.foreground(KColorScheme::NeutralText).color().name();

    output.replace("$BGCOLOR$"_L1, codeBackgroundColor);
    output.replace("$BORDERCOLOR$"_L1, codeBorderColor);
    output.replace("$USERCOLOR$"_L1, userHighlightForegroundColor);
    output.replace("$USERBGCOLOR$"_L1, userHighlightBackgroundColor);
    output.replace("$HEREBGCOLOR$"_L1, hereAllMentionBackgroundColor);
    output.replace("$HERECOLOR$"_L1, hereAllMentionForegroundColor);

    output.prepend("<qt>"_L1);
    output.append("</qt>"_L1);

    return output;
}
}

TextConverterTest::TextConverterTest(QObject *parent)
    : QObject(parent)
{
}

void TextConverterTest::shouldConvertText_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");
    QTest::newRow("empty") << QString() << QString();
    QTest::newRow("simpletext") << QStringLiteral("foo") << QStringLiteral("<p>foo</p>\n");
    QTest::newRow("customemojiwithoutmanager") << QStringLiteral(":foo:") << QStringLiteral("<p>:foo:</p>\n");

    QTest::newRow("testurl") << QStringLiteral("http://www.kde.org http://www.kde.org")
                             << QStringLiteral(
                                    "<p><a href=\"http://www.kde.org\">http://www.kde.org</a> <a href=\"http://www.kde.org\">http://www.kde.org</a></p>\n");
    QTest::newRow("named-url") << QStringLiteral("[example.com](http://example.com)")
                               << QStringLiteral("<p><a href=\"http://example.com\">example.com</a></p>\n");
    QTest::newRow("bold") << QStringLiteral("*bla*") << QStringLiteral("<p><strong>bla</strong></p>\n");
    QTest::newRow("multi star") << QStringLiteral("**bla**") << QStringLiteral("<p><strong>bla</strong></p>\n");
    QTest::newRow("multi star2") << QStringLiteral("***bla***") << QStringLiteral("<p><strong><strong>bla</strong></strong></p>\n");
    QTest::newRow("multi star3") << QStringLiteral("***bla ******") << QStringLiteral("<p>***bla ******</p>\n");
    QTest::newRow("Remove <br/>") << QStringLiteral("foo<br />") << QStringLiteral("<p>foo</p>\n");
    QTest::newRow("0.6.3") << QStringLiteral("0.6.3") << QStringLiteral("<p>0.6.3</p>\n");
    // Bug 391520
    QTest::newRow("multi-line") << QStringLiteral("These are the options:\n- a\n- b")
                                << QStringLiteral("<p>These are the options:</p>\n<ul>\n<li>a</li>\n<li>b</li>\n</ul>\n");

    QTest::newRow("word@") << QStringLiteral("@foo") << QStringLiteral("<p><a href='ruqola:/user/foo'>@foo</a></p>\n");
    QTest::newRow("word@-2") << QStringLiteral("@foo.bla") << QStringLiteral("<p><a href='ruqola:/user/foo.bla'>@foo.bla</a></p>\n");
    QTest::newRow("word@-3") << QStringLiteral("@foo.bla.bli") << QStringLiteral("<p><a href='ruqola:/user/foo.bla.bli'>@foo.bla.bli</a></p>\n");
    QTest::newRow("word@-4") << QStringLiteral("@foo.bla.bli dd") << QStringLiteral("<p><a href='ruqola:/user/foo.bla.bli'>@foo.bla.bli</a> dd</p>\n");
    QTest::newRow("word@-5") << QStringLiteral("bla bla 21 @foo.bla.bli dd")
                             << QStringLiteral("<p>bla bla 21 <a href='ruqola:/user/foo.bla.bli'>@foo.bla.bli</a> dd</p>\n");
    QTest::newRow("word@-6") << QStringLiteral("@foo-bla") << QStringLiteral("<p><a href='ruqola:/user/foo-bla'>@foo-bla</a></p>\n");
    QTest::newRow("word@-7") << QStringLiteral("@foo_bla") << QStringLiteral("<p><a href='ruqola:/user/foo_bla'>@foo_bla</a></p>\n");
    QTest::newRow("word@-8") << QStringLiteral("bli@foo_bla") << QStringLiteral("<p>bli@foo_bla</p>\n");
    QTest::newRow("word@-9") << QStringLiteral("@réunion") << QStringLiteral("<p><a href='ruqola:/user/réunion'>@réunion</a></p>\n");

    QTest::newRow("word#") << QStringLiteral("#foo") << QStringLiteral("<p><a href='ruqola:/room/foo'>#foo</a></p>\n");
    QTest::newRow("word#-2") << QStringLiteral("#foo.bla") << QStringLiteral("<p><a href='ruqola:/room/foo.bla'>#foo.bla</a></p>\n");
    QTest::newRow("word#-3") << QStringLiteral("#foo.bla.bli") << QStringLiteral("<p><a href='ruqola:/room/foo.bla.bli'>#foo.bla.bli</a></p>\n");
    QTest::newRow("word#-4") << QStringLiteral("#foo.bla.bli dd") << QStringLiteral("<p><a href='ruqola:/room/foo.bla.bli'>#foo.bla.bli</a> dd</p>\n");
    QTest::newRow("word#-5") << QStringLiteral("bla bla 21 #foo.bla.bli dd")
                             << QStringLiteral("<p>bla bla 21 <a href='ruqola:/room/foo.bla.bli'>#foo.bla.bli</a> dd</p>\n");
    QTest::newRow("word#-6") << QStringLiteral("#foo-bla") << QStringLiteral("<p><a href='ruqola:/room/foo-bla'>#foo-bla</a></p>\n");
    QTest::newRow("word#-7") << QStringLiteral("#foo_bla") << QStringLiteral("<p><a href='ruqola:/room/foo_bla'>#foo_bla</a></p>\n");
    QTest::newRow("word#-8") << QStringLiteral("#réunion") << QStringLiteral("<p><a href='ruqola:/room/réunion'>#réunion</a></p>\n");
    // Test parsing when it's in an url... don't replace it.

    QTest::newRow("url") << QStringLiteral("http://www.kde.org#foo_bla")
                         << QStringLiteral("<p><a href=\"http://www.kde.org#foo_bla\">http://www.kde.org#foo_bla</a></p>\n");

    QTest::newRow("`code`")
        << QStringLiteral("foo `bla` bar `asdf` test")
        << QStringLiteral("<p>foo <code style='background-color:$BGCOLOR$'>bla</code> bar <code style='background-color:$BGCOLOR$'>asdf</code> test</p>\n");
    QTest::newRow("`code \\` escaped`") << QStringLiteral("foo `bla\\` bar \\`asdf` test")
                                        << QStringLiteral("<p>foo `bla\\` bar <code style='background-color:$BGCOLOR$'>asdf</code> test</p>\n");
    QTest::newRow("quote<") << QStringLiteral("`inside quotes: <<....` this is outside the quotes <<.")
                            << QStringLiteral(
                                   "<p><code style='background-color:$BGCOLOR$'>inside quotes: &lt;&lt;....</code> this is outside the quotes &lt;&lt;.</p>\n");
    QTest::newRow("quotedtext1") << QStringLiteral("> toto") << QStringLiteral("<p><code style='background-color:$BGCOLOR$'>toto</code></p>\n");
    QTest::newRow("quotedtext2") << QStringLiteral("bla \n> toto")
                                 << QStringLiteral("<p>bla<br />\n<code style='background-color:$BGCOLOR$'>toto</code></p>\n");

    QTest::newRow("quotedtext3")
        << QStringLiteral("bla \n> toto \n> bla")
        << QStringLiteral(
               "<p>bla<br />\n<code style='background-color:$BGCOLOR$'>toto</code><br />\n<code style='background-color:$BGCOLOR$'>bla</code></p>\n");

    QTest::newRow("quotedtext4") << QStringLiteral("bla \n> toto \n> *bla*")
                                 << QStringLiteral(
                                        "<p>bla<br />\n<code style='background-color:$BGCOLOR$'>toto</code><br />\n<code "
                                        "style='background-color:$BGCOLOR$'></code><strong>bla</strong></p>\n");

    QTest::newRow("noquotedtext") << QStringLiteral("bla > toto") << QStringLiteral("<p>bla &gt; toto</p>\n");

    QTest::newRow("code-with-language") << QStringLiteral("```ruby\nssss\n```")
                                        << QStringLiteral(
                                               "<p><table><tr><td style='background-color:$BGCOLOR$; padding: 5px; border: 1px solid "
                                               "$BORDERCOLOR$'><code>ssss</code></td></tr></table></p>\n");

    QTest::newRow("code-with-language-2")
        << QStringLiteral("```dart\nimport 'package:flutter/material.dart';\nimport 'package:markdown/markdown.dart';\n```")
        << QStringLiteral(
               "<p><table><tr><td style='background-color:$BGCOLOR$; padding: 5px; border: 1px solid $BORDERCOLOR$'><code><span "
               "style=\"font-weight:bold;\">import</span>&nbsp;<span style=\"color:#bf0303;\">'package:flutter/material.dart'</span>;<br><span "
               "style=\"font-weight:bold;\">import</span>&nbsp;<span "
               "style=\"color:#bf0303;\">'package:markdown/markdown.dart'</span>;</code></td></tr></table></p>\n");

    QTest::newRow("indented-code-block")
        << QStringLiteral("```\n  first line\n  second line\n```")
        << QStringLiteral(
               "<p><table><tr><td style='background-color:$BGCOLOR$; padding: 5px; border: 1px solid "
               "$BORDERCOLOR$'><code>&nbsp;&nbsp;first&nbsp;line<br>&nbsp;&nbsp;second&nbsp;line</code></td></tr></table></p>\n");

    QTest::newRow("indented-code-block2")
        << QStringLiteral("```\n\tfirst line\t  second line\n```")
        << QStringLiteral(
               "<p><table><tr><td style='background-color:#f7f7f7; padding: 5px; border: 1px solid "
               "#707d8a'><code>&nbsp;&nbsp;&nbsp;&nbsp;first&nbsp;line&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;second&nbsp;line</code></td></tr></table></p>\n");
    // We don't need to convert #1
    QTest::newRow("href + #") << QStringLiteral("[test #1](http://www.kde.org)") << QStringLiteral("<p><a href=\"http://www.kde.org\">test #1</a></p>\n");
}

void TextConverterTest::shouldConvertText()
{
    QFETCH(QString, input);
    QFETCH(QString, output);

    output = prepareExpectedOutput(output);

    QEXPECT_FAIL("Remove <br/>", "Bug in KF6", Continue);

    QByteArray needUpdateMessageId;
    const TextConverter::ConvertMessageTextSettings settings(input, QString(), {}, {}, nullptr, nullptr, {}, {}, {});
    int recursiveIndex = 0;
    int numberOfTextSearched = 0;
    const int hightLightStringIndex = 0;
    const QString result = TextConverter::convertMessageText(settings, needUpdateMessageId, recursiveIndex, numberOfTextSearched, hightLightStringIndex);
    qDebug() << "result " << result;
    QCOMPARE(result,
             output); // TODO add autotests for highlightwords
}

void TextConverterTest::shouldHighlightWords_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("username");
    QTest::addColumn<QStringList>("highlightWords");
    QTest::addColumn<QString>("output");
    QTest::newRow("empty") << QString() << QString() << QStringList{} << QString();
    const QStringList highlightWords{QStringLiteral("ruqola"), QStringLiteral("kde")};
    QTest::newRow("lowercase") << QStringLiteral("Ruqola") << QStringLiteral("foo") << highlightWords
                               << QStringLiteral("<p><a style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">Ruqola</a></p>\n");
    QTest::newRow("two-word") << QStringLiteral("Ruqola kde") << QStringLiteral("foo") << highlightWords
                              << QStringLiteral(
                                     "<p><a style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">Ruqola</a> <a "
                                     "style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">kde</a></p>\n");

    QTest::newRow("words") << QStringLiteral("Ruqola bla kde KDE.") << QStringLiteral("foo") << highlightWords
                           << QStringLiteral(
                                  "<p><a style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">Ruqola</a> bla <a "
                                  "style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">kde</a> <a "
                                  "style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">KDE</a>.</p>\n");

    QTest::newRow("wordinurl") << QStringLiteral("https://www.kde.org/~/bla/bli.txt") << QStringLiteral("bla") << highlightWords
                               << QStringLiteral(
                                      "<p><a href=\"https://www.kde.org/~/bla/bli.txt\">https://www.<a "
                                      "style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">kde</a>.org/~/bla/bli.txt</a></p>\n");
    QTest::newRow("channelruqola")
        << QStringLiteral("#ruqola-bla bla kde KDE.") << QStringLiteral("foo") << highlightWords
        << QStringLiteral(
               "<p><a href='ruqola:/room/ruqola-bla'>#<a style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">ruqola</a>-bla</a> bla <a "
               "style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">kde</a> <a "
               "style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">KDE</a>.</p>\n");

    QTest::newRow("here1") << QStringLiteral("ruqola @here") << QStringLiteral("foo") << highlightWords
                           << QStringLiteral(
                                  "<p><a style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">ruqola</a> <a "
                                  "style=\"color:$HERECOLOR$;background-color:$HEREBGCOLOR$;font-weight:bold\">here</a></p>\n");
}

void TextConverterTest::shouldHighlightWords()
{
    QFETCH(QString, input);
    QFETCH(QString, username);
    QFETCH(QStringList, highlightWords);
    QFETCH(QString, output);

    output = prepareExpectedOutput(output);

    QByteArray needUpdateMessageId;
    int recursiveIndex = 0;
    int numberOfTextSearched = 0;
    int hightLightStringIndex = 0;
    const TextConverter::ConvertMessageTextSettings settings(input, username, {}, highlightWords, nullptr, nullptr, {}, {}, {});
    QCOMPARE(TextConverter::convertMessageText(settings, needUpdateMessageId, recursiveIndex, numberOfTextSearched, hightLightStringIndex), output);
}

void TextConverterTest::shouldHighlightText_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("username");
    QTest::addColumn<QString>("output");
    QTest::newRow("empty") << QString() << QString() << QString();
    QTest::newRow("word@1") << QStringLiteral("@foo") << QString() << QStringLiteral("<p><a href='ruqola:/user/foo'>@foo</a></p>\n");
    KColorScheme colorScheme;
    const auto userMentionForegroundColor = colorScheme.foreground(KColorScheme::NegativeText).color().name();
    const auto userMentionBackgroundColor = colorScheme.background(KColorScheme::NegativeBackground).color().name();
    QTest::newRow("word@1-username") << QStringLiteral("@foo") << QStringLiteral("foo")
                                     << QStringLiteral("<p><a href='ruqola:/user/foo' style=\"color:%1;background-color:%2;font-weight:bold\">@foo</a></p>\n")
                                            .arg(userMentionForegroundColor, userMentionBackgroundColor);
    QTest::newRow("word@2-username") << QStringLiteral("bla bla @foo") << QStringLiteral("foo")
                                     << QStringLiteral(
                                            "<p>bla bla <a href='ruqola:/user/foo' style=\"color:%1;background-color:%2;font-weight:bold\">@foo</a></p>\n")
                                            .arg(userMentionForegroundColor, userMentionBackgroundColor);
}

void TextConverterTest::shouldHighlightText()
{
    QFETCH(QString, input);
    QFETCH(QString, username);
    QFETCH(QString, output);

    output = prepareExpectedOutput(output);
    QByteArray needUpdateMessageId;
    int recursiveIndex = 0;
    int numberOfTextSearched = 0;
    int hightLightStringIndex = 0;
    const TextConverter::ConvertMessageTextSettings settings(input, username, {}, {}, nullptr, nullptr, {}, {}, {});
    QCOMPARE(TextConverter::convertMessageText(settings, needUpdateMessageId, recursiveIndex, numberOfTextSearched, hightLightStringIndex), output);
}

void TextConverterTest::shouldConvertTextWithEmoji_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");
    QTest::addColumn<QString>("serverUrl");

    QTest::newRow("empty") << QString() << QString() << QStringLiteral("www.kde.org");
    QTest::newRow("bold") << QStringLiteral("*foo*") << QStringLiteral("<p><strong>foo</strong></p>\n") << QStringLiteral("www.kde.org");
    QTest::newRow("italic") << QStringLiteral("_foo_") << QStringLiteral("<p><em>foo</em></p>\n") << QStringLiteral("www.kde.org");
    // TODO error
    QTest::newRow("italic2") << QStringLiteral("_personal: theming related tasks_") << QStringLiteral("<p><em>personal: theming related tasks</em></p>\n")
                             << QStringLiteral("www.kde.org");
    QTest::newRow("simpletext") << QStringLiteral("foo") << QStringLiteral("<p>foo</p>\n") << QStringLiteral("www.kde.org");
    QTest::newRow("customemojiwithmanager") << QStringLiteral(":foo:") << QStringLiteral("<p>:foo:</p>\n") << QStringLiteral("www.kde.org");

    QString smiley;
    smiley += QChar(0xD83D);
    smiley += QChar(0xDE42);
    const auto smileyText = QStringLiteral("<span style=\"font: x-large %2\" title=\":slight_smile:\">%1</span>").arg(smiley, Utils::emojiFontName());
    QTest::newRow("ascii-smiley") << QStringLiteral(":)") << QStringLiteral("<p>%1</p>\n").arg(smileyText) << QStringLiteral("www.kde.org");
    QTest::newRow("multi-smiley") << QStringLiteral(":):slight_smile::):)") << QStringLiteral("<p>%1</p>\n").arg(smileyText.repeated(4))
                                  << QStringLiteral("www.kde.org");

    QTest::newRow("url") << QStringLiteral("https://www.kde.org") << QStringLiteral("<p><a href=\"https://www.kde.org\">https://www.kde.org</a></p>\n")
                         << QStringLiteral("www.kde.org");

    QTest::newRow("\")") << QStringLiteral("\")") << QStringLiteral("<p>&quot;)</p>\n") << QStringLiteral("www.kde.org");

    //    QTest::newRow("customemojiwithmanager1") << QStringLiteral(":vader:")
    //                                             << QStringLiteral("<p><img height='22' width='22' src='http://www.kde.org/emoji-custom/vader.png'
    //                                             title=':vader:'/></p>\n")
    //                                             << QStringLiteral("www.kde.org");
    //    QTest::newRow("customemojiwithmanager2") << QStringLiteral(":vader::vader:")
    //                                             << QStringLiteral(
    //        "<p><img height='22' width='22' src='http://www.kde.org/emoji-custom/vader.png' title=':vader:'/><img height='22' width='22'
    //        src='http://www.kde.org/emoji-custom/vader.png' title=':vader:'/></p>\n")
    //                                             << QStringLiteral("www.kde.org");

    //    //Use server with http://
    //    QTest::newRow("customemojiwithmanager3") << QStringLiteral("the icon for vader is :vader::vader:")
    //                                             << QStringLiteral(
    //        "<p>the icon for vader is <img height='22' width='22' src='http://www.kde.org/emoji-custom/vader.png' title=':vader:'/><img height='22'
    //        width='22' src='http://www.kde.org/emoji-custom/vader.png' title=':vader:'/></p>\n")
    //                                             << QStringLiteral("http://www.kde.org");

    //    //alias support
    //    QTest::newRow("customemojiwithmanager4-with-alias") << QStringLiteral(":darth::vader:")
    //                                                        << QStringLiteral(
    //        "<p><img height='22' width='22' src='http://www.kde.org/emoji-custom/vader.png' title=':vader:'/><img height='22' width='22'
    //        src='http://www.kde.org/emoji-custom/vader.png' title=':vader:'/></p>\n")
    //                                                        << QStringLiteral("http://www.kde.org");

    QTest::newRow("quotedcode1") << QStringLiteral("bla\n```\nfoo\n```\nblub")
                                 << QStringLiteral(
                                        "<p>bla</p>\n<p><table><tr><td style='background-color:$BGCOLOR$; padding: 5px; border: 1px solid "
                                        "$BORDERCOLOR$'><code>foo</code></td></tr></table></p>\n<p>blub</p>\n")
                                 << QStringLiteral("www.kde.org");

    QTest::newRow("quotedcode2") << QStringLiteral("bla\n```\nfoo\n```\nbli")
                                 << QStringLiteral(
                                        "<p>bla</p>\n<p><table><tr><td style='background-color:$BGCOLOR$; padding: 5px; border: 1px solid "
                                        "$BORDERCOLOR$'><code>foo</code></td></tr></table></p>\n<p>bli</p>\n")
                                 << QStringLiteral("www.kde.org");
    QTest::newRow("quotedcode3") << QStringLiteral("bla\n```\nfoo\n```")
                                 << QStringLiteral(
                                        "<p>bla</p>\n<p><table><tr><td style='background-color:$BGCOLOR$; padding: 5px; border: 1px solid "
                                        "$BORDERCOLOR$'><code>foo</code></td></tr></table></p>\n")
                                 << QStringLiteral("www.kde.org");
    QTest::newRow("quotedcode4") << QStringLiteral("```\nfoo\n```\nff")
                                 << QStringLiteral(
                                        "<p><table><tr><td style='background-color:$BGCOLOR$; padding: 5px; border: 1px solid "
                                        "$BORDERCOLOR$'><code>foo</code></td></tr></table></p>\n<p>ff</p>\n")
                                 << QStringLiteral("www.kde.org");

    QTest::newRow("quotedcode5") << QStringLiteral("bla\n```\nfoo\n```\nff")
                                 << QStringLiteral(
                                        "<p>bla</p>\n<p><table><tr><td style='background-color:$BGCOLOR$; padding: 5px; border: 1px solid "
                                        "$BORDERCOLOR$'><code>foo</code></td></tr></table></p>\n<p>ff</p>\n")
                                 << QStringLiteral("www.kde.org");

    QTest::newRow("quotedcode6") << QStringLiteral("*foo*\n```\nfoo\n```\n*bar*\n```\nblub\n```\n*asdf*")
                                 << QStringLiteral(
                                        "<p><strong>foo</strong></p>\n<p><table><tr><td style='background-color:$BGCOLOR$; padding: 5px; border: 1px solid "
                                        "$BORDERCOLOR$'><code>foo</code></td></tr></table></p>\n<p><strong>bar</strong></p>\n<p><table><tr><td "
                                        "style='background-color:$BGCOLOR$; padding: 5px; border: "
                                        "1px solid $BORDERCOLOR$'><code>blub</code></td></tr></table></p>\n<p><strong>asdf</strong></p>\n")
                                 << QStringLiteral("www.kde.org");
    QTest::newRow("quotedcode7") << QStringLiteral(":)\n```\n:)\n```\n:)")
                                 << QStringLiteral(
                                        "<p>%1</p>\n<p><table><tr><td style='background-color:$BGCOLOR$; padding: 5px; border: 1px solid "
                                        "$BORDERCOLOR$'><code>:)</code></td></tr></table></p>\n<p>%1</p>\n")
                                        .arg(smileyText)
                                 << QStringLiteral("www.kde.org");
    QTest::newRow("quotedcode8")
        << QStringLiteral("```javascript\ncode\n```")
        << QStringLiteral(
               "<p><table><tr><td style='background-color:$BGCOLOR$; padding: 5px; border: 1px solid $BORDERCOLOR$'><code>code</code></td></tr></table></p>\n")
        << QStringLiteral("www.kde.org");

    QTest::newRow("quotedcode9") << QStringLiteral("```\nblub\ncode\n```")
                                 << QStringLiteral(
                                        "<p><table><tr><td style='background-color:$BGCOLOR$; padding: 5px; border: 1px solid "
                                        "$BORDERCOLOR$'><code>blub<br>code</code></td></tr></table></p>\n")
                                 << QStringLiteral("www.kde.org");
    QTest::newRow("inline-code-with-brackets")
        << QStringLiteral("`[[test]]` and `a[b` something")
        << QStringLiteral(
               "<p><code style='background-color:$BGCOLOR$'>[[test]]</code> and <code style='background-color:$BGCOLOR$'>a[b</code> "
               "something</p>\n")
        << QStringLiteral("www.kde.org");

    QTest::newRow("inline-code-with-tilde") << QStringLiteral("`auto [a, b] = std::minmax_element(~~~);`")
                                            << QStringLiteral(
                                                   "<p><code style='background-color:$BGCOLOR$'>auto [a, b] = "
                                                   "std::minmax_element(~~~);</code></p>\n")
                                            << QStringLiteral("www.kde.org");

    QTest::newRow("inline-code-with-smiley") << QStringLiteral(":) `:)` :)")
                                             << QStringLiteral("<p>%1 <code style='background-color:$BGCOLOR$'>:)</code> %1</p>\n").arg(smileyText)
                                             << QStringLiteral("www.kde.org");

    QTest::newRow("url-with-emoji") << QStringLiteral("https://www.kde.org/:x:/bla.html")
                                    << QStringLiteral("<p>%1 <code style='background-color:$BGCOLOR$'>:)</code> %1</p>\n").arg(smileyText)
                                    << QStringLiteral("www.kde.org");
}

void TextConverterTest::shouldConvertTextWithEmoji()
{
    QFETCH(QString, input);
    QFETCH(QString, output);
    QFETCH(QString, serverUrl);

    output = prepareExpectedOutput(output);

    // Load emoji
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/json/restapi/emojiparent.json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);
    EmojiManager manager(nullptr);
    manager.loadCustomEmoji(obj);
    manager.setServerUrl(serverUrl);

    QByteArray needUpdateMessageId;
    int recursiveIndex = 0;
    int numberOfTextSearched = 0;
    int hightLightStringIndex = 0;
    const TextConverter::ConvertMessageTextSettings settings(input, QString(), {}, {}, &manager, nullptr, {}, {}, {});
    auto actualOutput = TextConverter::convertMessageText(settings, needUpdateMessageId, recursiveIndex, numberOfTextSearched, hightLightStringIndex);
    if (QLatin1StringView(QTest::currentDataTag()) == "quotedcode7"_L1) {
        // remove additional highlighting of the ':)' symbols within the <code> block
        // the text color is syntax highlighting theme dependent, so hard for us to check
        actualOutput.replace(QRegularExpression(QStringLiteral("<code><span style=\".+\">:\\)</span></code>")), QStringLiteral("<code>:)</code>"));
    }
    QEXPECT_FAIL("url-with-emoji", "Currently it if we have a emoji char in url", Continue);
    const bool isEqual = (actualOutput == output);
    if (!isEqual) {
        qDebug() << " actual: " << actualOutput;
        qDebug() << " expected: " << output;
    }
    QCOMPARE(actualOutput, output); // TODO add autotest for highlightwords
}

void TextConverterTest::shouldShowChannels_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");
    QTest::addColumn<QMap<QString, QByteArray>>("mentions");
    QTest::addColumn<QList<Channels::ChannelInfo>>("channels");

    {
        QMap<QString, QByteArray> mentions;
        QList<Channels::ChannelInfo> channels;
        QTest::newRow("empty") << QString() << QString() << mentions << channels;
    }
    {
        QMap<QString, QByteArray> mentions;
        QList<Channels::ChannelInfo> channels;
        Channels::ChannelInfo info;
        info.name = QStringLiteral("foo");
        info.identifier = "idd"_ba;
        channels.append(info);
        QTest::newRow("word#") << QStringLiteral("#foo") << QStringLiteral("<p><a href='ruqola:/room/idd'>#foo</a></p>\n") << mentions << channels;
    }
    {
        QMap<QString, QByteArray> mentions;
        QList<Channels::ChannelInfo> channels;
        Channels::ChannelInfo info;
        info.name = QStringLiteral("bla");
        info.identifier = "idd"_ba;
        channels.append(info);
        QTest::newRow("not existing room") << QStringLiteral("#foo") << QStringLiteral("<p><a href='ruqola:/room/foo'>#foo</a></p>\n") << mentions << channels;
    }
    {
        QMap<QString, QByteArray> mentions;
        QList<Channels::ChannelInfo> channels;
        Channels::ChannelInfo info;
        info.name = QStringLiteral("bla");
        info.fname = QStringLiteral("FNAME");
        info.identifier = "idd"_ba;
        channels.append(info);
        QTest::newRow("use fname") << QStringLiteral("#bla") << QStringLiteral("<p><a href='ruqola:/room/idd'>#FNAME</a></p>\n") << mentions << channels;
    }
    {
        QMap<QString, QByteArray> mentions;

        QList<Channels::ChannelInfo> channels;
        {
            Channels::ChannelInfo info;
            info.name = QStringLiteral("bli");
            info.identifier = "112"_ba;
            channels.append(info);
        }
        {
            Channels::ChannelInfo info;
            info.name = QStringLiteral("oss");
            info.identifier = "kli"_ba;
            channels.append(info);
        }
        QTest::newRow("multi channel") << QStringLiteral("foo #bli blass #oss")
                                       << QStringLiteral("<p>foo <a href='ruqola:/room/112'>#bli</a> blass <a href='ruqola:/room/kli'>#oss</a></p>\n")
                                       << mentions << channels;
    }
}

void TextConverterTest::shouldShowChannels()
{
    using mapMentions = QMap<QString, QByteArray>;
    using mapChannels = QList<Channels::ChannelInfo>;
    QFETCH(QString, input);
    QFETCH(QString, output);
    QFETCH(mapMentions, mentions);
    QFETCH(mapChannels, channels);

    Channels *c = new Channels;
    c->setChannels(channels);
    output = prepareExpectedOutput(output);
    QByteArray needUpdateMessageId;
    int recursiveIndex = 0;
    int numberOfTextSearched = 0;
    int hightLightStringIndex = 0;
    const TextConverter::ConvertMessageTextSettings settings(input, {}, {}, {}, nullptr, nullptr, mentions, c, {});
    QCOMPARE(TextConverter::convertMessageText(settings, needUpdateMessageId, recursiveIndex, numberOfTextSearched, hightLightStringIndex), output);
    delete c;
}

void TextConverterTest::shouldShowUsers()
{
    using mapMentions = QMap<QString, QByteArray>;
    using mapChannels = QList<Channels::ChannelInfo>;
    QFETCH(QString, input);
    QFETCH(QString, output);
    QFETCH(mapMentions, mentions);
    QFETCH(mapChannels, channels);

    Channels *c = new Channels;
    c->setChannels(channels);
    output = prepareExpectedOutput(output);
    QByteArray needUpdateMessageId;
    int recursiveIndex = 0;
    int numberOfTextSearched = 0;
    int hightLightStringIndex = 0;
    const TextConverter::ConvertMessageTextSettings settings(input, {}, {}, {}, nullptr, nullptr, mentions, c, {});
    QCOMPARE(TextConverter::convertMessageText(settings, needUpdateMessageId, recursiveIndex, numberOfTextSearched, hightLightStringIndex), output);
    delete c;
}

void TextConverterTest::shouldShowUsers_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");
    QTest::addColumn<QMap<QString, QByteArray>>("mentions");
    QTest::addColumn<QList<Channels::ChannelInfo>>("channels");

    {
        QMap<QString, QByteArray> mentions;
        QList<Channels::ChannelInfo> lst;
        QTest::newRow("empty") << QString() << QString() << mentions << lst;
    }
    {
        QMap<QString, QByteArray> mentions;
        mentions.insert(QStringLiteral("kde"), "bb"_ba);
        QList<Channels::ChannelInfo> lst;
        Channels::ChannelInfo info;
        info.name = QStringLiteral("foo");
        info.identifier = "idd"_ba;
        lst.append(info);
        QTest::newRow("channel-user1") << QStringLiteral("#foo @kde")
                                       << QStringLiteral("<p><a href='ruqola:/room/idd'>#foo</a> <a href='ruqola:/user/bb'>@kde</a></p>\n") << mentions << lst;
    }

    {
        QMap<QString, QByteArray> mentions;
        mentions.insert(QStringLiteral("kde1"), "bb"_ba);
        QList<Channels::ChannelInfo> lst;
        Channels::ChannelInfo info;
        info.name = QStringLiteral("foo2");
        info.identifier = "idd"_ba;
        lst.append(info);
        QTest::newRow("channel-user-unknown") << QStringLiteral("#foo @kde")
                                              << QStringLiteral("<p><a href='ruqola:/room/foo'>#foo</a> <a href='ruqola:/user/kde'>@kde</a></p>\n") << mentions
                                              << lst;
    }

    {
        QMap<QString, QByteArray> mentions;
        mentions.insert(QStringLiteral("nico"), "bb"_ba);
        mentions.insert(QStringLiteral("nico.bla"), "bb_new"_ba);
        QList<Channels::ChannelInfo> lst;
        QTest::newRow("quasi same name") << QStringLiteral("@nico foo @nico.bla")
                                         << QStringLiteral("<p><a href='ruqola:/user/bb'>@nico</a> foo <a href='ruqola:/user/bb_new'>@nico.bla</a></p>\n")
                                         << mentions << lst;
    }

    {
        QMap<QString, QByteArray> mentions;
        mentions.insert(QStringLiteral("nico"), "bb"_ba);
        mentions.insert(QStringLiteral("nico.bla"), "bb_new"_ba);
        QList<Channels::ChannelInfo> lst;
        QTest::newRow("quasi same name-2") << QStringLiteral("foo @nico.bla") << QStringLiteral("<p>foo <a href='ruqola:/user/bb_new'>@nico.bla</a></p>\n")
                                           << mentions << lst;
    }

    {
        QMap<QString, QByteArray> mentions;
        mentions.insert(QStringLiteral("nico"), "bb"_ba);
        mentions.insert(QStringLiteral("nico.bla"), "bb_new"_ba);
        QList<Channels::ChannelInfo> lst;
        QTest::newRow("quasi same name-1") << QStringLiteral("@nico foo") << QStringLiteral("<p><a href='ruqola:/user/bb'>@nico</a> foo</p>\n") << mentions
                                           << lst;
    }
}

void TextConverterTest::shouldShowSearchedText_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("username");
    QTest::addColumn<QStringList>("highlightWords");
    QTest::addColumn<QString>("searchedText");
    QTest::addColumn<QString>("output");
    QTest::newRow("empty") << QString() << QString() << QStringList{} << QString() << QString();
    const QStringList highlightWords{QStringLiteral("ruqola"), QStringLiteral("kde")};
    QTest::newRow("lowercase") << QStringLiteral("Ruqola") << QStringLiteral("foo") << highlightWords << QString()
                               << QStringLiteral("<p><a style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">Ruqola</a></p>\n");
    QTest::newRow("two-word") << QStringLiteral("Ruqola kde") << QStringLiteral("foo") << highlightWords << QString()
                              << QStringLiteral(
                                     "<p><a style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">Ruqola</a> <a "
                                     "style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">kde</a></p>\n");

    QTest::newRow("words") << QStringLiteral("Ruqola bla kde KDE.") << QStringLiteral("foo") << highlightWords << QString()
                           << QStringLiteral(
                                  "<p><a style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">Ruqola</a> bla <a "
                                  "style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">kde</a> <a "
                                  "style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">KDE</a>.</p>\n");

    QTest::newRow("wordinurl") << QStringLiteral("https://www.kde.org/~/bla/bli.txt") << QStringLiteral("bla") << highlightWords << QString()
                               << QStringLiteral(
                                      "<p><a href=\"https://www.kde.org/~/bla/bli.txt\">https://www.<a "
                                      "style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">kde</a>.org/~/bla/bli.txt</a></p>\n");
    QTest::newRow("channelruqola")
        << QStringLiteral("#ruqola-bla bla kde KDE.") << QStringLiteral("foo") << highlightWords << QString()
        << QStringLiteral(
               "<p><a href='ruqola:/room/ruqola-bla'>#<a style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">ruqola</a>-bla</a> bla <a "
               "style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">kde</a> <a "
               "style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">KDE</a>.</p>\n");
}

void TextConverterTest::shouldShowSearchedText()
{
    QFETCH(QString, input);
    QFETCH(QString, username);
    QFETCH(QStringList, highlightWords);
    QFETCH(QString, searchedText);
    QFETCH(QString, output);

    output = prepareExpectedOutput(output);

    QByteArray needUpdateMessageId;
    int recursiveIndex = 0;
    int numberOfTextSearched = 0;
    int hightLightStringIndex = 0;
    const TextConverter::ConvertMessageTextSettings settings(input, username, {}, highlightWords, nullptr, nullptr, {}, nullptr, {}, searchedText);
    QCOMPARE(TextConverter::convertMessageText(settings, needUpdateMessageId, recursiveIndex, numberOfTextSearched, hightLightStringIndex), output);
}

#include "moc_textconvertertest.cpp"
