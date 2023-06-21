/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "otrmanagertest.h"
#include "otr/otrmanager.h"
#include <QTest>

QTEST_GUILESS_MAIN(OtrManagerTest)

OtrManagerTest::OtrManagerTest(QObject *parent)
    : QObject(parent)
{
}

void OtrManagerTest::shouldHaveDefaultValues()
{
    OtrManager w(nullptr);
    // TODO
}

#include "moc_otrmanagertest.cpp"
