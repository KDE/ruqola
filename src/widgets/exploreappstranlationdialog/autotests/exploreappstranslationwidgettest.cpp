/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exploreappstranslationwidgettest.h"
#include "exploreappstranlationdialog/exploreappstranslationwidget.h"
#include <QTest>
#include <QTreeWidget>
#include <QVBoxLayout>
QTEST_MAIN(ExploreAppsTranslationWidgetTest)

ExploreAppsTranslationWidgetTest::ExploreAppsTranslationWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ExploreAppsTranslationWidgetTest::shouldHaveDefaultValues()
{
    ExploreAppsTranslationWidget w;
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mTreeWidget = w.findChild<QTreeWidget *>(QStringLiteral("mTreeWidget"));
    QVERIFY(mTreeWidget);
    QCOMPARE(mTreeWidget->columnCount(), 2);
}

#include "moc_exploreappstranslationwidgettest.cpp"
