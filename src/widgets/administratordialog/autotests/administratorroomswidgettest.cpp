/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorroomswidgettest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mSearchLineEdit = w.findChild<QLineEdit *>(u"mSearchLineEdit"_s);
    QVERIFY(mSearchLineEdit);
    QVERIFY(mSearchLineEdit->isClearButtonEnabled());

    auto mTreeView = w.findChild<QTreeView *>(u"mTreeView"_s);
    QVERIFY(mTreeView);
    QVERIFY(mTreeView->isSortingEnabled());
    QCOMPARE(mTreeView->selectionMode(), QAbstractItemView::SelectionMode::SingleSelection);
    QVERIFY(!mTreeView->rootIsDecorated());

    auto mSelectRoomType = w.findChild<AdministratorRoomsSelectRoomTypeWidget *>(u"mSelectRoomType"_s);
    QVERIFY(mSelectRoomType);
}

#include "moc_administratorroomswidgettest.cpp"
