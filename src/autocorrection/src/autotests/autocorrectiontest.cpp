/*
  SPDX-FileCopyrightText: 2014-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "autocorrectiontest.h"
#include "../autocorrection.h"
#include "settings/textautocorrectionsettings.h"
#include <QStandardPaths>
#include <QTest>
#include <QTextDocument>
QTEST_MAIN(AutoCorrectionTest)

#ifndef Q_OS_WIN
void initLocale()
{
    setenv("LC_ALL", "en_US.utf-8", 1);
}

Q_CONSTRUCTOR_FUNCTION(initLocale)
#endif

AutoCorrectionTest::AutoCorrectionTest()
{
    QStandardPaths::setTestModeEnabled(true);
    mConfig = KSharedConfig::openConfig(QStringLiteral("autocorrectiontestrc"));
    TextAutoCorrection::TextAutoCorrectionSettings::self()->setSharedConfig(mConfig);
    TextAutoCorrection::TextAutoCorrectionSettings::self()->load();
}

AutoCorrectionTest::~AutoCorrectionTest() = default;

void AutoCorrectionTest::shouldHaveDefaultValue()
{
    TextAutoCorrection::AutoCorrection autocorrection;
    QVERIFY(!autocorrection.autoCorrectionSettings()->isEnabledAutoCorrection());
    QVERIFY(!autocorrection.autoCorrectionSettings()->isUppercaseFirstCharOfSentence());
    QVERIFY(!autocorrection.autoCorrectionSettings()->isFixTwoUppercaseChars());
    QVERIFY(!autocorrection.autoCorrectionSettings()->isSingleSpaces());
    QVERIFY(!autocorrection.autoCorrectionSettings()->isAutoFractions());
    QVERIFY(!autocorrection.autoCorrectionSettings()->isCapitalizeWeekDays());
    QVERIFY(!autocorrection.autoCorrectionSettings()->isReplaceDoubleQuotes());
    QVERIFY(!autocorrection.autoCorrectionSettings()->isReplaceSingleQuotes());
    QVERIFY(!autocorrection.autoCorrectionSettings()->isAdvancedAutocorrect());
    QVERIFY(!autocorrection.autoCorrectionSettings()->isAutoFormatUrl());
    QVERIFY(!autocorrection.autoCorrectionSettings()->isAutoBoldUnderline());
    QVERIFY(!autocorrection.autoCorrectionSettings()->isSuperScript());
    QVERIFY(!autocorrection.autoCorrectionSettings()->isAddNonBreakingSpace());
    QVERIFY(!autocorrection.autoCorrectionSettings()->isReplaceDoubleQuotesByFrenchQuotes());
}

void AutoCorrectionTest::shouldRestoreValue()
{
    TextAutoCorrection::AutoCorrection autocorrection;
    // TODO
}

void AutoCorrectionTest::shouldUpperCaseFirstCharOfSentence()
{
    TextAutoCorrection::AutoCorrection autocorrection;
    auto settings = new TextAutoCorrection::AutoCorrectionSettings;
    settings->setEnabledAutoCorrection(true);
    settings->setUppercaseFirstCharOfSentence(true);
    autocorrection.setAutoCorrectionSettings(settings);

    // Uppercase here.
    QTextDocument doc;
    QString text = QStringLiteral("foo");
    doc.setPlainText(text);
    int position = text.length();
    autocorrection.autocorrect(false, doc, position);
    QCOMPARE(doc.toPlainText(), QStringLiteral("Foo"));

    // IT's not first char -> not uppercase
    text = QStringLiteral(" foo");
    doc.setPlainText(text);
    position = text.length();
    autocorrection.autocorrect(false, doc, position);
    QCOMPARE(doc.toPlainText(), text);

    // It's already uppercase
    text = QStringLiteral("Foo");
    doc.setPlainText(text);
    position = text.length();
    autocorrection.autocorrect(false, doc, position);
    QCOMPARE(doc.toPlainText(), text);

    // Word is after a ". "
    text = QStringLiteral("Foo. foo");
    doc.setPlainText(text);
    position = text.length();
    autocorrection.autocorrect(false, doc, position);
    QCOMPARE(doc.toPlainText(), QStringLiteral("Foo. Foo"));
    QCOMPARE(position, text.length());
}

void AutoCorrectionTest::shouldFixTwoUpperCaseChars()
{
    TextAutoCorrection::AutoCorrection autocorrection;
    auto settings = new TextAutoCorrection::AutoCorrectionSettings;
    settings->setEnabledAutoCorrection(true);
    settings->setFixTwoUppercaseChars(true);
    autocorrection.setAutoCorrectionSettings(settings);

    // Remove two uppercases
    QTextDocument doc;
    QString text = QStringLiteral("FOo");
    doc.setPlainText(text);
    int position = text.length();
    autocorrection.autocorrect(false, doc, position);
    QCOMPARE(doc.toPlainText(), QStringLiteral("Foo"));

    // There is not two uppercase
    text = QStringLiteral("foo");
    doc.setPlainText(text);
    position = text.length();
    autocorrection.autocorrect(false, doc, position);
    QCOMPARE(doc.toPlainText(), text);

    text = QStringLiteral("Foo");
    doc.setPlainText(text);
    position = text.length();
    autocorrection.autocorrect(false, doc, position);
    QCOMPARE(doc.toPlainText(), text);

    // There is a uppercase word
    text = QStringLiteral("FOO");
    doc.setPlainText(text);
    position = text.length();
    autocorrection.autocorrect(false, doc, position);
    QCOMPARE(doc.toPlainText(), text);

    // Exclude 2 upper letter
    text = QStringLiteral("ABc");
    doc.setPlainText(text);
    position = text.length();
    autocorrection.autocorrect(false, doc, position);
    QCOMPARE(doc.toPlainText(), QStringLiteral("Abc"));

    QSet<QString> exception;
    exception.insert(QStringLiteral("ABc"));
    settings->setTwoUpperLetterExceptions(exception);
    autocorrection.setAutoCorrectionSettings(settings);
    text = QStringLiteral("ABc");
    doc.setPlainText(text);
    position = text.length();
    autocorrection.autocorrect(false, doc, position);
    QCOMPARE(doc.toPlainText(), text);
}

void AutoCorrectionTest::shouldReplaceSingleQuote()
{
    TextAutoCorrection::AutoCorrection autocorrection;
    auto settings = new TextAutoCorrection::AutoCorrectionSettings;
    settings->setEnabledAutoCorrection(true);
    settings->setReplaceSingleQuotes(true);
    autocorrection.setAutoCorrectionSettings(settings);

    TextAutoCorrection::AutoCorrectionUtils::TypographicQuotes simpleQuote;
    simpleQuote.begin = QLatin1Char('A');
    simpleQuote.end = QLatin1Char('B');

    settings->setTypographicSingleQuotes(simpleQuote);
    autocorrection.setAutoCorrectionSettings(settings);

    QTextDocument doc;
    QString text = QStringLiteral("sss");
    doc.setPlainText(QStringLiteral("'") + text);
    int position = text.length();
    autocorrection.autocorrect(false, doc, position);
    QCOMPARE(doc.toPlainText(), QString(simpleQuote.begin + text));

    text = QStringLiteral("sss");
    doc.setPlainText(text + QStringLiteral("'"));
    position = text.length();
    autocorrection.autocorrect(false, doc, position);
    QCOMPARE(doc.toPlainText(), QString(text + simpleQuote.end));

    text = QStringLiteral("sss");
    doc.setPlainText(QStringLiteral("'") + text + QStringLiteral("'"));
    position = text.length();
    autocorrection.autocorrect(false, doc, position);
    QCOMPARE(doc.toPlainText(), QString(simpleQuote.begin + text + simpleQuote.end));
}

void AutoCorrectionTest::shouldReplaceDoubleQuote()
{
    TextAutoCorrection::AutoCorrection autocorrection;
    auto settings = new TextAutoCorrection::AutoCorrectionSettings;
    settings->setEnabledAutoCorrection(true);
    settings->setReplaceDoubleQuotes(true);
    TextAutoCorrection::AutoCorrectionUtils::TypographicQuotes doubleQuote;
    doubleQuote.begin = QLatin1Char('A');
    doubleQuote.end = QLatin1Char('B');

    settings->setTypographicDoubleQuotes(doubleQuote);
    autocorrection.setAutoCorrectionSettings(settings);

    QTextDocument doc;
    QString text = QStringLiteral("sss");

    doc.setPlainText(QLatin1Char('"') + text);
    int position = text.length();
    autocorrection.autocorrect(false, doc, position);
    QCOMPARE(doc.toPlainText(), QString(doubleQuote.begin + text));

    text = QStringLiteral("sss");
    doc.setPlainText(text + QStringLiteral("\""));
    position = text.length();
    autocorrection.autocorrect(false, doc, position);
    QCOMPARE(doc.toPlainText(), QString(text + doubleQuote.end));

    text = QStringLiteral("sss");
    doc.setPlainText(QStringLiteral("\"") + text + QStringLiteral("\""));
    position = text.length();
    autocorrection.autocorrect(false, doc, position);
    QCOMPARE(doc.toPlainText(), QString(doubleQuote.begin + text + doubleQuote.end));
}

void AutoCorrectionTest::shouldNotReplaceUppercaseLetter()
{
    TextAutoCorrection::AutoCorrection autocorrection;
    auto settings = new TextAutoCorrection::AutoCorrectionSettings;
    settings->setEnabledAutoCorrection(true);
    settings->setFixTwoUppercaseChars(true);
    autocorrection.setAutoCorrectionSettings(settings);
    QSet<QString> exceptions;
    exceptions.insert(QStringLiteral("ABc"));
    settings->setTwoUpperLetterExceptions(exceptions);
    autocorrection.setAutoCorrectionSettings(settings);

    QTextDocument doc;
    QString text = QStringLiteral("foo ABc");
    doc.setPlainText(text);
    int position = text.length();
    autocorrection.autocorrect(false, doc, position);
    QCOMPARE(doc.toPlainText(), text);
}

void AutoCorrectionTest::shouldReplaceToTextFormat()
{
    TextAutoCorrection::AutoCorrection autocorrection;
    auto settings = new TextAutoCorrection::AutoCorrectionSettings;
    settings->setEnabledAutoCorrection(true);
    settings->setAutoBoldUnderline(true);
    autocorrection.setAutoCorrectionSettings(settings);

    QTextDocument doc;
    // We don't use html => don't change it.
    QString text = QStringLiteral("*foo*");
    doc.setHtml(text);
    int position = text.length();
    autocorrection.autocorrect(false, doc, position);
    QCOMPARE(doc.toPlainText(), text);

    QString result = QStringLiteral("foo");
    doc.setHtml(text);
    position = text.length();
    autocorrection.autocorrect(true, doc, position);
    QCOMPARE(doc.toPlainText(), result);

    text = QStringLiteral("*foo*");
    doc.setHtml(text);
    position = text.length();
    autocorrection.autocorrect(true, doc, position);
    QCOMPARE(doc.toPlainText(), result);
    QTextCursor cursor(&doc);
    cursor.setPosition(2);
    QTextCharFormat charFormat = cursor.charFormat();
    QCOMPARE(charFormat.font().underline(), false);
    QCOMPARE(charFormat.font().bold(), true);
    QCOMPARE(charFormat.font().strikeOut(), false);

    text = QStringLiteral("_foo_");
    doc.setHtml(text);
    position = text.length();
    autocorrection.autocorrect(true, doc, position);
    QCOMPARE(doc.toPlainText(), result);
    cursor = QTextCursor(&doc);
    cursor.setPosition(2);
    charFormat = cursor.charFormat();
    QCOMPARE(charFormat.font().underline(), true);
    QCOMPARE(charFormat.font().bold(), false);
    QCOMPARE(charFormat.font().strikeOut(), false);

    text = QStringLiteral("-foo-");
    doc.setHtml(text);
    position = text.length();
    autocorrection.autocorrect(true, doc, position);
    QCOMPARE(doc.toPlainText(), result);
    cursor = QTextCursor(&doc);
    cursor.setPosition(2);
    charFormat = cursor.charFormat();
    QCOMPARE(charFormat.font().underline(), false);
    QCOMPARE(charFormat.font().bold(), false);
    QCOMPARE(charFormat.font().strikeOut(), true);

    // Don't convert it.
    text = QStringLiteral("-foo1");
    doc.setHtml(text);
    position = text.length();
    autocorrection.autocorrect(true, doc, position);
    QCOMPARE(doc.toPlainText(), text);

    text = QStringLiteral("*foo* blabla");
    position = 5;
    doc.setHtml(text);
    autocorrection.autocorrect(true, doc, position);
    result = QStringLiteral("foo blabla");
    QCOMPARE(doc.toPlainText(), result);
    QCOMPARE(position, 3);

    // TODO QCOMPARE(doc.toHtml(), text);
}

void AutoCorrectionTest::shouldReplaceAutoFraction()
{
    TextAutoCorrection::AutoCorrection autocorrection;
    auto settings = new TextAutoCorrection::AutoCorrectionSettings;
    settings->setEnabledAutoCorrection(true);
    settings->setAutoFractions(true);
    autocorrection.setAutoCorrectionSettings(settings);

    QTextDocument doc;
    QString text = QStringLiteral("1/2");
    doc.setPlainText(text);
    int position = text.length();
    autocorrection.autocorrect(false, doc, position);

    QCOMPARE(doc.toPlainText(), QStringLiteral("½"));

    QString suffix = QStringLiteral(" after");
    text = QStringLiteral("1/2");
    position = 3;
    text += suffix;
    doc.setPlainText(text);
    autocorrection.autocorrect(false, doc, position);
    QCOMPARE(doc.toPlainText(), QString(QStringLiteral("½") + suffix));
    QCOMPARE(position, 1);
}

void AutoCorrectionTest::shouldNotAddSpaceWhenWeAlreadyHaveASpace()
{
    TextAutoCorrection::AutoCorrection autocorrection;
    auto settings = new TextAutoCorrection::AutoCorrectionSettings;
    settings->setEnabledAutoCorrection(true);
    settings->setSingleSpaces(true);
    autocorrection.setAutoCorrectionSettings(settings);
    QTextDocument doc;
    // We already a space => don't allow to add more
    QString text = QStringLiteral("FOO ");
    doc.setPlainText(text);
    int position = text.length();
    bool result = autocorrection.autocorrect(false, doc, position);
    QCOMPARE(result, false);

    // We can add a space
    text = QStringLiteral("FOO");
    doc.setPlainText(text);
    position = text.length();
    result = autocorrection.autocorrect(false, doc, position);
    QCOMPARE(result, true);

    // We have a space => don't add it.
    text = QStringLiteral("FOO ");
    position = text.length();
    QString fullText = text + QStringLiteral("FOO");
    doc.setPlainText(fullText);
    result = autocorrection.autocorrect(false, doc, position);
    QCOMPARE(result, false);
}

void AutoCorrectionTest::shouldAutocorrectWord()
{
    TextAutoCorrection::AutoCorrection autocorrection;
    auto settings = new TextAutoCorrection::AutoCorrectionSettings;
    settings->setEnabledAutoCorrection(true);
    settings->setAdvancedAutocorrect(true);
    autocorrection.setAutoCorrectionSettings(settings);

    QTextDocument doc;
    // No changes
    QString text = QStringLiteral("FOOAA");
    doc.setPlainText(text);
    int position = text.length();
    int oldPosition = position;
    autocorrection.autocorrect(false, doc, position);
    QCOMPARE(doc.toPlainText(), text);
    QCOMPARE(position, oldPosition);

    // Convert word
    QHash<QString, QString> entries;
    const QString convertWord = QStringLiteral("BLABLA");
    entries.insert(text, convertWord);
    settings->setAutocorrectEntries(entries);
    autocorrection.setAutoCorrectionSettings(settings);
    text = QStringLiteral("FOOAA");
    doc.setPlainText(text);
    position = text.length();
    autocorrection.autocorrect(false, doc, position);
    QCOMPARE(doc.toPlainText(), convertWord);
    QCOMPARE(position, convertWord.length());

    QString suffix = QStringLiteral(" TOTO");
    text = QStringLiteral("FOOAA");
    position = text.length();
    text += suffix;
    doc.setPlainText(text);
    autocorrection.autocorrect(false, doc, position);
    QCOMPARE(doc.toPlainText(), QString(convertWord + suffix));
    // FIXME ? QCOMPARE(position, convertWord.length());
}

void AutoCorrectionTest::shouldNotUpperCaseFirstCharOfSentence()
{
    TextAutoCorrection::AutoCorrection autocorrection;
    auto settings = new TextAutoCorrection::AutoCorrectionSettings;
    settings->setEnabledAutoCorrection(true);
    settings->setUppercaseFirstCharOfSentence(true);
    autocorrection.setAutoCorrectionSettings(settings);
    QSet<QString> lst;
    lst.insert(QStringLiteral("Foo."));
    settings->setUpperCaseExceptions(lst);
    autocorrection.setAutoCorrectionSettings(settings);

    // Uppercase here.
    QTextDocument doc;
    QString text = QStringLiteral("foo. blabla Foo. tt");
    doc.setPlainText(text);
    int position = text.length();
    autocorrection.autocorrect(false, doc, position);
    QCOMPARE(doc.toPlainText(), text);

    settings->setUpperCaseExceptions(QSet<QString>());
    autocorrection.setAutoCorrectionSettings(settings);

    doc.setPlainText(text);
    position = text.length();
    autocorrection.autocorrect(false, doc, position);
    QString result = QStringLiteral("foo. blabla Foo. Tt");
    QCOMPARE(doc.toPlainText(), result);
}

using mapAutoCorrect = QHash<QString, QString>;
Q_DECLARE_METATYPE(mapAutoCorrect)

void AutoCorrectionTest::shouldAutocorrectMultiWord_data()
{
    QTest::addColumn<QString>("originalString");
    QTest::addColumn<QString>("convertedString");
    QTest::addColumn<mapAutoCorrect>("convertStringHash");

    mapAutoCorrect map;
    map.insert(QStringLiteral("boo"), QStringLiteral("bla"));
    QTest::newRow("simpleReplace") << QStringLiteral("boo") << QStringLiteral("bla") << map;

    map.clear();
    map.insert(QStringLiteral("a boo"), QStringLiteral("b bla"));
    QTest::newRow("multiword") << QStringLiteral("a boo") << QStringLiteral("b bla") << map;

    map.clear();
    map.insert(QStringLiteral("a boo"), QStringLiteral("b bla"));
    map.insert(QStringLiteral("a booeeeeeeeeeeeeeeeeeee"), QStringLiteral("b blaeee"));
    QTest::newRow("multiword-2") << QStringLiteral("toto. a boo") << QStringLiteral("toto. b bla") << map;

    map.clear();
    QTest::newRow("multiword-2 without replace") << QStringLiteral("toto. a boo") << QStringLiteral("toto. a boo") << map;

    map.clear();
    map.insert(QStringLiteral("a boo"), QStringLiteral("b bla"));
    QTest::newRow("multiword-2 with uppercase") << QStringLiteral("toto. A boo") << QStringLiteral("toto. B bla") << map;

    map.clear();
    map.insert(QStringLiteral("a boo1111111111"), QStringLiteral("b bla"));
    QTest::newRow("multiword-3") << QStringLiteral("a boo") << QStringLiteral("a boo") << map;

    map.clear();
    map.insert(QStringLiteral("boo"), QStringLiteral("Bla"));
    QTest::newRow("withuppercase") << QStringLiteral("Boo") << QStringLiteral("Bla") << map;

    map.clear();
    map.insert(QStringLiteral("boo"), QStringLiteral("bla"));
    QTest::newRow("withuppercase-2") << QStringLiteral("Boo") << QStringLiteral("Bla") << map;

    map.clear();
    map.insert(QStringLiteral("boo"), QStringLiteral("Bla"));
    QTest::newRow("withuppercase-3") << QStringLiteral("Boo") << QStringLiteral("Bla") << map;

    map.clear();
    map.insert(QStringLiteral("boo"), QStringLiteral("Bla"));
    QTest::newRow("withuppercase-4") << QStringLiteral("boo") << QStringLiteral("bla") << map;

    map.clear();
    map.insert(QStringLiteral("booooo"), QStringLiteral("bla"));
    QTest::newRow("nofindtext") << QStringLiteral("boo") << QStringLiteral("boo") << map;

    map.clear();
    map.insert(QStringLiteral("boosss"), QStringLiteral("Blasss"));
    map.insert(QStringLiteral("boo"), QStringLiteral("bla"));
    QTest::newRow("withponct") << QStringLiteral("boo!") << QStringLiteral("bla!") << map;

    map.clear();
    map.insert(QStringLiteral("boosss"), QStringLiteral("Blasss"));
    map.insert(QStringLiteral("boo"), QStringLiteral("bla"));
    QTest::newRow("withponct-2") << QStringLiteral("lolo. boo!") << QStringLiteral("lolo. bla!") << map;

    map.clear();
    map.insert(QStringLiteral("boo"), QStringLiteral("bla"));
    QTest::newRow("dontreplaceinpieceofword") << QStringLiteral("voitureboo") << QStringLiteral("voitureboo") << map;

    map.clear();
    map.insert(QStringLiteral("boo"), QStringLiteral("bla"));
    QTest::newRow("dontreplaceall") << QStringLiteral("Boo boo boo") << QStringLiteral("Boo boo bla") << map;

    map.clear();
    map.insert(QStringLiteral("boo"), QStringLiteral("bla"));
    map.insert(QStringLiteral("boo boo"), QStringLiteral("bli"));
    QTest::newRow("replace-1") << QStringLiteral("Boo boo boo") << QStringLiteral("Boo bli") << map;

    map.clear();
    map.insert(QStringLiteral("boo boo"), QStringLiteral("bli"));
    map.insert(QStringLiteral("boo"), QStringLiteral("bla"));
    QTest::newRow("replace-1-order") << QStringLiteral("Boo boo boo") << QStringLiteral("Boo bli") << map;

    map.clear();
    map.insert(QStringLiteral("au delà"), QStringLiteral("au-delà"));
    map.insert(QStringLiteral("boo boo"), QStringLiteral("bli"));
    QTest::newRow("replace-2") << QStringLiteral("voilà au delà") << QStringLiteral("voilà au-delà") << map;

    map.clear();
    map.insert(QStringLiteral("au delà"), QStringLiteral("au-delà"));
    map.insert(QStringLiteral("avant JC"), QStringLiteral("avant J.-C."));
    QTest::newRow("replace-3") << QStringLiteral("il est né avant JC") << QStringLiteral("il est né avant J.-C.") << map;
}

void AutoCorrectionTest::shouldAutocorrectMultiWord()
{
    QFETCH(QString, originalString);
    QFETCH(QString, convertedString);
    QFETCH(mapAutoCorrect, convertStringHash);

    TextAutoCorrection::AutoCorrection autocorrection;
    auto settings = new TextAutoCorrection::AutoCorrectionSettings;
    settings->setEnabledAutoCorrection(true);
    settings->setAdvancedAutocorrect(true);
    settings->setAutocorrectEntries(convertStringHash);
    autocorrection.setAutoCorrectionSettings(settings);

    QTextDocument doc;
    doc.setPlainText(originalString);
    int position = originalString.length();
    autocorrection.autocorrect(false, doc, position);
    QCOMPARE(doc.toPlainText(), convertedString);
}

void AutoCorrectionTest::shouldAddNonBreakingSpace_data()
{
    QTest::addColumn<QString>("originalString");
    QTest::addColumn<QString>("convertedString");
    QTest::addColumn<QString>("language");
    QTest::addColumn<bool>("enableAddNonBreakingSpace");
    QTest::newRow("convert1") << QStringLiteral("boo !") << QStringLiteral("boob!") << QStringLiteral("fr") << true;
    QTest::newRow("disable") << QStringLiteral("boo !") << QStringLiteral("boo !") << QStringLiteral("fr") << false;
    QTest::newRow("nonchanges") << QStringLiteral("boo") << QStringLiteral("boo") << QStringLiteral("fr") << true;
    QTest::newRow("convert2") << QStringLiteral("boo ;") << QStringLiteral("boob;") << QStringLiteral("fr") << true;
    QTest::newRow("convert3") << QStringLiteral("boo ?") << QStringLiteral("boob?") << QStringLiteral("fr") << true;
    QTest::newRow("convert4") << QStringLiteral("boo :") << QStringLiteral("boob:") << QStringLiteral("fr") << true;
    QTest::newRow("nonfrenchlanguage") << QStringLiteral("boo :") << QStringLiteral("boo :") << QStringLiteral("ge") << true;
    QTest::newRow("onecharacter") << QStringLiteral(":") << QStringLiteral(":") << QStringLiteral("fr") << true;
    QTest::newRow("onecharacter2") << QStringLiteral(" ") << QStringLiteral(" ") << QStringLiteral("fr") << true;
    QTest::newRow("percentage") << QStringLiteral("50 %") << QStringLiteral("50b%") << QStringLiteral("fr") << true;
    QTest::newRow("degrees") << QStringLiteral("50 °C") << QStringLiteral("50b°C") << QStringLiteral("fr") << true;
    QTest::newRow("simplespace") << QStringLiteral(" ") << QStringLiteral(" ") << QStringLiteral("fr") << true;
}

void AutoCorrectionTest::shouldAddNonBreakingSpace()
{
    QFETCH(QString, originalString);
    QFETCH(QString, convertedString);
    QFETCH(QString, language);
    QFETCH(bool, enableAddNonBreakingSpace);

    TextAutoCorrection::AutoCorrection autocorrection;
    auto settings = new TextAutoCorrection::AutoCorrectionSettings;
    settings->setEnabledAutoCorrection(true);
    settings->setAddNonBreakingSpace(enableAddNonBreakingSpace);

    settings->setLanguage(language);
    settings->setNonBreakingSpace(QChar(QLatin1Char('b')));
    autocorrection.setAutoCorrectionSettings(settings);

    QTextDocument doc;
    doc.setPlainText(originalString);
    int position = originalString.length();
    autocorrection.autocorrect(false, doc, position);
    QCOMPARE(doc.toPlainText(), convertedString);
}

void AutoCorrectionTest::shouldReplaceWithMultiOption_data()
{
    QTest::addColumn<QString>("originalString");
    QTest::addColumn<QString>("convertedString");
    QTest::addColumn<mapAutoCorrect>("convertStringHash");

    QTest::addColumn<bool>("enable");
    QTest::addColumn<bool>("uppercaseFirstCharOfSentence");
    QTest::addColumn<bool>("advancedAutocorrect");
    QTest::addColumn<bool>("fixtwouppercase");
    QTest::addColumn<int>("position");
    mapAutoCorrect map;
    map.insert(QStringLiteral("boo"), QStringLiteral("bla"));
    map.insert(QStringLiteral(":j2:"), QStringLiteral("TV"));
    map.insert(QStringLiteral("i"), QStringLiteral("I"));
    map.insert(QStringLiteral("Noel"), QStringLiteral("noal"));
    // create a big word otherwise we can't reproduce bug
    map.insert(QStringLiteral("anticonstitutionnellement"), QStringLiteral("2"));

    QTest::newRow("disable") << QStringLiteral("Boo boo boo") << QStringLiteral("Boo boo boo") << map << false << false << false << false << -1;
    QTest::newRow("enablebutdisablealloptions") << QStringLiteral("Boo boo boo") << QStringLiteral("Boo boo boo") << map << true << false << false << false
                                                << -1;
    QTest::newRow("enableandenableuppercase") << QStringLiteral("Boo boo boo") << QStringLiteral("Boo boo boo") << map << true << true << false << false << -1;
    QTest::newRow("enableandenableuppercaseandadvanced")
        << QStringLiteral("Boo boo boo") << QStringLiteral("Boo boo bla") << map << true << true << true << false << -1;

    QTest::newRow("enableandenableuppercaseandadvanced-2")
        << QStringLiteral("Boo boo. boo") << QStringLiteral("Boo boo. Bla") << map << true << true << true << false << -1;
    QTest::newRow("enableandenableuppercaseandadvanced-3")
        << QStringLiteral("blablobli") << QStringLiteral("Blablobli") << map << true << true << true << false << -1;
    QTest::newRow("enableandenableuppercaseandadvanced-4")
        << QStringLiteral("blablobli. foo") << QStringLiteral("blablobli. Foo") << map << true << true << true << false << -1;

    QTest::newRow("enableandenablefixtowuppercase") << QStringLiteral("Boo boo. BOo") << QStringLiteral("Boo boo. Boo") << map << true << true << false << true
                                                    << -1;
    QTest::newRow("enableandenablefixtowuppercase-2") << QStringLiteral("Boo BOo") << QStringLiteral("Boo Boo") << map << true << true << false << true << -1;

    QTest::newRow(":j2:") << QStringLiteral(":j2:") << QStringLiteral("TV") << map << true << false << true << false << -1;
    QTest::newRow(":j2: bla") << QStringLiteral(":j2: bla") << QStringLiteral(":j2: bla") << map << true << false << true << false << -1;
    QTest::newRow(":j2: bla 1") << QStringLiteral(":j2: bla") << QStringLiteral("TV bla") << map << true << false << true << false << 4;
    QTest::newRow(":j2: bla 2") << QStringLiteral(":j2: :j2:") << QStringLiteral(":j2: TV") << map << true << false << true << false << -1;
    QTest::newRow("La mais il n est pas ici ") << QStringLiteral("La mais il n est pas ici ") << QStringLiteral("La mais il n est pas ici ") << map << true
                                               << false << true << false << 25;
    QTest::newRow("a noel") << QStringLiteral("a noel") << QStringLiteral("a noal") << map << true << false << true << false << 6;
    // TODO add more
}

void AutoCorrectionTest::shouldReplaceWithMultiOption()
{
    QFETCH(QString, originalString);
    QFETCH(QString, convertedString);
    QFETCH(mapAutoCorrect, convertStringHash);
    QFETCH(bool, enable);
    QFETCH(bool, uppercaseFirstCharOfSentence);
    QFETCH(bool, advancedAutocorrect);
    QFETCH(bool, fixtwouppercase);
    QFETCH(int, position);

    TextAutoCorrection::AutoCorrection autocorrection;
    auto settings = new TextAutoCorrection::AutoCorrectionSettings;
    settings->setEnabledAutoCorrection(enable);
    settings->setAdvancedAutocorrect(advancedAutocorrect);
    settings->setAutocorrectEntries(convertStringHash);
    settings->setUppercaseFirstCharOfSentence(uppercaseFirstCharOfSentence);
    settings->setFixTwoUppercaseChars(fixtwouppercase);
    autocorrection.setAutoCorrectionSettings(settings);

    QTextDocument doc;
    doc.setPlainText(originalString);
    position = (position == -1) ? originalString.length() : position;
    autocorrection.autocorrect(false, doc, position);
    QCOMPARE(doc.toPlainText(), convertedString);
}

void AutoCorrectionTest::shouldAddNonBreakingSpaceBeforeAfterQuote()
{
    TextAutoCorrection::AutoCorrection autocorrection;
    auto settings = new TextAutoCorrection::AutoCorrectionSettings;
    settings->setEnabledAutoCorrection(true);
    settings->setReplaceDoubleQuotes(true);
    settings->setReplaceSingleQuotes(true);
    settings->setLanguage(QStringLiteral("fr"));
    settings->setAddNonBreakingSpace(true);
    // TODO fix me verify why it doesn't use no breaking space
    const QChar nbsp = QChar(/*QChar::Nbsp*/ QLatin1Char('b'));
    settings->setNonBreakingSpace(nbsp);
    autocorrection.setAutoCorrectionSettings(settings);

    TextAutoCorrection::AutoCorrectionUtils::TypographicQuotes doubleQuote;
    doubleQuote.begin = QLatin1Char('A');
    doubleQuote.end = QLatin1Char('B');
    settings->setTypographicDoubleQuotes(doubleQuote);

    TextAutoCorrection::AutoCorrectionUtils::TypographicQuotes simpleQuote;
    simpleQuote.begin = QLatin1Char('A');
    simpleQuote.end = QLatin1Char('B');

    settings->setTypographicSingleQuotes(simpleQuote);
    autocorrection.setAutoCorrectionSettings(settings);

    QTextDocument doc;
    QString text = QStringLiteral("sss");

    doc.setPlainText(QLatin1Char('"') + text);
    int position = text.length();
    autocorrection.autocorrect(false, doc, position);

    QCOMPARE(doc.toPlainText(), QString(doubleQuote.begin + nbsp + text));

    text = QStringLiteral("sss");
    doc.setPlainText(text + QStringLiteral("\""));
    position = text.length();
    autocorrection.autocorrect(false, doc, position);
    QCOMPARE(doc.toPlainText(), QString(text + nbsp + doubleQuote.end));

    // Simple quote
    text = QStringLiteral("sss");
    doc.setPlainText(text + QStringLiteral("\'"));
    position = text.length();
    autocorrection.autocorrect(false, doc, position);
    QCOMPARE(doc.toPlainText(), QString(text + nbsp + simpleQuote.end));

    text = QStringLiteral("sss");
    doc.setPlainText(QStringLiteral("\"") + text + QStringLiteral("\""));
    position = text.length();
    autocorrection.autocorrect(false, doc, position);
    QCOMPARE(doc.toPlainText(), QString(doubleQuote.begin + nbsp + text + nbsp + doubleQuote.end));

    // Simple quote
    text = QStringLiteral("sss");
    doc.setPlainText(QStringLiteral("\'") + text + QStringLiteral("\'"));
    position = text.length();
    autocorrection.autocorrect(false, doc, position);
    QCOMPARE(doc.toPlainText(), QString(simpleQuote.begin + nbsp + text + nbsp + simpleQuote.end));

    text = QLatin1Char('(');
    doc.setPlainText(QStringLiteral("\"") + text + QStringLiteral("\""));
    position = text.length();
    autocorrection.autocorrect(false, doc, position);
    QCOMPARE(doc.toPlainText(), QString(doubleQuote.begin + nbsp + text + nbsp + doubleQuote.end));
}

void AutoCorrectionTest::shouldAutoFormatURLs()
{
    TextAutoCorrection::AutoCorrection autocorrection;
    auto settings = new TextAutoCorrection::AutoCorrectionSettings;
    settings->setAutoFormatUrl(true);
    autocorrection.setAutoCorrectionSettings(settings);
    // autocorrection.autocorrect(true, doc, position);

    //    QTextCursor cursor(&doc);
    //    cursor.setPosition(2);
    //    QTextCharFormat charFormat = cursor.charFormat();
    //    QCOMPARE(charFormat.font().underline(), false);
    //    QCOMPARE(charFormat.font().bold(), true);
    //    QCOMPARE(charFormat.font().strikeOut(), false);
}
