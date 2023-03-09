/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "explorepermissionswidgettest.h"
#include "explorepermissionsdialog/explorepermissionswidget.h"

#include <QLineEdit>
#include <QSortFilterProxyModel>
#include <QStandardPaths>
#include <QTest>
#include <QTreeView>
#include <QVBoxLayout>

QTEST_MAIN(ExplorePermissionsWidgetTest)
ExplorePermissionsWidgetTest::ExplorePermissionsWidgetTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void ExplorePermissionsWidgetTest::shouldHaveDefaultValues()
{
    ExplorePermissionsWidget w;
    auto mTreeView = w.findChild<QTreeView *>(QStringLiteral("mTreeView"));
    QVERIFY(mTreeView);
    QVERIFY(!mTreeView->rootIsDecorated());
    QVERIFY(!mTreeView->isSortingEnabled());

    auto mSearchLineWidget = w.findChild<QLineEdit *>(QStringLiteral("mSearchLineWidget"));
    QVERIFY(mSearchLineWidget);
    QVERIFY(mSearchLineWidget->text().isEmpty());

    auto mPermissionFilterProxyModel = w.findChild<QSortFilterProxyModel *>(QStringLiteral("permissionFilterProxyModel"));
    QVERIFY(mPermissionFilterProxyModel);

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});
}
