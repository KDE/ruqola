/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagemaximumsizedialogtest.h"
#include "messagemaximumsizedialog/messagemaximumsizedialog.h"
#include <QTest>
QTEST_MAIN(MessageMaximumSizeDialogTest)

MessageMaximumSizeDialogTest::MessageMaximumSizeDialogTest(QObject *parent)
    : QObject{parent}
{
}

void MessageMaximumSizeDialogTest::shouldHaveDefaultValues()
{
    MessageMaximumSizeDialog d;
    // TODO
}
