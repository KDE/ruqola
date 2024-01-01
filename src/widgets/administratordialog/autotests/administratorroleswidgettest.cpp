/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorroleswidgettest.h"
#include "administratordialog/roles/administratorroleswidget.h"
#include "administratordialog/roles/rolestreeview.h"
#include <QHeaderView>
#include <QLineEdit>
#include <QSortFilterProxyModel>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(AdministratorRolesWidgetTest)
AdministratorRolesWidgetTest::AdministratorRolesWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void AdministratorRolesWidgetTest::shouldHaveDefaultValues()
{
    AdministratorRolesWidget w(nullptr);

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mTreeView = w.findChild<RolesTreeView *>(QStringLiteral("mTreeView"));
    QVERIFY(mTreeView);
    QVERIFY(!mTreeView->rootIsDecorated());
    QVERIFY(mTreeView->isSortingEnabled());
    QVERIFY(mTreeView->header()->sectionsClickable());

    auto mSearchLineWidget = w.findChild<QLineEdit *>(QStringLiteral("mSearchLineWidget"));
    QVERIFY(mSearchLineWidget);
    QVERIFY(mSearchLineWidget->text().isEmpty());
    QVERIFY(mSearchLineWidget->isClearButtonEnabled());
    QVERIFY(!mSearchLineWidget->placeholderText().isEmpty());

    auto mRoleFilterProxyModel = w.findChild<QSortFilterProxyModel *>(QStringLiteral("mRoleFilterProxyModel"));
    QVERIFY(mRoleFilterProxyModel);
}

#include "moc_administratorroleswidgettest.cpp"
