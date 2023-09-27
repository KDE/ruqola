/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "moderationmessagesdialogtest.h"
#include "administratordialog/moderationconsole/moderationmessagesdialog.h"
#include <QTest>
QTEST_MAIN(ModerationMessagesDialogTest)
ModerationMessagesDialogTest::ModerationMessagesDialogTest(QObject *parent)
    : QObject{parent}
{
}

void ModerationMessagesDialogTest::shouldHaveDefaultValues()
{
    ModerationMessagesDialog d(nullptr);
    // TODO
}

#include "moc_moderationmessagesdialogtest.cpp"
