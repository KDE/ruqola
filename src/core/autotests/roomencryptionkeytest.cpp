/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomencryptionkeytest.h"
#include "roomencryptionkey.h"
#include <QTest>
QTEST_GUILESS_MAIN(RoomEncryptionKeyTest)

RoomEncryptionKeyTest::RoomEncryptionKeyTest(QObject *parent)
    : QObject{parent}
{
}

void RoomEncryptionKeyTest::shouldHaveDefaultValues()
{
    const RoomEncryptionKey w;
    QVERIFY(w.e2EKey().isEmpty());
    QVERIFY(w.e2eKeyId().isEmpty());
    QVERIFY(w.e2ESuggestedKey().isEmpty());
    QVERIFY(w.usersWaitingForE2EKeys().isEmpty());
    QVERIFY(w.sessionKey().isEmpty());
}
#include "moc_roomencryptionkeytest.cpp"
