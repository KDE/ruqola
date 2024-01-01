/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationmessageswidgettest.h"
#include "administratordialog/moderationconsole/moderationmessageswidget.h"
#include <QTest>

QTEST_MAIN(ModerationMessagesWidgetTest)
ModerationMessagesWidgetTest::ModerationMessagesWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ModerationMessagesWidgetTest::shouldHaveDefaultValues()
{
    ModerationMessagesWidget w(nullptr);
    // TODO
}

#include "moc_moderationmessageswidgettest.cpp"
