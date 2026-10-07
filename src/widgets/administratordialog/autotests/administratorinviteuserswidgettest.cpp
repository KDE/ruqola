/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorinviteuserswidgettest.h"

#include "administratordialog/users/administratorinviteuserswidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTest>

using namespace Qt::Literals::StringLiterals;

QTEST_MAIN(AdministratorInviteUsersWidgetTest)
AdministratorInviteUsersWidgetTest::AdministratorInviteUsersWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void AdministratorInviteUsersWidgetTest::shouldHaveDefaultValues()
{
    const AdministratorInviteUsersWidget w;

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

void AdministratorInviteUsersWidgetTest::shouldValidateEmails()
{
    const AdministratorInviteUsersWidget w;
    auto mListEmails = w.findChild<QLineEdit *>(u"mListEmails"_s);
    {
        mListEmails->setText(u"foo,bla"_s);
        QCOMPARE(w.emails(), QStringList() << u"foo"_s << u"bla"_s);
    }
    {
        mListEmails->setText(u"foo, bla"_s);
        QCOMPARE(w.emails(), QStringList() << u"foo"_s << u"bla"_s);
    }
    {
        mListEmails->setText(u"foo, , bla"_s);
        QCOMPARE(w.emails(), QStringList() << u"foo"_s << u"bla"_s);
    }
}

#include "moc_administratorinviteuserswidgettest.cpp"
