/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createdirectmessagesdialogtest.h"
#include "dialogs/createdirectmessagesdialog.h"
#include "dialogs/createdirectmessageswidget.h"
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(CreateDirectMessagesDialogTest)
CreateDirectMessagesDialogTest::CreateDirectMessagesDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void CreateDirectMessagesDialogTest::shouldHaveDefaultValues()
{
    CreateDirectMessagesDialog w(nullptr);

    QVERIFY(!w.windowTitle().isEmpty());
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mCreateDirectMessagesWidget = w.findChild<CreateDirectMessagesWidget *>(QStringLiteral("mCreateDirectMessagesWidget"));
    QVERIFY(mCreateDirectMessagesWidget);
}
