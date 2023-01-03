/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "yandexengineclienttest.h"
#include "../yandexengineclient.h"
#include <QTest>
QTEST_MAIN(YandexEngineClientTest)

YandexEngineClientTest::YandexEngineClientTest(QObject *parent)
    : QObject{parent}
{
}

void YandexEngineClientTest::shouldHaveDefaultValues()
{
    YandexEngineClient client;
    QCOMPARE(client.name(), QStringLiteral("yandex"));
    QVERIFY(client.createTranslator());
    QVERIFY(!client.translatedName().isEmpty());
    QVERIFY(!client.supportedLanguages().isEmpty());
    QVERIFY(!client.hasConfigurationDialog());
}
