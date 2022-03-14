/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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

    auto *tab = qobject_cast<DirectoryWidget *>(w.widget(0));
    QCOMPARE(tab->objectName(), QStringLiteral("mRooms"));
    QCOMPARE(tab->type(), DirectoryWidget::Room);
    QVERIFY(tab);

    tab = qobject_cast<DirectoryWidget *>(w.widget(1));
    QCOMPARE(tab->objectName(), QStringLiteral("mUsers"));
    QCOMPARE(tab->type(), DirectoryWidget::User);
    QVERIFY(tab);

    tab = qobject_cast<DirectoryWidget *>(w.widget(2));
    QCOMPARE(tab->objectName(), QStringLiteral("mTeams"));
    QCOMPARE(tab->type(), DirectoryWidget::Team);
    QVERIFY(tab);
}
