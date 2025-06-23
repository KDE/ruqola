/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exploreappstranslationwidgettest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mTreeWidget = w.findChild<QTreeWidget *>(u"mTreeWidget"_s);
    QVERIFY(mTreeWidget);
    QCOMPARE(mTreeWidget->columnCount(), 2);
}

#include "moc_exploreappstranslationwidgettest.cpp"
