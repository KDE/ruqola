/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "passwordvalidatewidgettest.h"
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
    auto mGridLayout = w.findChild<QGridLayout *>(QStringLiteral("mGridLayout"));
    QVERIFY(mGridLayout);
    QCOMPARE(mGridLayout->contentsMargins(), QMargins{});
    QCOMPARE(mGridLayout->spacing(), 0);

    PasswordValidateLabel l;
    auto mainLayout = l.findChild<QHBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});
    QCOMPARE(mainLayout->spacing(), 0);

    auto mIconLabel = l.findChild<QLabel *>(QStringLiteral("mIconLabel"));
    QVERIFY(mIconLabel);

    auto mLabel = l.findChild<QLabel *>(QStringLiteral("mLabel"));
    QVERIFY(mLabel);
}

#include "moc_passwordvalidatewidgettest.cpp"
