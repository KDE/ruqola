/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "permissionswidgettest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mTreeView = w.findChild<PermissionsTreeView *>(u"mTreeView"_s);
    QVERIFY(mTreeView);
    QVERIFY(!mTreeView->rootIsDecorated());
    QVERIFY(mTreeView->isSortingEnabled());
    QVERIFY(mTreeView->header()->sectionsClickable());

    auto mSearchLineWidget = w.findChild<QLineEdit *>(u"mSearchLineWidget"_s);
    QVERIFY(mSearchLineWidget);
    QVERIFY(mSearchLineWidget->text().isEmpty());
    QVERIFY(mSearchLineWidget->isClearButtonEnabled());
    QVERIFY(!mSearchLineWidget->placeholderText().isEmpty());

    auto permissionFilterProxyModel = w.findChild<QSortFilterProxyModel *>(u"permissionFilterProxyModel"_s);
    QVERIFY(permissionFilterProxyModel);
}

#include "moc_permissionswidgettest.cpp"
