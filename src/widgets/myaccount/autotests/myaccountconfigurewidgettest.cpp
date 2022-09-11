/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountconfigurewidgettest.h"
#include "myaccount/myaccount2faconfigurewidget.h"
#include "myaccount/myaccountconfigurewidget.h"
#include "myaccount/myaccountpersonalaccesstokenconfigurewidget.h"
#include "myaccount/myaccountpreferenceconfigurewidget.h"
#include "myaccount/myaccountprofileconfigurewidget.h"
#include <QStackedWidget>
#include <QTabWidget>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(MyAccountConfigureWidgetTest)
MyAccountConfigureWidgetTest::MyAccountConfigureWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void MyAccountConfigureWidgetTest::shouldHaveDefaultValues()
{
    MyAccountConfigureWidget w(nullptr);

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto tabWidget = w.findChild<QTabWidget *>(QStringLiteral("tabWidget"));
    QVERIFY(tabWidget);

    auto mMyAccount2ProfileConfigureWidget = w.findChild<MyAccountProfileConfigureWidget *>(QStringLiteral("mMyAccount2ProfileConfigureWidget"));
    QVERIFY(mMyAccount2ProfileConfigureWidget);

    auto mMyAccount2FaConfigureWidget = w.findChild<MyAccount2FaConfigureWidget *>(QStringLiteral("mMyAccount2FaConfigureWidget"));
    QVERIFY(mMyAccount2FaConfigureWidget);

    auto mMyAccountPreferenceConfigureWidget = w.findChild<MyAccountPreferenceConfigureWidget *>(QStringLiteral("mMyAccountPreferenceConfigureWidget"));
    QVERIFY(mMyAccountPreferenceConfigureWidget);

    auto mMyAccountPersonalAccessTokenConfigureWidget =
        w.findChild<MyAccountPersonalAccessTokenConfigureWidget *>(QStringLiteral("mMyAccountPersonalAccessTokenConfigureWidget"));
    QVERIFY(mMyAccountPersonalAccessTokenConfigureWidget);

    auto mStackedWidget = w.findChild<QStackedWidget *>(QStringLiteral("mStackedWidget"));
    QVERIFY(mStackedWidget);
}
