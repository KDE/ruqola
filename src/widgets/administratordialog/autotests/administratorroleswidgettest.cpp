/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorroleswidgettest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mTreeView = w.findChild<RolesTreeView *>(u"mTreeView"_s);
    QVERIFY(mTreeView);
    QVERIFY(!mTreeView->rootIsDecorated());
    QVERIFY(mTreeView->isSortingEnabled());
    QVERIFY(mTreeView->header()->sectionsClickable());

    auto mSearchLineWidget = w.findChild<QLineEdit *>(u"mSearchLineWidget"_s);
    QVERIFY(mSearchLineWidget);
    QVERIFY(mSearchLineWidget->text().isEmpty());
    QVERIFY(mSearchLineWidget->isClearButtonEnabled());
    QVERIFY(!mSearchLineWidget->placeholderText().isEmpty());

    auto mRoleFilterProxyModel = w.findChild<QSortFilterProxyModel *>(u"mRoleFilterProxyModel"_s);
    QVERIFY(mRoleFilterProxyModel);
}

#include "moc_administratorroleswidgettest.cpp"
