/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "personalaccesstokeninfostest.h"
#include "personalaccesstokens/personalaccesstokeninfos.h"
#include <QTest>
QTEST_GUILESS_MAIN(PersonalAccessTokenInfosTest)

PersonalAccessTokenInfosTest::PersonalAccessTokenInfosTest(QObject *parent)
    : QObject{parent}
{
}

void PersonalAccessTokenInfosTest::shouldHaveDefaultValues()
{
    PersonalAccessTokenInfos w;
    // TODO
}

#include "moc_personalaccesstokeninfostest.cpp"
