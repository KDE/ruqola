/*
  SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "emoticoncategorybuttontest.h"
#include "emoticoncategorybutton.h"
#include <QTest>
QTEST_MAIN(EmoticonCategoryButtonTest)
EmoticonCategoryButtonTest::EmoticonCategoryButtonTest(QObject *parent)
    : QObject{parent}
{
}

void EmoticonCategoryButtonTest::shouldHaveDefaultValues()
{
    TextEmoticonsWidgets::EmoticonCategoryButton w;
    QVERIFY(w.isCheckable());
    QVERIFY(w.autoRaise());
}
