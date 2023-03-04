/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directorystackedwidgettest.h"
#include "directory/directorystackedwidget.h"
#include <QTest>
QTEST_MAIN(DirectoryStackedWidgetTest)

DirectoryStackedWidgetTest::DirectoryStackedWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void DirectoryStackedWidgetTest::shouldHaveDefaultValues()
{
    DirectoryStackedWidget w(nullptr, DirectoryWidget::Unknown);
    // TODO
}
