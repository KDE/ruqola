/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "passwordlineeditwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "misc/passwordlineeditwidget.h"
#include <KPasswordLineEdit>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTest>
QTEST_MAIN(PasswordLineEditWidgetTest)
PasswordLineEditWidgetTest::PasswordLineEditWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void PasswordLineEditWidgetTest::shouldHaveDefaultValues()
{
    PasswordLineEditWidget w;
    auto mainLayout = w.findChild<QHBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mPasswordLineEdit = w.findChild<KPasswordLineEdit *>(u"mPasswordLineEdit"_s);
    QVERIFY(mPasswordLineEdit);

    auto mResetPasswordButton = w.findChild<QPushButton *>(u"mResetPasswordButton"_s);
    QVERIFY(mResetPasswordButton);
    QVERIFY(!mResetPasswordButton->text().isEmpty());
}

#include "moc_passwordlineeditwidgettest.cpp"
