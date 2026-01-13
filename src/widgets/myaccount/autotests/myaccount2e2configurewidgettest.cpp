/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccount2e2configurewidgettest.h"

#include "misc/passwordconfirmwidget.h"
#include "myaccount/myaccount2e2configurewidget.h"
#include <QLabel>
#include <QPushButton>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(MyAccount2e2ConfigureWidgetTest)

using namespace Qt::Literals::StringLiterals;
MyAccount2e2ConfigureWidgetTest::MyAccount2e2ConfigureWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void MyAccount2e2ConfigureWidgetTest::shouldHaveDefaultValues()
{
    MyAccount2e2ConfigureWidget w(nullptr);
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto changePasswordLabel = w.findChild<QLabel *>(u"removePasswordlabel"_s);
    QVERIFY(changePasswordLabel);
    QVERIFY(!changePasswordLabel->text().isEmpty());
    QVERIFY(changePasswordLabel->wordWrap());

    auto mResetE2eKey = w.findChild<QPushButton *>(u"mResetE2eKey"_s);
    QVERIFY(mResetE2eKey);
    QVERIFY(!mResetE2eKey->text().isEmpty());

    auto removePasswordlabel = w.findChild<QLabel *>(u"removePasswordlabel"_s);
    QVERIFY(removePasswordlabel);
    QVERIFY(!removePasswordlabel->text().isEmpty());
    QVERIFY(removePasswordlabel->wordWrap());

    auto mPasswordConfirmWidget = w.findChild<PasswordConfirmWidget *>(u"mPasswordConfirmWidget"_s);
    QVERIFY(mPasswordConfirmWidget);

    auto mModifyE2ePassword = w.findChild<QPushButton *>(u"mModifyE2ePassword"_s);
    QVERIFY(mModifyE2ePassword);
    QVERIFY(!mModifyE2ePassword->text().isEmpty());
}

#include "moc_myaccount2e2configurewidgettest.cpp"
