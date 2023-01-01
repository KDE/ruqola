/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "serverconfiginfotest.h"
#include "serverconfiginfo.h"

#include <QTest>
QTEST_GUILESS_MAIN(ServerConfigInfoTest)

ServerConfigInfoTest::ServerConfigInfoTest(QObject *parent)
    : QObject(parent)
{
}

void ServerConfigInfoTest::shouldHaveDefaultValues()
{
    ServerConfigInfo info;
    QVERIFY(info.accountName().isEmpty());
    QVERIFY(info.userName().isEmpty());
    QVERIFY(info.serverVersionStr().isEmpty());
    QVERIFY(info.serverUrl().isEmpty());
    QVERIFY(info.logoUrl().isEmpty());
    QVERIFY(info.faviconUrl().isEmpty());
}
