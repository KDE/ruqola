/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "textconvertertest.h"

#include "emoticons/emojimanager.h"
#include "messages/message.h"
#include "ownuser/ownuserpreferences.h"
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
    const KColorScheme scheme;
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
    QTest::newRow("simpletext") << u"foo"_s << u"<p>foo</p>\n"_s;
    QTest::newRow("customemojiwithoutmanager") << u":foo:"_s << u"<p>:foo:</p>\n"_s;

    QTest::newRow("testurl") << u"http://www.kde.org http://www.kde.org"_s
                             << u"<p><a href=\"http://www.kde.org\">http://www.kde.org</a> <a href=\"http://www.kde.org\">http://www.kde.org</a></p>\n"_s;
    QTest::newRow("named-url") << u"[example.com](http://example.com)"_s << u"<p><a href=\"http://example.com\">example.com</a></p>\n"_s;
    QTest::newRow("unresolved quoted message") << u"[ ](https://example.com/channel/general?msg=quotedMessageId)\nVisible message"_s
                                               << u"<p>Visible message</p>\n"_s;
    QTest::newRow("bold") << u"*bla*"_s << u"<p><strong>bla</strong></p>\n"_s;
    QTest::newRow("multi star") << u"**bla**"_s << u"<p><strong>bla</strong></p>\n"_s;
    QTest::newRow("multi star2") << u"***bla***"_s << u"<p><strong><strong>bla</strong></strong></p>\n"_s;
    QTest::newRow("multi star3") << u"***bla ******"_s << u"<p>***bla ******</p>\n"_s;
    QTest::newRow("Remove <br/>") << u"foo<br />"_s << u"<p>foo</p>\n"_s;
    QTest::newRow("0.6.3") << u"0.6.3"_s << u"<p>0.6.3</p>\n"_s;
    // Bug 391520
    QTest::newRow("multi-line") << u"These are the options:\n- a\n- b"_s << u"<p>These are the options:</p>\n<ul>\n<li>a</li>\n<li>b</li>\n</ul>\n"_s;

    QTest::newRow("word@") << u"@foo"_s << u"<p><a href='ruqola:/user/foo'>@foo</a></p>\n"_s;
    QTest::newRow("word@-2") << u"@foo.bla"_s << u"<p><a href='ruqola:/user/foo.bla'>@foo.bla</a></p>\n"_s;
    QTest::newRow("word@-3") << u"@foo.bla.bli"_s << u"<p><a href='ruqola:/user/foo.bla.bli'>@foo.bla.bli</a></p>\n"_s;
    QTest::newRow("word@-4") << u"@foo.bla.bli dd"_s << u"<p><a href='ruqola:/user/foo.bla.bli'>@foo.bla.bli</a> dd</p>\n"_s;
    QTest::newRow("word@-5") << u"bla bla 21 @foo.bla.bli dd"_s << u"<p>bla bla 21 <a href='ruqola:/user/foo.bla.bli'>@foo.bla.bli</a> dd</p>\n"_s;
    QTest::newRow("word@-6") << u"@foo-bla"_s << u"<p><a href='ruqola:/user/foo-bla'>@foo-bla</a></p>\n"_s;
    QTest::newRow("word@-7") << u"@foo_bla"_s << u"<p><a href='ruqola:/user/foo_bla'>@foo_bla</a></p>\n"_s;
    QTest::newRow("word@-8") << u"bli@foo_bla"_s << u"<p>bli@foo_bla</p>\n"_s;
    QTest::newRow("word@-9") << u"@réunion"_s << u"<p><a href='ruqola:/user/réunion'>@réunion</a></p>\n"_s;

    QTest::newRow("word#") << u"#foo"_s << u"<p><a href='ruqola:/room/foo'>#foo</a></p>\n"_s;
    QTest::newRow("word#-2") << u"#foo.bla"_s << u"<p><a href='ruqola:/room/foo.bla'>#foo.bla</a></p>\n"_s;
    QTest::newRow("word#-3") << u"#foo.bla.bli"_s << u"<p><a href='ruqola:/room/foo.bla.bli'>#foo.bla.bli</a></p>\n"_s;
    QTest::newRow("word#-4") << u"#foo.bla.bli dd"_s << u"<p><a href='ruqola:/room/foo.bla.bli'>#foo.bla.bli</a> dd</p>\n"_s;
    QTest::newRow("word#-5") << u"bla bla 21 #foo.bla.bli dd"_s << u"<p>bla bla 21 <a href='ruqola:/room/foo.bla.bli'>#foo.bla.bli</a> dd</p>\n"_s;
    QTest::newRow("word#-6") << u"#foo-bla"_s << u"<p><a href='ruqola:/room/foo-bla'>#foo-bla</a></p>\n"_s;
    QTest::newRow("word#-7") << u"#foo_bla"_s << u"<p><a href='ruqola:/room/foo_bla'>#foo_bla</a></p>\n"_s;
    QTest::newRow("word#-8") << u"#réunion"_s << u"<p><a href='ruqola:/room/réunion'>#réunion</a></p>\n"_s;
    // Test parsing when it's in an url... don't replace it.

    QTest::newRow("url") << u"http://www.kde.org#foo_bla"_s << u"<p><a href=\"http://www.kde.org#foo_bla\">http://www.kde.org#foo_bla</a></p>\n"_s;

    QTest::newRow("`code`")
        << u"foo `bla` bar `asdf` test"_s
        << u"<p>foo <code style='background-color:$BGCOLOR$'>bla</code> bar <code style='background-color:$BGCOLOR$'>asdf</code> test</p>\n"_s;
    QTest::newRow("`code \\` escaped`") << u"foo `bla\\` bar \\`asdf` test"_s
                                        << u"<p>foo `bla\\` bar <code style='background-color:$BGCOLOR$'>asdf</code> test</p>\n"_s;
    QTest::newRow("quote<") << u"`inside quotes: <<....` this is outside the quotes <<."_s
                            << u"<p><code style='background-color:$BGCOLOR$'>inside quotes: &lt;&lt;....</code> this is outside the quotes &lt;&lt;.</p>\n"_s;
    QTest::newRow("quotedtext1") << u"> toto"_s << u"<p><code style='background-color:$BGCOLOR$'>toto</code></p>\n"_s;
    QTest::newRow("quotedtext2") << u"bla \n> toto"_s << u"<p>bla<br />\n<code style='background-color:$BGCOLOR$'>toto</code></p>\n"_s;

    QTest::newRow("quotedtext3")
        << u"bla \n> toto \n> bla"_s
        << u"<p>bla<br />\n<code style='background-color:$BGCOLOR$'>toto</code><br />\n<code style='background-color:$BGCOLOR$'>bla</code></p>\n"_s;

    QTest::newRow("quotedtext4") << u"bla \n> toto \n> *bla*"_s
                                 << u"<p>bla<br />\n<code style='background-color:$BGCOLOR$'>toto</code><br />\n<code "
                                    u"style='background-color:$BGCOLOR$'></code><strong>bla</strong></p>\n"_s;

    QTest::newRow("noquotedtext") << u"bla > toto"_s << u"<p>bla &gt; toto</p>\n"_s;

    QTest::newRow("code-with-language") << u"```ruby\nssss\n```"_s
                                        << u"<p><table><tr><td style='background-color:$BGCOLOR$; padding: 5px; border: 1px solid "
                                           u"$BORDERCOLOR$'><code>ssss</code></td></tr></table></p>\n"_s;

    QTest::newRow("code-with-language-2")
        << u"```dart\nimport 'package:flutter/material.dart';\nimport 'package:markdown/markdown.dart';\n```"_s
        << u"<p><table><tr><td style='background-color:$BGCOLOR$; padding: 5px; border: 1px solid $BORDERCOLOR$'><code><span "
           u"style=\"font-weight:bold;\">import</span>&nbsp;<span style=\"color:#bf0303;\">'package:flutter/material.dart'</span>;<br><span "
           u"style=\"font-weight:bold;\">import</span>&nbsp;<span "
           u"style=\"color:#bf0303;\">'package:markdown/markdown.dart'</span>;</code></td></tr></table></p>\n"_s;

    QTest::newRow("indented-code-block")
        << u"```\n  first line\n  second line\n```"_s
        << u"<p><table><tr><td style='background-color:$BGCOLOR$; padding: 5px; border: 1px solid "
           u"$BORDERCOLOR$'><code>&nbsp;&nbsp;first&nbsp;line<br>&nbsp;&nbsp;second&nbsp;line</code></td></tr></table></p>\n"_s;

    QTest::newRow("indented-code-block2")
        << u"```\n\tfirst line\t  second line\n```"_s
        << u"<p><table><tr><td style='background-color:#f7f7f7; padding: 5px; border: 1px solid "
           u"#707d8a'><code>&nbsp;&nbsp;&nbsp;&nbsp;first&nbsp;line&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;second&nbsp;line</code></td></tr></table></p>\n"_s;
    // We don't need to convert #1
    QTest::newRow("href + #") << u"[test #1](http://www.kde.org)"_s << u"<p><a href=\"http://www.kde.org\">test #1</a></p>\n"_s;

    // Bug we convert url with emoji
    QTest::newRow("url emoji") << u"http://www.kde.org/:b:/s/bla"_s << u"<p><a href=\"http://www.kde.org/:b:/s/bla\">http://www.kde.org/:b:/s/bla</a></p>\n"_s;
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
    const QStringList highlightWords{u"ruqola"_s, u"kde"_s};
    QTest::newRow("lowercase") << u"Ruqola"_s << u"foo"_s << highlightWords
                               << u"<p><a style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">Ruqola</a></p>\n"_s;
    QTest::newRow("two-word") << u"Ruqola kde"_s << u"foo"_s << highlightWords
                              << u"<p><a style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">Ruqola</a> <a "
                                 u"style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">kde</a></p>\n"_s;

    QTest::newRow("words") << u"Ruqola bla kde KDE."_s << u"foo"_s << highlightWords
                           << u"<p><a style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">Ruqola</a> bla <a "
                              u"style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">kde</a> <a "
                              u"style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">KDE</a>.</p>\n"_s;

    QTest::newRow("wordinurl") << u"https://www.kde.org/~/bla/bli.txt"_s << u"bla"_s << highlightWords
                               << u"<p><a href=\"https://www.kde.org/~/bla/bli.txt\">https://www.<a "
                                  u"style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">kde</a>.org/~/bla/bli.txt</a></p>\n"_s;
    QTest::newRow("channelruqola")
        << u"#ruqola-bla bla kde KDE."_s << u"foo"_s << highlightWords
        << u"<p><a href='ruqola:/room/ruqola-bla'>#<a style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">ruqola</a>-bla</a> bla <a "
           u"style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">kde</a> <a "
           u"style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">KDE</a>.</p>\n"_s;

    QTest::newRow("here1") << u"ruqola @here"_s << u"foo"_s << highlightWords
                           << u"<p><a style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">ruqola</a> <a "
                              u"style=\"color:$HERECOLOR$;background-color:$HEREBGCOLOR$;font-weight:bold\">here</a></p>\n"_s;
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
    const int hightLightStringIndex = 0;

    const TextConverter::ConvertMessageTextSettings
        settings(input, username, {}, OwnUserPreferences::generateRegularExpressions(highlightWords), nullptr, nullptr, {}, {}, {});
    QCOMPARE(TextConverter::convertMessageText(settings, needUpdateMessageId, recursiveIndex, numberOfTextSearched, hightLightStringIndex), output);
}

void TextConverterTest::shouldHighlightText_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("username");
    QTest::addColumn<QString>("output");
    QTest::newRow("empty") << QString() << QString() << QString();
    QTest::newRow("word@1") << u"@foo"_s << QString() << u"<p><a href='ruqola:/user/foo'>@foo</a></p>\n"_s;
    const KColorScheme colorScheme;
    const auto userMentionForegroundColor = colorScheme.foreground(KColorScheme::NegativeText).color().name();
    const auto userMentionBackgroundColor = colorScheme.background(KColorScheme::NegativeBackground).color().name();
    QTest::newRow("word@1-username") << u"@foo"_s << u"foo"_s
                                     << u"<p><a href='ruqola:/user/foo' style=\"color:%1;background-color:%2;font-weight:bold\">@foo</a></p>\n"_s.arg(
                                            userMentionForegroundColor,
                                            userMentionBackgroundColor);
    QTest::newRow("word@2-username") << u"bla bla @foo"_s << u"foo"_s
                                     << u"<p>bla bla <a href='ruqola:/user/foo' style=\"color:%1;background-color:%2;font-weight:bold\">@foo</a></p>\n"_s.arg(
                                            userMentionForegroundColor,
                                            userMentionBackgroundColor);
    // Mentions taking different branches, so the replacements have different lengths: checks
    // that the offset used to walk the string stays in sync.
    const auto hereAllMentionForegroundColor = colorScheme.foreground(KColorScheme::NeutralText).color().name();
    const auto hereAllMentionBackgroundColor = colorScheme.background(KColorScheme::NeutralBackground).color().name();
    QTest::newRow("mixed mentions") << u"@foo bla @here bli @other"_s << u"foo"_s
                                    << u"<p><a href='ruqola:/user/foo' style=\"color:%1;background-color:%2;font-weight:bold\">@foo</a> bla <a "
                                       u"style=\"color:%3;background-color:%4;font-weight:bold\">here</a> bli <a "
                                       u"href='ruqola:/user/other'>@other</a></p>\n"_s.arg(userMentionForegroundColor,
                                                                                           userMentionBackgroundColor,
                                                                                           hereAllMentionForegroundColor,
                                                                                           hereAllMentionBackgroundColor);
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
    const int hightLightStringIndex = 0;
    const TextConverter::ConvertMessageTextSettings settings(input, username, {}, {}, nullptr, nullptr, {}, {}, {});
    QCOMPARE(TextConverter::convertMessageText(settings, needUpdateMessageId, recursiveIndex, numberOfTextSearched, hightLightStringIndex), output);
}

void TextConverterTest::shouldConvertTextWithEmoji_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");
    QTest::addColumn<QString>("serverUrl");

    QTest::newRow("empty") << QString() << QString() << u"www.kde.org"_s;
    QTest::newRow("bold") << u"*foo*"_s << u"<p><strong>foo</strong></p>\n"_s << u"www.kde.org"_s;
    QTest::newRow("italic") << u"_foo_"_s << u"<p><em>foo</em></p>\n"_s << u"www.kde.org"_s;
    // TODO error
    QTest::newRow("italic2") << u"_personal: theming related tasks_"_s << u"<p><em>personal: theming related tasks</em></p>\n"_s << u"www.kde.org"_s;
    QTest::newRow("simpletext") << u"foo"_s << u"<p>foo</p>\n"_s << u"www.kde.org"_s;
    QTest::newRow("customemojiwithmanager") << u":foo:"_s << u"<p>:foo:</p>\n"_s << u"www.kde.org"_s;

    QString smiley;
    smiley += QChar(0xD83D);
    smiley += QChar(0xDE42);
    // The title is the identifier of the emoji, which ktextaddons takes from emojibase
    // since 2.1.45; before that :slightly_smiling_face: was only an alias.
#if TEXTEMOTICONSCORE_VERSION >= QT_VERSION_CHECK(2, 1, 45)
    const auto smileyIdentifier = u":slightly_smiling_face:"_s;
#else
    const auto smileyIdentifier = u":slight_smile:"_s;
#endif
    const auto smileyText = u"<span style=\"font: x-large %2\" title=\"%3\">%1</span>"_s.arg(smiley, Utils::emojiFontName(), smileyIdentifier);
    QTest::newRow("ascii-smiley") << u":)"_s << u"<p>%1</p>\n"_s.arg(smileyText) << u"www.kde.org"_s;
    QTest::newRow("multi-smiley") << u":):slightly_smiling_face::):)"_s << u"<p>%1</p>\n"_s.arg(smileyText.repeated(4)) << u"www.kde.org"_s;

    QTest::newRow("url") << u"https://www.kde.org"_s << u"<p><a href=\"https://www.kde.org\">https://www.kde.org</a></p>\n"_s << u"www.kde.org"_s;

    QTest::newRow("\")") << u"\")"_s << u"<p>&quot;)</p>\n"_s << u"www.kde.org"_s;

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

    QTest::newRow("quotedcode1") << u"bla\n```\nfoo\n```\nblub"_s
                                 << u"<p>bla</p>\n<p><table><tr><td style='background-color:$BGCOLOR$; padding: 5px; border: 1px solid "
                                    u"$BORDERCOLOR$'><code>foo</code></td></tr></table></p>\n<p>blub</p>\n"_s
                                 << u"www.kde.org"_s;

    QTest::newRow("quotedcode2") << u"bla\n```\nfoo\n```\nbli"_s
                                 << u"<p>bla</p>\n<p><table><tr><td style='background-color:$BGCOLOR$; padding: 5px; border: 1px solid "
                                    u"$BORDERCOLOR$'><code>foo</code></td></tr></table></p>\n<p>bli</p>\n"_s
                                 << u"www.kde.org"_s;
    QTest::newRow("quotedcode3") << u"bla\n```\nfoo\n```"_s
                                 << u"<p>bla</p>\n<p><table><tr><td style='background-color:$BGCOLOR$; padding: 5px; border: 1px solid "
                                    u"$BORDERCOLOR$'><code>foo</code></td></tr></table></p>\n"_s
                                 << u"www.kde.org"_s;
    QTest::newRow("quotedcode4") << u"```\nfoo\n```\nff"_s
                                 << u"<p><table><tr><td style='background-color:$BGCOLOR$; padding: 5px; border: 1px solid "
                                    u"$BORDERCOLOR$'><code>foo</code></td></tr></table></p>\n<p>ff</p>\n"_s
                                 << u"www.kde.org"_s;

    QTest::newRow("quotedcode5") << u"bla\n```\nfoo\n```\nff"_s
                                 << u"<p>bla</p>\n<p><table><tr><td style='background-color:$BGCOLOR$; padding: 5px; border: 1px solid "
                                    u"$BORDERCOLOR$'><code>foo</code></td></tr></table></p>\n<p>ff</p>\n"_s
                                 << u"www.kde.org"_s;

    QTest::newRow("quotedcode6") << u"*foo*\n```\nfoo\n```\n*bar*\n```\nblub\n```\n*asdf*"_s
                                 << u"<p><strong>foo</strong></p>\n<p><table><tr><td style='background-color:$BGCOLOR$; padding: 5px; border: 1px solid "
                                    u"$BORDERCOLOR$'><code>foo</code></td></tr></table></p>\n<p><strong>bar</strong></p>\n<p><table><tr><td "
                                    u"style='background-color:$BGCOLOR$; padding: 5px; border: "
                                    u"1px solid $BORDERCOLOR$'><code>blub</code></td></tr></table></p>\n<p><strong>asdf</strong></p>\n"_s
                                 << u"www.kde.org"_s;
    QTest::newRow("quotedcode7") << u":)\n```\n:)\n```\n:)"_s
                                 << u"<p>%1</p>\n<p><table><tr><td style='background-color:$BGCOLOR$; padding: 5px; border: 1px solid "
                                    u"$BORDERCOLOR$'><code>:)</code></td></tr></table></p>\n<p>%1</p>\n"_s.arg(smileyText)
                                 << u"www.kde.org"_s;
    QTest::newRow("quotedcode8")
        << u"```javascript\ncode\n```"_s
        << u"<p><table><tr><td style='background-color:$BGCOLOR$; padding: 5px; border: 1px solid $BORDERCOLOR$'><code>code</code></td></tr></table></p>\n"_s
        << u"www.kde.org"_s;

    QTest::newRow("quotedcode9") << u"```\nblub\ncode\n```"_s
                                 << u"<p><table><tr><td style='background-color:$BGCOLOR$; padding: 5px; border: 1px solid "
                                    u"$BORDERCOLOR$'><code>blub<br>code</code></td></tr></table></p>\n"_s
                                 << u"www.kde.org"_s;
    QTest::newRow("inline-code-with-brackets")
        << u"`[[test]]` and `a[b` something"_s
        << u"<p><code style='background-color:$BGCOLOR$'>[[test]]</code> and <code style='background-color:$BGCOLOR$'>a[b</code> "
           u"something</p>\n"_s
        << u"www.kde.org"_s;

    QTest::newRow("inline-code-with-tilde") << u"`auto [a, b] = std::minmax_element(~~~);`"_s
                                            << u"<p><code style='background-color:$BGCOLOR$'>auto [a, b] = "
                                               u"std::minmax_element(~~~);</code></p>\n"_s
                                            << u"www.kde.org"_s;

    QTest::newRow("inline-code-with-smiley") << u":) `:)` :)"_s << u"<p>%1 <code style='background-color:$BGCOLOR$'>:)</code> %1</p>\n"_s.arg(smileyText)
                                             << u"www.kde.org"_s;

    QTest::newRow("url-with-emoji") << u"https://www.kde.org/:x:/bla.html"_s
                                    << u"<p>%1 <code style='background-color:$BGCOLOR$'>:)</code> %1</p>\n"_s.arg(smileyText) << u"www.kde.org"_s;
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
    const int hightLightStringIndex = 0;
    const TextConverter::ConvertMessageTextSettings settings(input, QString(), {}, {}, &manager, nullptr, {}, {}, {});
    auto actualOutput = TextConverter::convertMessageText(settings, needUpdateMessageId, recursiveIndex, numberOfTextSearched, hightLightStringIndex);
    if (QLatin1StringView(QTest::currentDataTag()) == "quotedcode7"_L1) {
        // remove additional highlighting of the ':)' symbols within the <code> block
        // the text color is syntax highlighting theme dependent, so hard for us to check
        static const auto codespan = QRegularExpression(u"<code><span style=\".+\">:\\)</span></code>"_s);
        actualOutput.replace(codespan, u"<code>:)</code>"_s);
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
        const QMap<QString, QByteArray> mentions;
        const QList<Channels::ChannelInfo> channels;
        QTest::newRow("empty") << QString() << QString() << mentions << channels;
    }
    {
        const QMap<QString, QByteArray> mentions;
        QList<Channels::ChannelInfo> channels;
        Channels::ChannelInfo info;
        info.name = u"foo"_s;
        info.identifier = "idd"_ba;
        channels.append(info);
        QTest::newRow("word#") << u"#foo"_s << u"<p><a href='ruqola:/room/idd'>#foo</a></p>\n"_s << mentions << channels;
    }
    {
        const QMap<QString, QByteArray> mentions;
        QList<Channels::ChannelInfo> channels;
        Channels::ChannelInfo info;
        info.name = u"bla"_s;
        info.identifier = "idd"_ba;
        channels.append(info);
        QTest::newRow("not existing room") << u"#foo"_s << u"<p><a href='ruqola:/room/foo'>#foo</a></p>\n"_s << mentions << channels;
    }
    {
        const QMap<QString, QByteArray> mentions;
        QList<Channels::ChannelInfo> channels;
        Channels::ChannelInfo info;
        info.name = u"bla"_s;
        info.fname = u"FNAME"_s;
        info.identifier = "idd"_ba;
        channels.append(info);
        QTest::newRow("use fname") << u"#bla"_s << u"<p><a href='ruqola:/room/idd'>#FNAME</a></p>\n"_s << mentions << channels;
    }
    {
        const QMap<QString, QByteArray> mentions;

        QList<Channels::ChannelInfo> channels;
        {
            Channels::ChannelInfo info;
            info.name = u"bli"_s;
            info.identifier = "112"_ba;
            channels.append(info);
        }
        {
            Channels::ChannelInfo info;
            info.name = u"oss"_s;
            info.identifier = "kli"_ba;
            channels.append(info);
        }
        QTest::newRow("multi channel") << u"foo #bli blass #oss"_s
                                       << u"<p>foo <a href='ruqola:/room/112'>#bli</a> blass <a href='ruqola:/room/kli'>#oss</a></p>\n"_s << mentions
                                       << channels;
    }
    {
        const QMap<QString, QByteArray> mentions;
        QList<Channels::ChannelInfo> channels;
        Channels::ChannelInfo info;
        info.name = u"bli"_s;
        info.identifier = "112"_ba;
        channels.append(info);
        QTest::newRow("same channel twice") << u"foo #bli blass #bli"_s
                                            << u"<p>foo <a href='ruqola:/room/112'>#bli</a> blass <a href='ruqola:/room/112'>#bli</a></p>\n"_s << mentions
                                            << channels;
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
    const int hightLightStringIndex = 0;
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
    const int hightLightStringIndex = 0;
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
        const QMap<QString, QByteArray> mentions;
        const QList<Channels::ChannelInfo> lst;
        QTest::newRow("empty") << QString() << QString() << mentions << lst;
    }
    {
        QMap<QString, QByteArray> mentions;
        mentions.insert(u"kde"_s, "bb"_ba);
        QList<Channels::ChannelInfo> lst;
        Channels::ChannelInfo info;
        info.name = u"foo"_s;
        info.identifier = "idd"_ba;
        lst.append(info);
        QTest::newRow("channel-user1") << u"#foo @kde"_s << u"<p><a href='ruqola:/room/idd'>#foo</a> <a href='ruqola:/user/kde'>@kde</a></p>\n"_s << mentions
                                       << lst;
    }

    {
        QMap<QString, QByteArray> mentions;
        mentions.insert(u"kde1"_s, "bb"_ba);
        QList<Channels::ChannelInfo> lst;
        Channels::ChannelInfo info;
        info.name = u"foo2"_s;
        info.identifier = "idd"_ba;
        lst.append(info);
        QTest::newRow("channel-user-unknown") << u"#foo @kde"_s << u"<p><a href='ruqola:/room/foo'>#foo</a> <a href='ruqola:/user/kde'>@kde</a></p>\n"_s
                                              << mentions << lst;
    }

    {
        QMap<QString, QByteArray> mentions;
        mentions.insert(u"nico"_s, "bb"_ba);
        mentions.insert(u"nico.bla"_s, "bb_new"_ba);
        const QList<Channels::ChannelInfo> lst;
        QTest::newRow("quasi same name") << u"@nico foo @nico.bla"_s
                                         << u"<p><a href='ruqola:/user/nico'>@nico</a> foo <a href='ruqola:/user/nico.bla'>@nico.bla</a></p>\n"_s << mentions
                                         << lst;
    }

    {
        QMap<QString, QByteArray> mentions;
        mentions.insert(u"nico"_s, "bb"_ba);
        mentions.insert(u"nico.bla"_s, "bb_new"_ba);
        const QList<Channels::ChannelInfo> lst;
        QTest::newRow("quasi same name-2") << u"foo @nico.bla"_s << u"<p>foo <a href='ruqola:/user/nico.bla'>@nico.bla</a></p>\n"_s << mentions << lst;
    }

    {
        QMap<QString, QByteArray> mentions;
        mentions.insert(u"nico"_s, "bb"_ba);
        mentions.insert(u"nico.bla"_s, "bb_new"_ba);
        const QList<Channels::ChannelInfo> lst;
        QTest::newRow("quasi same name-1") << u"@nico foo"_s << u"<p><a href='ruqola:/user/nico'>@nico</a> foo</p>\n"_s << mentions << lst;
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
    const QStringList highlightWords{u"ruqola"_s, u"kde"_s};
    QTest::newRow("lowercase") << u"Ruqola"_s << u"foo"_s << highlightWords << QString()
                               << u"<p><a style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">Ruqola</a></p>\n"_s;
    QTest::newRow("two-word") << u"Ruqola kde"_s << u"foo"_s << highlightWords << QString()
                              << u"<p><a style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">Ruqola</a> <a "
                                 u"style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">kde</a></p>\n"_s;

    QTest::newRow("words") << u"Ruqola bla kde KDE."_s << u"foo"_s << highlightWords << QString()
                           << u"<p><a style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">Ruqola</a> bla <a "
                              u"style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">kde</a> <a "
                              u"style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">KDE</a>.</p>\n"_s;

    QTest::newRow("wordinurl") << u"https://www.kde.org/~/bla/bli.txt"_s << u"bla"_s << highlightWords << QString()
                               << u"<p><a href=\"https://www.kde.org/~/bla/bli.txt\">https://www.<a "
                                  u"style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">kde</a>.org/~/bla/bli.txt</a></p>\n"_s;
    QTest::newRow("searched-text") << u"Ruqola bla"_s << u"foo"_s << QStringList{} << u"bla"_s
                                   << u"<p>Ruqola <a style=\"color:$HERECOLOR$;background-color:$HEREBGCOLOR$;\">bla</a></p>\n"_s;
    QTest::newRow("searched-text-with-highlight") << u"Ruqola bla"_s << u"foo"_s << highlightWords << u"bla"_s
                                                  << u"<p><a style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">Ruqola</a> <a "
                                                     u"style=\"color:$HERECOLOR$;background-color:$HEREBGCOLOR$;\">bla</a></p>\n"_s;
    QTest::newRow("channelruqola")
        << u"#ruqola-bla bla kde KDE."_s << u"foo"_s << highlightWords << QString()
        << u"<p><a href='ruqola:/room/ruqola-bla'>#<a style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">ruqola</a>-bla</a> bla <a "
           u"style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">kde</a> <a "
           u"style=\"color:$USERCOLOR$;background-color:$USERBGCOLOR$;\">KDE</a>.</p>\n"_s;
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
    const int hightLightStringIndex = 0;
    const TextConverter::ConvertMessageTextSettings
        settings(input, username, {}, OwnUserPreferences::generateRegularExpressions(highlightWords), nullptr, nullptr, {}, nullptr, {}, searchedText);
    QCOMPARE(TextConverter::convertMessageText(settings, needUpdateMessageId, recursiveIndex, numberOfTextSearched, hightLightStringIndex), output);
}

#include "moc_textconvertertest.cpp"
