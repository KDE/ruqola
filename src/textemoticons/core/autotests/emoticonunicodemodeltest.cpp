/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticonunicodemodeltest.h"
#include "emoticonunicodemodel.h"
#include <QTest>
QTEST_GUILESS_MAIN(EmoticonUnicodeModelTest)

EmoticonUnicodeModelTest::EmoticonUnicodeModelTest(QObject *parent)
    : QObject{parent}
{
}

void EmoticonUnicodeModelTest::shouldHaveDefaultValues()
{
    TextEmoticonsCore::EmoticonUnicodeModel w;
    QVERIFY(w.emoticonList().isEmpty());
}
