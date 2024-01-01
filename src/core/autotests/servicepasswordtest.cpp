/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "servicepasswordtest.h"
#include "servicepassword.h"
#include <QTest>
QTEST_GUILESS_MAIN(ServicePasswordTest)
ServicePasswordTest::ServicePasswordTest(QObject *parent)
    : QObject(parent)
{
}

void ServicePasswordTest::shouldHaveDefaultValues()
{
    ServicePassword p;
    QVERIFY(!p.totp());
    QVERIFY(!p.email2faEnabled());
    QVERIFY(p.password().isEmpty());
}

#include "moc_servicepasswordtest.cpp"
