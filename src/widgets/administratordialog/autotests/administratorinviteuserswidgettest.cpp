/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorinviteuserswidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "administratordialog/users/administratorinviteuserswidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTest>
QTEST_MAIN(AdministratorInviteUsersWidgetTest)
AdministratorInviteUsersWidgetTest::AdministratorInviteUsersWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void AdministratorInviteUsersWidgetTest::shouldHaveDefaultValues()
{
    AdministratorInviteUsersWidget w;

    auto mListEmails = w.findChild<QLineEdit *>(u"mListEmails"_s);
    QVERIFY(mListEmails);
    QVERIFY(mListEmails->text().isEmpty());
    QVERIFY(!mListEmails->placeholderText().isEmpty());

    auto mainLayout = w.findChild<QHBoxLayout *>(u"formLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto label = w.findChild<QLabel *>(u"label"_s);
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());
}

#include "moc_administratorinviteuserswidgettest.cpp"
