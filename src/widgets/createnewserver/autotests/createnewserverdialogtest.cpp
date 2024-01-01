/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createnewserverdialogtest.h"
#include "createnewserver/createnewserverdialog.h"
#include "createnewserver/createnewserverstackwidget.h"
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

    auto mCreateNewServerStackWidget = w.findChild<CreateNewServerStackWidget *>(QStringLiteral("mCreateNewServerStackWidget"));
    QVERIFY(mCreateNewServerStackWidget);

    auto buttonBox = w.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(buttonBox);

    const AccountManager::AccountManagerInfo info = w.accountInfo();
    QVERIFY(info.accountName.isEmpty());
    QVERIFY(info.userName.isEmpty());
    QVERIFY(info.serverUrl.isEmpty());
}

#include "moc_createnewserverdialogtest.cpp"
