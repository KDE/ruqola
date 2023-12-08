/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "authenticationbuttontest.h"
#include "common/authenticationbutton.h"
#include <QTest>
QTEST_MAIN(AuthenticationButtonTest)
AuthenticationButtonTest::AuthenticationButtonTest(QObject *parent)
    : QObject{parent}
{
}

void AuthenticationButtonTest::shouldHaveDefaultValues()
{
    AuthenticationButton w;
    QVERIFY(w.text().isEmpty());
    QVERIFY(!w.authenticationInfo().isValid());
}
