/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "threadmessagemodeltest.h"
#include "model/threadmessagemodel.h"

#include <QTest>
QTEST_GUILESS_MAIN(ThreadMessageModelTest)

ThreadMessageModelTest::ThreadMessageModelTest(QObject *parent)
    : QObject(parent)
{
}

void ThreadMessageModelTest::shouldHaveDefaultValues()
{
    ThreadMessageModel w;
    QVERIFY(w.threadMessageId().isEmpty());
}

#include "moc_threadmessagemodeltest.cpp"
