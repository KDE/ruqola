/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "libretranslateengineutiltest.h"
#include "../libretranslateengineutil.h"
#include <QTest>
QTEST_MAIN(LibreTranslateEngineUtilTest)
LibreTranslateEngineUtilTest::LibreTranslateEngineUtilTest(QObject *parent)
    : QObject{parent}
{
}

void LibreTranslateEngineUtilTest::shouldHaveDefaultValues()
{
    QCOMPARE(LibreTranslateEngineUtil::groupName(), QStringLiteral("LibreTranslateTranslator"));
    QCOMPARE(LibreTranslateEngineUtil::serverUrlKey(), QStringLiteral("ServerUrl"));
}
