/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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
    AdministratorServerInfoWidget w;
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
    QCOMPARE(hboxLayout->contentsMargins(), {});

    auto mRefreshButton = w.findChild<QPushButton *>(QStringLiteral("mRefreshButton"));
    QVERIFY(mRefreshButton);
    QVERIFY(!mRefreshButton->text().isEmpty());
}
