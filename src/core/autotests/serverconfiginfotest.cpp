/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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
    QVERIFY(info.serverName().isEmpty());
    QVERIFY(info.userName().isEmpty());
    QVERIFY(info.serverVersionStr().isEmpty());
    QVERIFY(info.serverUrl().isEmpty());
    QVERIFY(info.logoUrl().isEmpty());
    QVERIFY(info.faviconUrl().isEmpty());
}
