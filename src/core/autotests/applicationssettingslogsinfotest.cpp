/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

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
    QCOMPARE(log.totalTime(), 0);

    ApplicationsSettingsLogsInfo::LogsArgument args;
    QVERIFY(args.caller.isEmpty());
    QVERIFY(args.method.isEmpty());
    QVERIFY(args.severity.isEmpty());
    QVERIFY(args.args.isEmpty());
}

void ApplicationsSettingsLogsInfoTest::shouldLoadApplicationLogsInfo_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<ApplicationsSettingsLogsInfo>("info");

    {
        ApplicationsSettingsLogsInfo info;
        info.setMethod("app:construct"_L1);
        info.setCreatedAt(QDateTime(QDate(2024, 12, 05), QTime(03, 28, 02, 762), QTimeZone::UTC));
        info.setTotalTime(32);

        QList<ApplicationsSettingsLogsInfo::LogsArgument> args;
        {
            ApplicationsSettingsLogsInfo::LogsArgument arg;
            arg.caller = "anonymous OR constructor -> App"_L1;
            arg.method = "app:construct"_L1;
            arg.severity = "debug"_L1;
            arg.args =
                "[<br/>&nbsp;&nbsp;&nbsp;&nbsp;\"Constructed&nbsp;the&nbsp;App&nbsp;icanhazdadjoke&nbsp;(92b75f5e-5df0-4d4e-8c5b-2184ba3a5d77)\",<br/"
                ">&nbsp;&nbsp;&nbsp;&nbsp;\"v1.0.2&nbsp;which&nbsp;depends&nbsp;on&nbsp;the&nbsp;API&nbsp;v^1.0.0!\",<br/"
                ">&nbsp;&nbsp;&nbsp;&nbsp;\"Created&nbsp;by&nbsp;Kai&nbsp;Alexander&nbsp;Fischer\"<br/>]<br/>";

            args.append(arg);
        }
        {
            ApplicationsSettingsLogsInfo::LogsArgument arg;
            arg.caller = "anonymous OR constructor"_L1;
            arg.method = "app:construct"_L1;
            arg.severity = "debug"_L1;
            arg.args = "[<br/>&nbsp;&nbsp;&nbsp;&nbsp;\"The&nbsp;status&nbsp;is&nbsp;now:&nbsp;constructed\"<br/>]<br/>";
            args.append(arg);
        }
        info.setArguments(args);

        QTest::addRow("test1") << u"test1"_s << info;
    }
}

void ApplicationsSettingsLogsInfoTest::shouldLoadApplicationLogsInfo()
{
    QFETCH(QString, name);
    QFETCH(ApplicationsSettingsLogsInfo, info);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/applicationssettingslogsinfo/"_L1 + name + ".json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);
    ApplicationsSettingsLogsInfo m;
    m.parseLogs(obj);
    QCOMPARE(m, info);
}

#include "moc_applicationssettingslogsinfotest.cpp"
