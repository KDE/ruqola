/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagestarredtest.h"
#include "messages/messagestarred.h"
#include <QTest>
QTEST_GUILESS_MAIN(MessageStarredTest)
MessageStarredTest::MessageStarredTest(QObject *parent)
    : QObject(parent)
{
}

void MessageStarredTest::shouldHaveDefaultValue()
{
    MessageStarred m;
    QVERIFY(!m.isStarred());
}

#include "moc_messagestarredtest.cpp"
