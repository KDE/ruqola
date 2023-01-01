/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorroomswidgettest.h"
#include "administratordialog/rooms/administratorroomsselectroomtypewidget.h"
#include "administratordialog/rooms/administratorroomswidget.h"
#include <QLineEdit>
#include <QTest>
#include <QTreeView>
#include <QVBoxLayout>
QTEST_MAIN(AdministratorRoomsWidgetTest)
AdministratorRoomsWidgetTest::AdministratorRoomsWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void AdministratorRoomsWidgetTest::shouldHaveDefaultValues()
{
    AdministratorRoomsWidget w(nullptr);
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mSearchLineEdit = w.findChild<QLineEdit *>(QStringLiteral("mSearchLineEdit"));
    QVERIFY(mSearchLineEdit);
    QVERIFY(mSearchLineEdit->isClearButtonEnabled());

    auto mTreeView = w.findChild<QTreeView *>(QStringLiteral("mTreeView"));
    QVERIFY(mTreeView);
    QVERIFY(mTreeView->isSortingEnabled());
    QCOMPARE(mTreeView->selectionMode(), QAbstractItemView::SelectionMode::SingleSelection);
    QVERIFY(!mTreeView->rootIsDecorated());

    auto mSelectRoomType = w.findChild<AdministratorRoomsSelectRoomTypeWidget *>(QStringLiteral("mSelectRoomType"));
    QVERIFY(mSelectRoomType);
}
