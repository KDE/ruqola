/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "lingvaengineclienttest.h"
#include "../lingvaengineclient.h"
#include <QTest>
QTEST_MAIN(LingvaEngineClientTest)

LingvaEngineClientTest::LingvaEngineClientTest(QObject *parent)
    : QObject{parent}
{
}

void LingvaEngineClientTest::shouldHaveDefaultValues()
{
    LingvaEngineClient client;
    QCOMPARE(client.name(), QStringLiteral("lingva"));
    QVERIFY(client.createTranslator());
    QVERIFY(!client.translatedName().isEmpty());
    QVERIFY(!client.supportedLanguages().isEmpty());
    QVERIFY(client.hasConfigurationDialog());
}
