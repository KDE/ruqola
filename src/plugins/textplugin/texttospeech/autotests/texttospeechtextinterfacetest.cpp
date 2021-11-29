/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "texttospeechtextinterfacetest.h"
#include "../texttospeechtextinterface.h"
#include <QMenu>
#include <QTest>
QTEST_MAIN(TextToSpeechTextInterfaceTest)
TextToSpeechTextInterfaceTest::TextToSpeechTextInterfaceTest(QObject *parent)
    : QObject(parent)
{
}

void TextToSpeechTextInterfaceTest::shouldHaveDefaultValues()
{
    TextToSpeechTextInterface w;
    QMenu menu;
    QVERIFY(menu.actions().isEmpty());
    w.addAction(&menu);
    QCOMPARE(menu.actions().count(), 2);
}
