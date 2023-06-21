/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "restauthenticationmanagertest.h"
#include "restauthenticationmanager.h"
#include <QTest>
QTEST_GUILESS_MAIN(RESTAuthenticationManagerTest)
RESTAuthenticationManagerTest::RESTAuthenticationManagerTest(QObject *parent)
    : QObject(parent)
{
}

#include "moc_restauthenticationmanagertest.cpp"
