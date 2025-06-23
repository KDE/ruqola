/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "asktwoauthenticationpassworddialogtest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mAskTwoAuthicationPasswordWidget = w.findChild<AskTwoAuthenticationPasswordWidget *>(u"mAskTwoAuthicationPasswordWidget"_s);
    QVERIFY(mAskTwoAuthicationPasswordWidget);

    auto buttonBox = w.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(buttonBox);

    QVERIFY(!w.rocketChatAccount());
    RocketChatAccount account;
    account.setAccountName(u"bla"_s);
    w.setRocketChatAccount(&account);
    QVERIFY(w.rocketChatAccount());
    QCOMPARE(w.rocketChatAccount()->accountName(), u"bla"_s);
}

#include "moc_asktwoauthenticationpassworddialogtest.cpp"
