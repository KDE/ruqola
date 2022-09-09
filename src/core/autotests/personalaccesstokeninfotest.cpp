/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "personalaccesstokeninfotest.h"
#include "personalaccesstokens/personalaccesstokeninfo.h"
#include <QTest>
QTEST_GUILESS_MAIN(PersonalAccessTokenInfoTest)
PersonalAccessTokenInfoTest::PersonalAccessTokenInfoTest(QObject *parent)
    : QObject{parent}
{
}

void PersonalAccessTokenInfoTest::shouldHaveDefaultValues()
{
    PersonalAccessTokenInfo w;
    QVERIFY(!w.bypassTwoFactor());
    QVERIFY(w.name().isEmpty());
    QVERIFY(w.lastTokenPart().isEmpty());
    QVERIFY(!w.isValid());
}
