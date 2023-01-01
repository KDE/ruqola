/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "oauthinfotest.h"
#include "oauth/oauthinfo.h"
#include <QTest>
QTEST_GUILESS_MAIN(OauthInfoTest)

OauthInfoTest::OauthInfoTest(QObject *parent)
    : QObject{parent}
{
}

void OauthInfoTest::shouldHaveDefaultValues()
{
    OauthInfo info;
    QVERIFY(info.identifier().isEmpty());
    QVERIFY(info.name().isEmpty());
    QVERIFY(info.clientId().isEmpty());
    QVERIFY(info.clientSecret().isEmpty());
    QVERIFY(info.redirectUri().isEmpty());
    QVERIFY(info.createdBy().isEmpty());
    QVERIFY(!info.active());
    // TODO
}
