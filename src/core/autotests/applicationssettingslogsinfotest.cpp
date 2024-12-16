/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingslogsinfotest.h"
#include "apps/applicationssettingslogsinfo.h"
#include "ruqola_autotest_helper.h"
#include <QTest>

QTEST_GUILESS_MAIN(ApplicationsSettingsLogsInfoTest)
using namespace Qt::Literals::StringLiterals;
ApplicationsSettingsLogsInfoTest::ApplicationsSettingsLogsInfoTest(QObject *parent)
    : QObject{parent}
{
}

void ApplicationsSettingsLogsInfoTest::shouldHaveDefaultValues()
{
    ApplicationsSettingsLogsInfo log;
    QVERIFY(log.method().isEmpty());
    QVERIFY(log.arguments().isEmpty());
    QVERIFY(log.createdAt().isNull());

    ApplicationsSettingsLogsInfo::LogsArgument args;
    QVERIFY(args.caller.isEmpty());
    QVERIFY(args.method.isEmpty());
    QVERIFY(args.severity.isEmpty());
    QVERIFY(args.args.isEmpty());
}

#include "moc_applicationssettingslogsinfotest.cpp"
