/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "threadmessagedialogtest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mThreadMessageWidget = w.findChild<ThreadMessageWidget *>(u"mThreadMessageWidget"_s);
    QVERIFY(mThreadMessageWidget);

    auto button = w.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);
}

#include "moc_threadmessagedialogtest.cpp"
