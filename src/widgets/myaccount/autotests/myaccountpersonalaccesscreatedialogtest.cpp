/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountpersonalaccesscreatedialogtest.h"
using namespace Qt::Literals::StringLiterals;

#include "myaccount/myaccountpersonalaccesscreatedialog.h"
#include "myaccount/myaccountpersonalaccesscreatewidget.h"
#include <QDialogButtonBox>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(MyAccountPersonalAccessCreateDialogTest)

MyAccountPersonalAccessCreateDialogTest::MyAccountPersonalAccessCreateDialogTest(QObject *parent)
    : QObject{parent}
{
}

void MyAccountPersonalAccessCreateDialogTest::shouldHaveDefaultValues()
{
    MyAccountPersonalAccessCreateDialog d;
    QVERIFY(!d.windowTitle().isEmpty());

    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mCreateWidget = d.findChild<MyAccountPersonalAccessCreateWidget *>(u"mCreateWidget"_s);
    QVERIFY(mCreateWidget);

    auto button = d.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);
}

#include "moc_myaccountpersonalaccesscreatedialogtest.cpp"
