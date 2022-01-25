/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

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
    // TODO
}
