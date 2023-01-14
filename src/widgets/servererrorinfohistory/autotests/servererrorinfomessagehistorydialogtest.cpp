/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "servererrorinfomessagehistorydialogtest.h"
#include "servererrorinfohistory/servererrorinfomessagehistorydialog.h"
#include "servererrorinfohistory/servererrorinfomessagehistorywidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(ServerErrorInfoMessageHistoryDialogTest)
ServerErrorInfoMessageHistoryDialogTest::ServerErrorInfoMessageHistoryDialogTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void ServerErrorInfoMessageHistoryDialogTest::shouldHaveDefaultValues()
{
    ServerErrorInfoMessageHistoryDialog d;
    QVERIFY(!d.windowTitle().isEmpty());
    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    auto mServerErrorInfoMessageHistoryWidget = d.findChild<ServerErrorInfoMessageHistoryWidget *>(QStringLiteral("mServerErrorInfoMessageHistoryWidget"));
    QVERIFY(mServerErrorInfoMessageHistoryWidget);

    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
}
