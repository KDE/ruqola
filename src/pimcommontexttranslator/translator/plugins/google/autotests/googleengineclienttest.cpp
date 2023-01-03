/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "googleengineclienttest.h"
#include "../googleengineclient.h"
#include <QTest>
QTEST_MAIN(GoogleEngineClientTest)

GoogleEngineClientTest::GoogleEngineClientTest(QObject *parent)
    : QObject{parent}
{
}

void GoogleEngineClientTest::shouldHaveDefaultValues()
{
    GoogleEngineClient client;
    QCOMPARE(client.name(), QStringLiteral("google"));
    QVERIFY(client.createTranslator());
    QVERIFY(!client.translatedName().isEmpty());
    QVERIFY(!client.supportedLanguages().isEmpty());
    QVERIFY(!client.hasConfigurationDialog());
}
