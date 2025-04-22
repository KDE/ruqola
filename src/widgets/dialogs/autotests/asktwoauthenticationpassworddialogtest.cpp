/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "asktwoauthenticationpassworddialogtest.h"
#include "dialogs/asktwoauthenticationpassworddialog.h"
#include "dialogs/asktwoauthenticationpasswordwidget.h"
#include "rocketchataccount.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(AskTwoAuthenticationPasswordDialogTest)
AskTwoAuthenticationPasswordDialogTest::AskTwoAuthenticationPasswordDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void AskTwoAuthenticationPasswordDialogTest::shouldHaveDefaultValues()
{
    AskTwoAuthenticationPasswordDialog w;
    QVERIFY(!w.windowTitle().isEmpty());
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mAskTwoAuthicationPasswordWidget = w.findChild<AskTwoAuthenticationPasswordWidget *>(QStringLiteral("mAskTwoAuthicationPasswordWidget"));
    QVERIFY(mAskTwoAuthicationPasswordWidget);

    auto buttonBox = w.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(buttonBox);

    QVERIFY(!w.rocketChatAccount());
    RocketChatAccount account;
    account.setAccountName(QStringLiteral("bla"));
    w.setRocketChatAccount(&account);
    QVERIFY(w.rocketChatAccount());
    QCOMPARE(w.rocketChatAccount()->accountName(), QStringLiteral("bla"));
}

#include "moc_asktwoauthenticationpassworddialogtest.cpp"
