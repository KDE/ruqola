/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "registrationwidgettest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});
}

#include "moc_registrationwidgettest.cpp"
