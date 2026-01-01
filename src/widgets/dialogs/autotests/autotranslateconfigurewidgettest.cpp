/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autotranslateconfigurewidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "dialogs/autotranslateconfigurewidget.h"
#include <QCheckBox>
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QTest>
QTEST_MAIN(AutoTranslateConfigureWidgetTest)

AutoTranslateConfigureWidgetTest::AutoTranslateConfigureWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void AutoTranslateConfigureWidgetTest::shouldHaveDefaultValues()
{
    AutoTranslateConfigureWidget w(nullptr);

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mAutoTranslate = w.findChild<QCheckBox *>(u"mAutoTranslate"_s);
    QVERIFY(mAutoTranslate);
    QVERIFY(!mAutoTranslate->text().isEmpty());
    mainLayout->addWidget(mAutoTranslate);

    auto horizontalLayout = w.findChild<QHBoxLayout *>(u"horizontalLayout"_s);
    QVERIFY(horizontalLayout);
    QCOMPARE(horizontalLayout->contentsMargins(), QMargins{});

    auto label = w.findChild<QLabel *>(u"label"_s);
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    auto mLanguage = w.findChild<QComboBox *>(u"mLanguage"_s);
    QVERIFY(mLanguage);

    QVERIFY(!w.room());
}

#include "moc_autotranslateconfigurewidgettest.cpp"
