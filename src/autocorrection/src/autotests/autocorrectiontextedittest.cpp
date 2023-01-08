/*
  SPDX-FileCopyrightText: 2014-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "autocorrectiontextedittest.h"
#include "autocorrection.h"
#include "autocorrectionsettings.h"
#include <QTest>
#include <TextAutoCorrection/AutoCorrectionTextEdit>
#include <qtestkeyboard.h>

AutoCorrectionTextEditTest::AutoCorrectionTextEditTest() = default;

void AutoCorrectionTextEditTest::shouldNotAutocorrectWhenDisabled()
{
    TextAutoCorrection::AutoCorrectionTextEdit richtext(nullptr);
    QHash<QString, QString> entries;
    const QString originalWord = QStringLiteral("FOOFOO");
    const QString replaceWord = QStringLiteral("BLABLA");
    entries.insert(originalWord, replaceWord);
    auto settings = new TextAutoCorrection::AutoCorrectionSettings;
    settings->setAutocorrectEntries(entries);
    richtext.autocorrection()->setAutoCorrectionSettings(settings);
    richtext.show();
    QVERIFY(QTest::qWaitForWindowExposed(&richtext));
    QTest::keyClicks(&richtext, originalWord);
    QTest::keyClick(&richtext, ' ');
    QCOMPARE(richtext.toPlainText(), QString(originalWord + QLatin1Char(' ')));
}

void AutoCorrectionTextEditTest::shouldReplaceWordWhenExactText()
{
    TextAutoCorrection::AutoCorrectionTextEdit richtext(nullptr);
    const QString originalWord = QStringLiteral("FOOFOO");
    const QString replaceWord = QStringLiteral("BLABLA");
    QHash<QString, QString> entries;
    entries.insert(originalWord, replaceWord);
    auto settings = new TextAutoCorrection::AutoCorrectionSettings;
    settings->setAutocorrectEntries(entries);
    settings->setEnabledAutoCorrection(true);
    settings->setAdvancedAutocorrect(true);
    richtext.autocorrection()->setAutoCorrectionSettings(settings);
    richtext.show();
    QVERIFY(QTest::qWaitForWindowExposed(&richtext));
    QTest::keyClicks(&richtext, originalWord);
    QTest::keyClick(&richtext, ' ');
    QCOMPARE(richtext.toPlainText(), QString(replaceWord + QLatin1Char(' ')));
}

void AutoCorrectionTextEditTest::shouldNotReplaceWordWhenInexactText()
{
    TextAutoCorrection::AutoCorrectionTextEdit richtext(nullptr);
    const QString originalWord = QStringLiteral("FOOFOO");
    const QString replaceWord = QStringLiteral("BLABLA");
    QHash<QString, QString> entries;
    entries.insert(originalWord, replaceWord);
    auto settings = new TextAutoCorrection::AutoCorrectionSettings;
    settings->setAutocorrectEntries(entries);
    settings->setEnabledAutoCorrection(true);
    settings->setAdvancedAutocorrect(true);
    richtext.autocorrection()->setAutoCorrectionSettings(settings);

    richtext.show();
    const QString nonExactText = QStringLiteral("BLIBLI");
    QVERIFY(QTest::qWaitForWindowExposed(&richtext));
    QTest::keyClicks(&richtext, nonExactText);
    QTest::keyClick(&richtext, ' ');
    QCOMPARE(richtext.toPlainText(), QString(nonExactText + QLatin1Char(' ')));
}

void AutoCorrectionTextEditTest::shouldReplaceWhenPressEnter()
{
    TextAutoCorrection::AutoCorrectionTextEdit richtext(nullptr);
    const QString originalWord = QStringLiteral("FOOFOO");
    const QString replaceWord = QStringLiteral("BLABLA");
    QHash<QString, QString> entries;
    entries.insert(originalWord, replaceWord);

    auto settings = new TextAutoCorrection::AutoCorrectionSettings;
    settings->setAutocorrectEntries(entries);
    settings->setEnabledAutoCorrection(true);
    settings->setAdvancedAutocorrect(true);
    richtext.autocorrection()->setAutoCorrectionSettings(settings);

    richtext.show();
    QVERIFY(QTest::qWaitForWindowExposed(&richtext));
    QTest::keyClicks(&richtext, originalWord);
    QTest::keyPress(&richtext, Qt::Key_Enter);
    QCOMPARE(richtext.toPlainText(), QString(replaceWord + QLatin1Char('\n')));
}

void AutoCorrectionTextEditTest::shouldReplaceWhenPressReturn()
{
    TextAutoCorrection::AutoCorrectionTextEdit richtext(nullptr);
    const QString originalWord = QStringLiteral("FOOFOO");
    const QString replaceWord = QStringLiteral("BLABLA");
    QHash<QString, QString> entries;
    entries.insert(originalWord, replaceWord);
    auto settings = new TextAutoCorrection::AutoCorrectionSettings;
    settings->setAutocorrectEntries(entries);
    settings->setEnabledAutoCorrection(true);
    settings->setAdvancedAutocorrect(true);
    richtext.autocorrection()->setAutoCorrectionSettings(settings);
    richtext.show();
    QVERIFY(QTest::qWaitForWindowExposed(&richtext));
    QTest::keyClicks(&richtext, originalWord);
    QTest::keyPress(&richtext, Qt::Key_Return);
    QCOMPARE(richtext.toPlainText(), QString(replaceWord + QLatin1Char('\n')));
}

QTEST_MAIN(AutoCorrectionTextEditTest)
