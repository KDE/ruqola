/*
  SPDX-FileCopyrightText: 2014-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "translatorwidgettest.h"
#include "translator/widgets/translatorwidget.h"

#include <QPushButton>

#include <QComboBox>
#include <QSignalSpy>
#include <QStandardPaths>
#include <QTest>
#include <QToolButton>
#include <qtestkeyboard.h>
#include <qtestmouse.h>
QTEST_MAIN(TranslatorWidgetTest)

TranslatorWidgetTest::TranslatorWidgetTest()
{
    QStandardPaths::setTestModeEnabled(true);
}

void TranslatorWidgetTest::shouldHaveDefaultValuesOnCreation()
{
    TextTranslator::TranslatorWidget edit;
    auto from = edit.findChild<QComboBox *>(QStringLiteral("from"));
    auto to = edit.findChild<QComboBox *>(QStringLiteral("to"));

    auto inputtext = edit.findChild<TextTranslator::TranslatorTextEdit *>(QStringLiteral("inputtext"));
    auto translate = edit.findChild<QPushButton *>(QStringLiteral("translate-button"));
    auto clear = edit.findChild<QPushButton *>(QStringLiteral("clear-button"));
    auto invert = edit.findChild<QPushButton *>(QStringLiteral("invert-button"));
    auto configure = edit.findChild<QToolButton *>(QStringLiteral("configure_button"));
    auto close = edit.findChild<QToolButton *>(QStringLiteral("close-button"));
    QVERIFY(configure);
    QVERIFY(close);
    QVERIFY(invert);
    QVERIFY(clear);
    QVERIFY(translate);
    QVERIFY(from);
    QVERIFY(to);
    QCOMPARE(from->count() > 0, true);
    QCOMPARE(to->count() > 0, true);
    QVERIFY(inputtext);
    QCOMPARE(inputtext->toPlainText(), QString());
    QCOMPARE(translate->isEnabled(), false);
}

void TranslatorWidgetTest::shouldEnableTranslateButtonWhenTextToTranslateIsNotEmpty()
{
    TextTranslator::TranslatorWidget edit;

    auto inputtext = edit.findChild<TextTranslator::TranslatorTextEdit *>(QStringLiteral("inputtext"));
    auto translate = edit.findChild<QPushButton *>(QStringLiteral("translate-button"));
    inputtext->setPlainText(QStringLiteral("Foo"));
    QCOMPARE(translate->isEnabled(), true);
}

void TranslatorWidgetTest::shouldDisableTranslateButtonAndClearTextWhenClickOnClearButton()
{
    TextTranslator::TranslatorWidget edit;
    auto inputtext = edit.findChild<TextTranslator::TranslatorTextEdit *>(QStringLiteral("inputtext"));
    auto translate = edit.findChild<QPushButton *>(QStringLiteral("translate-button"));
    inputtext->setPlainText(QStringLiteral("Foo"));
    auto clear = edit.findChild<QPushButton *>(QStringLiteral("clear-button"));
    QTest::mouseClick(clear, Qt::LeftButton);
    QCOMPARE(inputtext->toPlainText(), QString());
    QCOMPARE(translate->isEnabled(), false);
}

void TranslatorWidgetTest::shouldInvertLanguageWhenClickOnInvertButton()
{
    TextTranslator::TranslatorWidget edit;
    auto from = edit.findChild<QComboBox *>(QStringLiteral("from"));
    auto to = edit.findChild<QComboBox *>(QStringLiteral("to"));

    const int fromIndex = 5;
    const int toIndex = 7;
    from->setCurrentIndex(fromIndex);
    to->setCurrentIndex(toIndex);
    auto invert = edit.findChild<QPushButton *>(QStringLiteral("invert-button"));
    QTest::mouseClick(invert, Qt::LeftButton);
    const int newFromIndex = from->currentIndex();
    const int newToIndex = to->currentIndex();
    QCOMPARE(fromIndex != newFromIndex, true);
    QCOMPARE(toIndex != newToIndex, true);
}

void TranslatorWidgetTest::shouldHideWidgetWhenPressEscape()
{
    TextTranslator::TranslatorWidget edit;
    edit.show();
    QVERIFY(QTest::qWaitForWindowExposed(&edit));
    QTest::keyPress(&edit, Qt::Key_Escape);
    QCOMPARE(edit.isVisible(), false);
}

void TranslatorWidgetTest::shouldEmitTranslatorWasClosedSignalWhenCloseIt()
{
    TextTranslator::TranslatorWidget edit;
    edit.show();
    QVERIFY(QTest::qWaitForWindowExposed(&edit));
    QSignalSpy spy(&edit, &TextTranslator::TranslatorWidget::toolsWasClosed);
    QTest::keyClick(&edit, Qt::Key_Escape);
    QCOMPARE(spy.count(), 1);
}
