/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appscountinfotest.h"
#include "apps/appscountinfo.h"
#include "ruqola_autotest_helper.h"
#include <QJsonObject>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(AppsCountInfoTest)
AppsCountInfoTest::AppsCountInfoTest(QObject *parent)
    : QObject{parent}
{
}

void AppsCountInfoTest::shouldHaveDefaultValues()
{
    AppsCountInfo w;
    QCOMPARE(w.maxMarketplaceApps(), -1);
    QCOMPARE(w.maxPrivateApps(), -1);
    QCOMPARE(w.totalMarketplaceEnabled(), -1);
    QCOMPARE(w.totalPrivateEnabled(), -1);
}

void AppsCountInfoTest::shouldLoadAppsCountInfo_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<AppsCountInfo>("appscountinfo");

    {
        AppsCountInfo appsCountInfo;
        appsCountInfo.setMaxMarketplaceApps(-1);
        appsCountInfo.setMaxPrivateApps(-1);
        appsCountInfo.setTotalMarketplaceEnabled(1);
        appsCountInfo.setTotalPrivateEnabled(1);
        QTest::addRow("test1") << u"test1"_s << appsCountInfo;
    }

    {
        AppsCountInfo appsCountInfo;
        appsCountInfo.setMaxMarketplaceApps(5);
        appsCountInfo.setMaxPrivateApps(6);
        appsCountInfo.setTotalMarketplaceEnabled(2);
        appsCountInfo.setTotalPrivateEnabled(3);
        QTest::addRow("test2") << u"test2"_s << appsCountInfo;
    }
}

void AppsCountInfoTest::shouldLoadAppsCountInfo()
{
    QFETCH(QString, name);
    QFETCH(AppsCountInfo, appscountinfo);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/appscountinfo/"_L1 + name + ".json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);
    AppsCountInfo m;
    m.parseCountInfo(obj);
    QCOMPARE(m, appscountinfo);
}

#include "moc_appscountinfotest.cpp"
