/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directorynotauthorizedwidgettest.h"
#include "directory/directorynotauthorizedwidget.h"
#include <QTest>
QTEST_MAIN(DirectoryNotAuthorizedWidgetTest)
DirectoryNotAuthorizedWidgetTest::DirectoryNotAuthorizedWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void DirectoryNotAuthorizedWidgetTest::shouldHaveDefaultValues()
{
    DirectoryNotAuthorizedWidget w(nullptr);
    // TODO
}
