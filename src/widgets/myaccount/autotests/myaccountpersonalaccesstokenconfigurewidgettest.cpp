/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountpersonalaccesstokenconfigurewidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "myaccount/myaccountpersonalaccesstokenconfigurewidget.h"
#include "myaccount/myaccountpersonalaccesstokentreeview.h"
#include <QLineEdit>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(MyAccountPersonalAccessTokenConfigureWidgetTest)
MyAccountPersonalAccessTokenConfigureWidgetTest::MyAccountPersonalAccessTokenConfigureWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void MyAccountPersonalAccessTokenConfigureWidgetTest::shouldHaveDefaultValues()
{
    MyAccountPersonalAccessTokenConfigureWidget w(nullptr);

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mSearchLineWidget = w.findChild<QLineEdit *>(u"mSearchLineWidget"_s);
    QVERIFY(mSearchLineWidget);
    QVERIFY(mSearchLineWidget->text().isEmpty());
    QVERIFY(!mSearchLineWidget->placeholderText().isEmpty());

    auto mPersonalAccessTokenTreeView = w.findChild<MyAccountPersonalAccessTokenTreeView *>(u"mPersonalAccessTokenTreeView"_s);
    QVERIFY(mPersonalAccessTokenTreeView);
}

#include "moc_myaccountpersonalaccesstokenconfigurewidgettest.cpp"
