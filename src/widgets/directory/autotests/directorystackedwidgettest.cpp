/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directorystackedwidgettest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mDirectoryWidget = w.findChild<DirectoryWidget *>(u"mDirectoryWidget"_s);
    QVERIFY(mDirectoryWidget);

    auto mDirectoryNotAutorizedWidget = w.findChild<DirectoryNotAuthorizedWidget *>(u"mDirectoryNotAutorizedWidget"_s);
    QVERIFY(mDirectoryNotAutorizedWidget);

    QCOMPARE(w.currentWidget(), mDirectoryWidget);

    QVERIFY(w.isAutorized());

    QVERIFY(w.directoryWidget());

    QVERIFY(w.directoryNotAutorizedWidget());
}

#include "moc_directorystackedwidgettest.cpp"
