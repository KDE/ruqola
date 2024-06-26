/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directorystackedwidgettest.h"
#include "directory/directorynotauthorizedwidget.h"
#include "directory/directorystackedwidget.h"
#include "directory/directorywidget.h"
#include <QTest>
QTEST_MAIN(DirectoryStackedWidgetTest)

DirectoryStackedWidgetTest::DirectoryStackedWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void DirectoryStackedWidgetTest::shouldHaveDefaultValues()
{
    DirectoryStackedWidget w(nullptr, DirectoryWidget::DirectoryType::Unknown);

    auto mDirectoryWidget = w.findChild<DirectoryWidget *>(QStringLiteral("mDirectoryWidget"));
    QVERIFY(mDirectoryWidget);

    auto mDirectoryNotAutorizedWidget = w.findChild<DirectoryNotAuthorizedWidget *>(QStringLiteral("mDirectoryNotAutorizedWidget"));
    QVERIFY(mDirectoryNotAutorizedWidget);

    QCOMPARE(w.currentWidget(), mDirectoryWidget);

    QVERIFY(w.isAutorized());

    QVERIFY(w.directoryWidget());

    QVERIFY(w.directoryNotAutorizedWidget());
}

#include "moc_directorystackedwidgettest.cpp"
