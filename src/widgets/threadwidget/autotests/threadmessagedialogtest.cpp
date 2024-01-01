/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "threadmessagedialogtest.h"
#include "threadwidget/threadmessagedialog.h"
#include "threadwidget/threadmessagewidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(ThreadMessageDialogTest)
ThreadMessageDialogTest::ThreadMessageDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void ThreadMessageDialogTest::shouldHaveDefaultValues()
{
    ThreadMessageDialog w(nullptr);
    QVERIFY(!w.windowTitle().isEmpty());
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mThreadMessageWidget = w.findChild<ThreadMessageWidget *>(QStringLiteral("mThreadMessageWidget"));
    QVERIFY(mThreadMessageWidget);

    auto button = w.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
}

#include "moc_threadmessagedialogtest.cpp"
