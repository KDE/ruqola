/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "forwardmessagedialogtest.h"
#include "forwardmessage/forwardmessagedialog.h"
#include <QTest>
QTEST_MAIN(ForwardMessageDialogTest)
ForwardMessageDialogTest::ForwardMessageDialogTest(QObject *parent)
    : QObject{parent}
{
}

void ForwardMessageDialogTest::shouldHaveDefaultValues()
{
    ForwardMessageDialog w;
    // TODO
}

#include "moc_forwardmessagedialogtest.cpp"
