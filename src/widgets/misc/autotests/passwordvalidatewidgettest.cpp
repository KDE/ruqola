/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "passwordvalidatewidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "misc/passwordvalidatewidget.h"

#include <QGridLayout>
#include <QLabel>
#include <QTest>
QTEST_MAIN(PasswordValidateWidgetTest)
PasswordValidateWidgetTest::PasswordValidateWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void PasswordValidateWidgetTest::shouldHaveDefaultValues()
{
    PasswordValidateWidget w;
    auto mGridLayout = w.findChild<QGridLayout *>(u"mGridLayout"_s);
    QVERIFY(mGridLayout);
    QCOMPARE(mGridLayout->contentsMargins(), QMargins{});

    PasswordValidateLabel l;
    auto mainLayout = l.findChild<QHBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});
    QCOMPARE(mainLayout->spacing(), 0);

    auto mIconLabel = l.findChild<QLabel *>(u"mIconLabel"_s);
    QVERIFY(mIconLabel);

    auto mLabel = l.findChild<QLabel *>(u"mLabel"_s);
    QVERIFY(mLabel);
}

#include "moc_passwordvalidatewidgettest.cpp"
