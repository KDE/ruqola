/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#include "directorytabwidgettest.h"
#include "directory/directorytabwidget.h"
#include "directory/directorywidget.h"
#include <QTest>
QTEST_MAIN(DirectoryTabWidgetTest)
DirectoryTabWidgetTest::DirectoryTabWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void DirectoryTabWidgetTest::shouldHaveDefaultValues()
{
    DirectoryTabWidget w(nullptr);
    QCOMPARE(w.count(), 3);

    DirectoryWidget *tab = qobject_cast<DirectoryWidget *>(w.widget(0));
    QCOMPARE(tab->type(), DirectoryWidget::Room);
    QVERIFY(tab);

    tab = qobject_cast<DirectoryWidget *>(w.widget(1));
    QCOMPARE(tab->type(), DirectoryWidget::User);
    QVERIFY(tab);

    tab = qobject_cast<DirectoryWidget *>(w.widget(2));
    QCOMPARE(tab->type(), DirectoryWidget::Team);
    QVERIFY(tab);
}
