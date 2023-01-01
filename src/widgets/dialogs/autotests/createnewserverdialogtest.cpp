/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createnewserverdialogtest.h"
#include "dialogs/createnewserverdialog.h"
#include "dialogs/createnewserverwidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(CreateNewServerDialogTest)
CreateNewServerDialogTest::CreateNewServerDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void CreateNewServerDialogTest::shouldHaveDefaultValues()
{
    CreateNewServerDialog w;
    QVERIFY(!w.windowTitle().isEmpty());
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mNewAccountWidget = w.findChild<CreateNewServerWidget *>(QStringLiteral("mNewAccountWidget"));
    QVERIFY(mNewAccountWidget);

    auto buttonBox = w.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(buttonBox);

    const AccountManager::AccountManagerInfo info = w.accountInfo();
    QVERIFY(info.accountName.isEmpty());
    QVERIFY(info.userName.isEmpty());
    QVERIFY(info.serverUrl.isEmpty());
}
