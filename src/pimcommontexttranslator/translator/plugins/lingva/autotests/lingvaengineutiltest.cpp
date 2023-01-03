/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "lingvaengineutiltest.h"
#include "../lingvaengineutil.h"
#include <QTest>
QTEST_MAIN(LingvaEngineUtilTest)
LingvaEngineUtilTest::LingvaEngineUtilTest(QObject *parent)
    : QObject{parent}
{
}

void LingvaEngineUtilTest::shouldHaveDefaultValues()
{
    QCOMPARE(LingvaEngineUtil::groupName(), QStringLiteral("LingvaTranslator"));
    QCOMPARE(LingvaEngineUtil::serverUrlKey(), QStringLiteral("ServerUrl"));
}
