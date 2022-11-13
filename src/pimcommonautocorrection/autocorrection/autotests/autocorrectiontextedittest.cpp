/*
  SPDX-FileCopyrightText: 2014-2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "autocorrectiontextedittest.h"
#include <PimCommonAutoCorrection/AutoCorrectionTextEdit>
#include <QTest>
#include <autocorrection/autocorrection.h>
#include <autocorrection/autocorrectionsettings.h>
#include <qtestkeyboard.h>

AutoCorrectionTextEditTest::AutoCorrectionTextEditTest() = default;

void AutoCorrectionTextEditTest::shouldNotAutocorrectWhenDisabled()
{
    PimCommonAutoCorrection::AutoCorrectionTextEdit richtext(nullptr);
    QHash<QString, QString> entries;
    const QString originalWord = QStringLiteral("FOOFOO");
    const QString replaceWord = QStringLiteral("BLABLA");
    entries.insert(originalWord, replaceWord);
    auto settings = new PimCommonAutoCorrection::AutoCorrectionSettings;
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
    PimCommonAutoCorrection::AutoCorrectionTextEdit richtext(nullptr);
    const QString originalWord = QStringLiteral("FOOFOO");
    const QString replaceWord = QStringLiteral("BLABLA");
    QHash<QString, QString> entries;
    entries.insert(originalWord, replaceWord);
    auto settings = new PimCommonAutoCorrection::AutoCorrectionSettings;
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
    PimCommonAutoCorrection::AutoCorrectionTextEdit richtext(nullptr);
    const QString originalWord = QStringLiteral("FOOFOO");
    const QString replaceWord = QStringLiteral("BLABLA");
    QHash<QString, QString> entries;
    entries.insert(originalWord, replaceWord);
    auto settings = new PimCommonAutoCorrection::AutoCorrectionSettings;
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
    PimCommonAutoCorrection::AutoCorrectionTextEdit richtext(nullptr);
    const QString originalWord = QStringLiteral("FOOFOO");
    const QString replaceWord = QStringLiteral("BLABLA");
    QHash<QString, QString> entries;
    entries.insert(originalWord, replaceWord);

    auto settings = new PimCommonAutoCorrection::AutoCorrectionSettings;
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
    PimCommonAutoCorrection::AutoCorrectionTextEdit richtext(nullptr);
    const QString originalWord = QStringLiteral("FOOFOO");
    const QString replaceWord = QStringLiteral("BLABLA");
    QHash<QString, QString> entries;
    entries.insert(originalWord, replaceWord);
    auto settings = new PimCommonAutoCorrection::AutoCorrectionSettings;
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
