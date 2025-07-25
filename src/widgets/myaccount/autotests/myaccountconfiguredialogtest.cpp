/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountconfiguredialogtest.h"
using namespace Qt::Literals::StringLiterals;

#include "myaccount/myaccountconfiguredialog.h"
#include "myaccount/myaccountconfigurewidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(MyAccountConfigureDialogTest)
MyAccountConfigureDialogTest::MyAccountConfigureDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void MyAccountConfigureDialogTest::shouldHaveDefaultValue()
{
    MyAccountConfigureDialog w(nullptr);
    QVERIFY(!w.windowTitle().isEmpty());

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mMyAccountConfigWidget = w.findChild<MyAccountConfigureWidget *>(u"mMyAccountConfigWidget"_s);
    QVERIFY(mMyAccountConfigWidget);

    auto button = w.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);
}

#include "moc_myaccountconfiguredialogtest.cpp"
