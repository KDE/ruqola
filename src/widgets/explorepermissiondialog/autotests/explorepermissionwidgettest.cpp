/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "explorepermissionwidgettest.h"
#include "explorepermissiondialog/explorepermissionwidget.h"

#include <QLineEdit>
#include <QSortFilterProxyModel>
#include <QStandardPaths>
#include <QTest>
#include <QTreeView>

QTEST_MAIN(ExplorePermissionWidgetTest)
ExplorePermissionWidgetTest::ExplorePermissionWidgetTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void ExplorePermissionWidgetTest::shouldHaveDefaultValues()
{
    ExplorePermissionWidget w;
    auto mTreeView = w.findChild<QTreeView *>(QStringLiteral("mTreeView"));
    QVERIFY(mTreeView);
    QVERIFY(!mTreeView->rootIsDecorated());
    QVERIFY(!mTreeView->isSortingEnabled());

    auto mSearchLineWidget = w.findChild<QLineEdit *>(QStringLiteral("mSearchLineWidget"));
    QVERIFY(mSearchLineWidget);
    QVERIFY(mSearchLineWidget->text().isEmpty());

    auto mPermissionFilterProxyModel = w.findChild<QSortFilterProxyModel *>(QStringLiteral("permissionFilterProxyModel"));
    QVERIFY(mPermissionFilterProxyModel);
}
