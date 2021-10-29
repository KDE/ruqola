/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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
    QCOMPARE(mainLayout->contentsMargins(), {});

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
