/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountpersonalaccesstokenconfigurewidgettest.h"
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

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mSearchLineWidget = w.findChild<QLineEdit *>(QStringLiteral("mSearchLineWidget"));
    QVERIFY(mSearchLineWidget);
    QVERIFY(mSearchLineWidget->text().isEmpty());
    QVERIFY(!mSearchLineWidget->placeholderText().isEmpty());

    auto mPersonalAccessTokenTreeView = w.findChild<MyAccountPersonalAccessTokenTreeView *>(QStringLiteral("mPersonalAccessTokenTreeView"));
    QVERIFY(mPersonalAccessTokenTreeView);
}
