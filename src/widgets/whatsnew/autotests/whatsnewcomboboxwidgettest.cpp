/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "whatsnewcomboboxwidgettest.h"
#include "whatsnew/whatsnewcomboboxwidget.h"
#include <QComboBox>
#include <QLabel>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(WhatsNewComboBoxWidgetTest)

WhatsNewComboBoxWidgetTest::WhatsNewComboBoxWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void WhatsNewComboBoxWidgetTest::shouldHaveDefaultValues()
{
    WhatsNewComboBoxWidget w;
    auto mainLayout = w.findChild<QHBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mVersionComboBox = w.findChild<QComboBox *>(QStringLiteral("mVersionComboBox"));
    QVERIFY(mVersionComboBox);

    auto label = w.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());
}
