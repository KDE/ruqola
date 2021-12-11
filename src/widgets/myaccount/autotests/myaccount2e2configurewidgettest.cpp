/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccount2e2configurewidgettest.h"
#include "myaccount/myaccount2e2configurewidget.h"
#include <QLabel>
#include <QPushButton>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(MyAccount2e2ConfigureWidgetTest)

MyAccount2e2ConfigureWidgetTest::MyAccount2e2ConfigureWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void MyAccount2e2ConfigureWidgetTest::shouldHaveDefaultValues()
{
    MyAccount2e2ConfigureWidget w(nullptr);
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mResetE2eKey = w.findChild<QPushButton *>(QStringLiteral("mResetE2eKey"));
    QVERIFY(mResetE2eKey);
    QVERIFY(!mResetE2eKey->text().isEmpty());

    auto label = w.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());
    QVERIFY(label->wordWrap());
}
