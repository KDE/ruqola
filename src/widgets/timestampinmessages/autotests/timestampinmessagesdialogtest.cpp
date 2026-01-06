/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "timestampinmessagesdialogtest.h"
#include "timestampinmessages/timestampinmessagesdialog.h"
#include "timestampinmessages/timestampinmessageswidget.h"
#include <QTest>
QTEST_MAIN(TimeStampInMessagesDialogTest)

TimeStampInMessagesDialogTest::TimeStampInMessagesDialogTest(QObject *parent)
    : QObject{parent}
{
}

#include "moc_timestampinmessagesdialogtest.cpp"
