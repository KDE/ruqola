/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createnewserverwidgettest.h"
#include "dialogs/createnewserverwidget.h"
#include <KPasswordLineEdit>
#include <QFormLayout>
#include <QLineEdit>
#include <QTest>
QTEST_MAIN(CreateNewServerWidgetTest)
CreateNewServerWidgetTest::CreateNewServerWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void CreateNewServerWidgetTest::shouldHaveDefaultValues()
{
    CreateNewServerWidget w;

    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), {});

    auto mAccountName = w.findChild<QLineEdit *>(QStringLiteral("mAccountName"));
    QVERIFY(mAccountName);
    QVERIFY(mAccountName->text().isEmpty());
    QVERIFY(mAccountName->isClearButtonEnabled());

    auto mServerName = w.findChild<QLineEdit *>(QStringLiteral("mServerName"));
    QVERIFY(mServerName);
    QVERIFY(mServerName->text().isEmpty());
    QVERIFY(mServerName->isClearButtonEnabled());

    auto mUserName = w.findChild<QLineEdit *>(QStringLiteral("mUserName"));
    QVERIFY(mUserName);
    QVERIFY(mUserName->text().isEmpty());
    QVERIFY(mUserName->isClearButtonEnabled());

    QVERIFY(w.accountInfo().userName.isEmpty());
    QVERIFY(w.accountInfo().serverUrl.isEmpty());
    QVERIFY(w.accountInfo().accountName.isEmpty());

    auto mPasswordLineEdit = w.findChild<KPasswordLineEdit *>(QStringLiteral("mPasswordLineEdit"));
    QVERIFY(mPasswordLineEdit);
}
