/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "deeplengineclienttest.h"
#include "../deeplengineclient.h"
#include <QTest>
QTEST_MAIN(DeeplEngineClientTest)

DeeplEngineClientTest::DeeplEngineClientTest(QObject *parent)
    : QObject{parent}
{
}

void DeeplEngineClientTest::shouldHaveDefaultValues()
{
    DeeplEngineClient client;
    QCOMPARE(client.name(), QStringLiteral("deepl"));
    QVERIFY(client.createTranslator());
    QVERIFY(!client.translatedName().isEmpty());
    QVERIFY(!client.supportedLanguages().isEmpty());
    QVERIFY(client.hasConfigurationDialog());
}
