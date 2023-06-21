/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorinviteuserswidgettest.h"
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

    auto mListEmails = w.findChild<QLineEdit *>(QStringLiteral("mListEmails"));
    QVERIFY(mListEmails);
    QVERIFY(mListEmails->text().isEmpty());
    QVERIFY(!mListEmails->placeholderText().isEmpty());

    auto mainLayout = w.findChild<QHBoxLayout *>(QStringLiteral("formLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto label = w.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());
}

#include "moc_administratorinviteuserswidgettest.cpp"
