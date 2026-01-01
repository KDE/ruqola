/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "systemmessagetypeutiltest.h"
using namespace Qt::Literals::StringLiterals;

#include "messages/systemmessagetypeutil.h"
#include <QTest>
QTEST_GUILESS_MAIN(SystemMessageTypeUtilTest)
SystemMessageTypeUtilTest::SystemMessageTypeUtilTest(QObject *parent)
    : QObject{parent}
{
}

void SystemMessageTypeUtilTest::shouldTestEnum()
{
    for (int i = SystemMessageTypeUtil::SystemMessageType::Unknown; i < SystemMessageTypeUtil::SystemMessageType::LastElement; ++i) {
        const QString str = SystemMessageTypeUtil::systemMessageTypeStringFromEnum(static_cast<SystemMessageTypeUtil::SystemMessageType>(i));
        auto enumValue = SystemMessageTypeUtil::systemMessageTypeFromString(str);
        QCOMPARE(enumValue, static_cast<SystemMessageTypeUtil::SystemMessageType>(i));
    }
}

#include "moc_systemmessagetypeutiltest.cpp"
