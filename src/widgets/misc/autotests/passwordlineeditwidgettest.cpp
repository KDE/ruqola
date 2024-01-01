/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "passwordlineeditwidgettest.h"
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
    auto mainLayout = w.findChild<QHBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mPasswordLineEdit = w.findChild<KPasswordLineEdit *>(QStringLiteral("mPasswordLineEdit"));
    QVERIFY(mPasswordLineEdit);

    auto mResetPasswordButton = w.findChild<QPushButton *>(QStringLiteral("mResetPasswordButton"));
    QVERIFY(mResetPasswordButton);
    QVERIFY(!mResetPasswordButton->text().isEmpty());
}

#include "moc_passwordlineeditwidgettest.cpp"
