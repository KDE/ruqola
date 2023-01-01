/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "textselectionimpltest.h"
#include "delegateutils/textselectionimpl.h"
#include <QTest>

QTEST_MAIN(TextSelectionImplTest)

TextSelectionImplTest::TextSelectionImplTest(QObject *parent)
    : QObject{parent}
{
}

void TextSelectionImplTest::shouldHaveDefaultValues()
{
    TextSelectionImpl impl;
    QVERIFY(impl.textSelection());
    QVERIFY(!impl.mightStartDrag());
}
