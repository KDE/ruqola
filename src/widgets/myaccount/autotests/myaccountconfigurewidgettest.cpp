/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountconfigurewidgettest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto tabWidget = w.findChild<QTabWidget *>(u"tabWidget"_s);
    QVERIFY(tabWidget);

    auto mMyAccount2ProfileConfigureWidget = w.findChild<MyAccountProfileConfigureWidget *>(u"mMyAccount2ProfileConfigureWidget"_s);
    QVERIFY(mMyAccount2ProfileConfigureWidget);

    auto mMyAccount2FaConfigureWidget = w.findChild<MyAccount2FaConfigureWidget *>(u"mMyAccount2FaConfigureWidget"_s);
    QVERIFY(mMyAccount2FaConfigureWidget);

    auto mMyAccountPreferenceConfigureWidget = w.findChild<MyAccountPreferenceConfigureWidget *>(u"mMyAccountPreferenceConfigureWidget"_s);
    QVERIFY(mMyAccountPreferenceConfigureWidget);

    auto mMyAccountPersonalAccessTokenConfigureWidget =
        w.findChild<MyAccountPersonalAccessTokenConfigureWidget *>(u"mMyAccountPersonalAccessTokenConfigureWidget"_s);
    QVERIFY(mMyAccountPersonalAccessTokenConfigureWidget);

    auto mStackedWidget = w.findChild<QStackedWidget *>(u"mStackedWidget"_s);
    QVERIFY(mStackedWidget);
}

#include "moc_myaccountconfigurewidgettest.cpp"
