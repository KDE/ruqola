/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "registrationwidgettest.h"
#include "administratordialog/registration/registrationwidget.h"
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(RegistrationWidgetTest)
RegistrationWidgetTest::RegistrationWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void RegistrationWidgetTest::shouldHaveDefaultValues()
{
    RegistrationWidget w;

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});
}

#include "moc_registrationwidgettest.cpp"
