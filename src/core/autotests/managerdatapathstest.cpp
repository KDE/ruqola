/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "managerdatapathstest.h"
#include "managerdatapaths.h"
#include <QTest>

QTEST_GUILESS_MAIN(ManagerDataPathsTest)

ManagerDataPathsTest::ManagerDataPathsTest(QObject *parent)
    : QObject(parent)
{
}

void ManagerDataPathsTest::shouldHaveDefaultValue()
{
    ManagerDataPaths p;
    // TODO
}

#include "moc_managerdatapathstest.cpp"
