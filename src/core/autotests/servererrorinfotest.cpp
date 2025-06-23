/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "servererrorinfotest.h"
using namespace Qt::Literals::StringLiterals;

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
    QVERIFY(info.message().isEmpty());
    QVERIFY(info.accountName().isEmpty());
    QVERIFY(info.dateTime().isValid());
    QVERIFY(!info.identifier().isEmpty());
}

#include "moc_servererrorinfotest.cpp"
