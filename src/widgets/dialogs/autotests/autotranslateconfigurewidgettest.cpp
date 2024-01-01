/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autotranslateconfigurewidgettest.h"
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

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mAutoTranslate = w.findChild<QCheckBox *>(QStringLiteral("mAutoTranslate"));
    QVERIFY(mAutoTranslate);
    QVERIFY(!mAutoTranslate->text().isEmpty());
    mainLayout->addWidget(mAutoTranslate);

    auto horizontalLayout = w.findChild<QHBoxLayout *>(QStringLiteral("horizontalLayout"));
    QVERIFY(horizontalLayout);
    QCOMPARE(horizontalLayout->contentsMargins(), QMargins{});

    auto label = w.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    auto mLanguage = w.findChild<QComboBox *>(QStringLiteral("mLanguage"));
    QVERIFY(mLanguage);

    QVERIFY(!w.room());
}

#include "moc_autotranslateconfigurewidgettest.cpp"
