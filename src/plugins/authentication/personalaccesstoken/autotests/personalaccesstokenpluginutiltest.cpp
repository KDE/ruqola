/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "personalaccesstokenpluginutiltest.h"
#include "../personalaccesstokenpluginutil.h"
#include <QTest>
QTEST_GUILESS_MAIN(PersonalAccessTokenPluginUtilTest)
PersonalAccessTokenPluginUtilTest::PersonalAccessTokenPluginUtilTest(QObject *parent)
    : QObject{parent}
{
}

void PersonalAccessTokenPluginUtilTest::shouldHaveDefaultValues()
{
    PersonalAccessTokenPluginUtil::PersonalAccessTokenPluginInfo info;
    QVERIFY(info.token.isEmpty());
    QVERIFY(info.userId.isEmpty());
    QVERIFY(info.isEmpty());
    QVERIFY(!info.isValid());
}

#include "moc_personalaccesstokenpluginutiltest.cpp"
