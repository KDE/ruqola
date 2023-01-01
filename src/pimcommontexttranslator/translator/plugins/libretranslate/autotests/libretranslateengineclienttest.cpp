/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "libretranslateengineclienttest.h"
#include "../libretranslateengineclient.h"
#include <QTest>
QTEST_MAIN(LibreTranslateEngineClientTest)

LibreTranslateEngineClientTest::LibreTranslateEngineClientTest(QObject *parent)
    : QObject{parent}
{
}

void LibreTranslateEngineClientTest::shouldHaveDefaultValues()
{
    LibreTranslateEngineClient client;
    QCOMPARE(client.name(), QStringLiteral("libretranslate"));
    QVERIFY(client.createTranslator());
    QVERIFY(!client.translatedName().isEmpty());
    QVERIFY(!client.supportedLanguages().isEmpty());
    QVERIFY(client.hasConfigurationDialog());
}
