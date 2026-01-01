/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorserverinfowidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "administratordialog/serverinfo/administratorserverinfowidget.h"
#include <KTreeWidgetSearchLineWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QTest>
#include <QTreeWidget>
#include <QVBoxLayout>
QTEST_MAIN(AdministratorServerInfoWidgetTest)
AdministratorServerInfoWidgetTest::AdministratorServerInfoWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void AdministratorServerInfoWidgetTest::shouldHaveDefaultValues()
{
    AdministratorServerInfoWidget w(nullptr);
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mTreeWidget = w.findChild<QTreeWidget *>(u"mTreeWidget"_s);
    QVERIFY(mTreeWidget);
    QCOMPARE(mTreeWidget->columnCount(), 2);
    QCOMPARE(mTreeWidget->contextMenuPolicy(), Qt::CustomContextMenu);
    QVERIFY(mTreeWidget->header()->isHidden());

    auto mSearchLineWidget = w.findChild<KTreeWidgetSearchLineWidget *>(u"mSearchLineWidget"_s);
    QVERIFY(mSearchLineWidget);

    auto hboxLayout = w.findChild<QHBoxLayout *>(u"hboxLayout"_s);
    QVERIFY(hboxLayout);
    QCOMPARE(hboxLayout->contentsMargins(), QMargins{});

    auto mRefreshButton = w.findChild<QPushButton *>(u"mRefreshButton"_s);
    QVERIFY(mRefreshButton);
    QVERIFY(mRefreshButton->text().isEmpty());
    QVERIFY(!mRefreshButton->toolTip().isEmpty());
}

#include "moc_administratorserverinfowidgettest.cpp"
