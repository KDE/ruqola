/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticonunicodeutilstest.h"
#include "emoticonunicodeutils.h"
#include <QTest>

QTEST_GUILESS_MAIN(EmoticonUnicodeUtilsTest)

EmoticonUnicodeUtilsTest::EmoticonUnicodeUtilsTest(QObject *parent)
    : QObject{parent}
{
}

void EmoticonUnicodeUtilsTest::shouldHaveDefaultValues()
{
    QCOMPARE(TextEmoticonsCore::EmoticonUnicodeUtils::recentIdentifier(), QStringLiteral("recents"));
    QCOMPARE(TextEmoticonsCore::EmoticonUnicodeUtils::customIdentifier(), QStringLiteral("customs"));
}

#include "moc_emoticonunicodeutilstest.cpp"
