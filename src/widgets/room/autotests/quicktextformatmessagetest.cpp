/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "quicktextformatmessagetest.h"
#include "room/quicktextformatmessage.h"
#include <QTest>
QTEST_GUILESS_MAIN(QuickTextFormatMessageTest)
QuickTextFormatMessageTest::QuickTextFormatMessageTest(QObject *parent)
    : QObject{parent}
{
}

void QuickTextFormatMessageTest::shouldHaveDefaultValues()
{
    QuickTextFormatMessageTest t;
    // TODO
}
