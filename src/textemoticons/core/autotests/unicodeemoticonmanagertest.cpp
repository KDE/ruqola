/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "unicodeemoticonmanagertest.h"
#include "unicodeemoticonmanager.h"
#include <QTest>
QTEST_GUILESS_MAIN(UnicodeEmoticonManagerTest)

UnicodeEmoticonManagerTest::UnicodeEmoticonManagerTest(QObject *parent)
    : QObject{parent}
{
}

void UnicodeEmoticonManagerTest::shouldHaveDefaultValues()
{
    TextEmoticonsCore::UnicodeEmoticonManager w;
    QVERIFY(!w.unicodeEmojiList().isEmpty());
    QVERIFY(!w.categories().isEmpty());
}

#include "moc_unicodeemoticonmanagertest.cpp"
