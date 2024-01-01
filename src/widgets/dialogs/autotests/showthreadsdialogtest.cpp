/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showthreadsdialogtest.h"
#include "dialogs/showthreadsdialog.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ShowThreadsDialogTest)

ShowThreadsDialogTest::ShowThreadsDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void ShowThreadsDialogTest::shouldHaveDefaultValues()
{
    ShowThreadsDialog w(nullptr);
    QVERIFY(!w.windowTitle().isEmpty());

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto button = w.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
}

#include "moc_showthreadsdialogtest.cpp"
