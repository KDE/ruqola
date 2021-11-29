/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "authenticationcomboboxtest.h"
#include "common/authenticationcombobox.h"
#include <QTest>
QTEST_MAIN(AuthenticationComboBoxTest)

AuthenticationComboBoxTest::AuthenticationComboBoxTest(QObject *parent)
    : QObject(parent)
{
}

void AuthenticationComboBoxTest::shouldHaveDefaultValues()
{
    AuthenticationComboBox w;
    QCOMPARE(w.count(), 0); // We need to fill it with result of account info
}
