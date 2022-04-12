/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagelistviewtest.h"
#include <QTest>
QTEST_MAIN(MessageListViewTest)

MessageListViewTest::MessageListViewTest(QObject *parent)
    : QObject{parent}
{
}

void MessageListViewTest::shouldHaveDefaultValues()
{
    // Qt::Key_ToDoList
}
