/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountpersonalaccesscreatedialogtest.h"
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

    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mCreateWidget = d.findChild<MyAccountPersonalAccessCreateWidget *>(QStringLiteral("mCreateWidget"));
    QVERIFY(mCreateWidget);

    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
}

#include "moc_myaccountpersonalaccesscreatedialogtest.cpp"
