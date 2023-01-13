/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "servererrorinfotest.h"
#include "servererrorinfo.h"
#include <QTest>

QTEST_GUILESS_MAIN(ServerErrorInfoTest)
ServerErrorInfoTest::ServerErrorInfoTest(QObject *parent)
    : QObject{parent}
{
}

void ServerErrorInfoTest::shouldHaveDefaultValues()
{
    ServerErrorInfo info;
    // TODO
}
