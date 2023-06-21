/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorserverinfowidgettest.h"
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
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mTreeWidget = w.findChild<QTreeWidget *>(QStringLiteral("mTreeWidget"));
    QVERIFY(mTreeWidget);
    QCOMPARE(mTreeWidget->columnCount(), 2);
    QCOMPARE(mTreeWidget->contextMenuPolicy(), Qt::CustomContextMenu);
    QVERIFY(mTreeWidget->header()->isHidden());

    auto mSearchLineWidget = w.findChild<KTreeWidgetSearchLineWidget *>(QStringLiteral("mSearchLineWidget"));
    QVERIFY(mSearchLineWidget);

    auto hboxLayout = w.findChild<QHBoxLayout *>(QStringLiteral("hboxLayout"));
    QVERIFY(hboxLayout);
    QCOMPARE(hboxLayout->contentsMargins(), QMargins{});

    auto mRefreshButton = w.findChild<QPushButton *>(QStringLiteral("mRefreshButton"));
    QVERIFY(mRefreshButton);
    QVERIFY(mRefreshButton->text().isEmpty());
    QVERIFY(!mRefreshButton->toolTip().isEmpty());
}

#include "moc_administratorserverinfowidgettest.cpp"
