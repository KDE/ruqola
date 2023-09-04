/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "whatsnewwidgettest.h"
#include "whatsnew/whatsnewcomboboxwidget.h"
#include "whatsnew/whatsnewwidget.h"
#include <KSeparator>
#include <QLabel>
#include <QScrollArea>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(WhatsNewWidgetTest)
WhatsNewWidgetTest::WhatsNewWidgetTest(QWidget *parent)
    : QWidget{parent}
{
}

void WhatsNewWidgetTest::shouldHaveDefaultValues()
{
    WhatsNewWidget w;
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mLabelInfo = w.findChild<QLabel *>(QStringLiteral("mLabelInfo"));
    QVERIFY(mLabelInfo);
    QVERIFY(mLabelInfo->text().isEmpty());

    auto mWhatsNewComboBoxWidget = w.findChild<WhatsNewComboBoxWidget *>(QStringLiteral("mWhatsNewComboBoxWidget"));
    QVERIFY(mWhatsNewComboBoxWidget);

    auto separator = w.findChild<KSeparator *>(QStringLiteral("separator"));
    QVERIFY(separator);

    auto scrollArea = w.findChild<QScrollArea *>(QStringLiteral("scrollArea"));
    QVERIFY(scrollArea);
    QCOMPARE(scrollArea->widgetResizable(), true);
}

#include "moc_whatsnewwidgettest.cpp"
