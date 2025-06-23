/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "resetpasswordwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "dialogs/resetpasswordwidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTest>
QTEST_MAIN(ResetPasswordWidgetTest)
ResetPasswordWidgetTest::ResetPasswordWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ResetPasswordWidgetTest::shouldHaveDefaultValues()
{
    ResetPasswordWidget w;

    auto mainLayout = w.findChild<QHBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto label = w.findChild<QLabel *>(u"label"_s);
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    auto mEmail = w.findChild<QLineEdit *>(u"mEmail"_s);
    QVERIFY(mEmail);
    QVERIFY(mEmail->text().isEmpty());
}

#include "moc_resetpasswordwidgettest.cpp"
