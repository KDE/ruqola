/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "emoticonunicodeproxymodeltest.h"
#include "emoticonunicodeproxymodel.h"
#include <QTest>
QTEST_GUILESS_MAIN(EmoticonUnicodeProxyModelTest)
EmoticonUnicodeProxyModelTest::EmoticonUnicodeProxyModelTest(QObject *parent)
    : QObject{parent}
{
}

void EmoticonUnicodeProxyModelTest::shouldHaveDefaultValues()
{
    TextEmoticonsCore::EmoticonUnicodeProxyModel w;

    QVERIFY(w.category().isEmpty());
    QVERIFY(w.recentEmoticons().isEmpty());
    QVERIFY(w.searchIdentifier().isEmpty());
}
