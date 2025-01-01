/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "e2ekeymanagertest.h"
#include "encryption/e2ekeymanager.h"
#include <QTest>
QTEST_GUILESS_MAIN(E2eKeyManagerTest)
E2eKeyManagerTest::E2eKeyManagerTest(QObject *parent)
    : QObject{parent}
{
}

void E2eKeyManagerTest::shouldHaveDefaultValues()
{
    E2eKeyManager m(nullptr);
    QCOMPARE(m.status(), E2eKeyManager::Status::Unknown);
    QVERIFY(!m.keySaved());
}

#include "moc_e2ekeymanagertest.cpp"
