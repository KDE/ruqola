/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "confirmpasswordwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "dialogs/confirmpasswordwidget.h"
#include <KPasswordLineEdit>
#include <QLabel>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ConfirmPasswordWidgetTest)

ConfirmPasswordWidgetTest::ConfirmPasswordWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ConfirmPasswordWidgetTest::shouldHaveDefaultValues()
{
    ConfirmPasswordWidget w;
    QVERIFY(w.password().isEmpty());
    auto mPasswordLineEdit = w.findChild<KPasswordLineEdit *>(u"mPasswordLineEdit"_s);
    QVERIFY(mPasswordLineEdit);
    QVERIFY(mPasswordLineEdit->password().isEmpty());

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto label = w.findChild<QLabel *>(u"label"_s);
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());
}

#include "moc_confirmpasswordwidgettest.cpp"
