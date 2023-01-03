/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "translatorengineloadertest.h"
#include "translator/translatorengineloader.h"
#include <QTest>
QTEST_MAIN(TranslatorEngineLoaderTest)
TranslatorEngineLoaderTest::TranslatorEngineLoaderTest(QObject *parent)
    : QObject{parent}
{
}

void TranslatorEngineLoaderTest::shouldHaveDefaultValues()
{
    PimCommonTextTranslator::TranslatorEngineLoader w;
    QVERIFY(!w.translatorEngineInfos().isEmpty());
}
