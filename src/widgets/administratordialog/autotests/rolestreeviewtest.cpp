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

#include "rolestreeviewtest.h"
#include "administratordialog/roles/rolestreeview.h"
#include <QHeaderView>
#include <QTest>
QTEST_MAIN(RolesTreeViewTest)
RolesTreeViewTest::RolesTreeViewTest(QObject *parent)
    : QObject{parent}
{
}

void RolesTreeViewTest::shouldHaveDefaultValues()
{
    RolesTreeView w;
    QVERIFY(!w.rootIsDecorated());
    QVERIFY(w.isSortingEnabled());
    QCOMPARE(w.contextMenuPolicy(), Qt::CustomContextMenu);
    QVERIFY(w.header()->sectionsClickable());
}
