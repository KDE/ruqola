/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "permissionswidgettest.h"
#include "administratordialog/permissions/permissionstreeview.h"
#include "administratordialog/permissions/permissionswidget.h"
#include <QHeaderView>
#include <QLineEdit>
#include <QSortFilterProxyModel>
#include <QTest>
#include <QTreeView>
#include <QVBoxLayout>
QTEST_MAIN(PermissionsWidgetTest)

PermissionsWidgetTest::PermissionsWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void PermissionsWidgetTest::shouldHaveDefaultValues()
{
    PermissionsWidget w(nullptr);

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mTreeView = w.findChild<PermissionsTreeView *>(QStringLiteral("mTreeView"));
    QVERIFY(mTreeView);
    QVERIFY(!mTreeView->rootIsDecorated());
    QVERIFY(mTreeView->isSortingEnabled());
    QVERIFY(mTreeView->header()->sectionsClickable());

    auto mSearchLineWidget = w.findChild<QLineEdit *>(QStringLiteral("mSearchLineWidget"));
    QVERIFY(mSearchLineWidget);
    QVERIFY(mSearchLineWidget->text().isEmpty());
    QVERIFY(mSearchLineWidget->isClearButtonEnabled());
    QVERIFY(!mSearchLineWidget->placeholderText().isEmpty());

    auto permissionFilterProxyModel = w.findChild<QSortFilterProxyModel *>(QStringLiteral("permissionFilterProxyModel"));
    QVERIFY(permissionFilterProxyModel);
}
