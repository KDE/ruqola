/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "licensesmanagertest.h"
#include "licenses/licensesmanager.h"
#include "ruqola_autotest_helper.h"
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(LicensesManagerTest)
LicensesManagerTest::LicensesManagerTest(QObject *parent)
    : QObject{parent}
{
}

void LicensesManagerTest::shouldHaveDefaultValues()
{
    LicensesManager w;
    QVERIFY(w.licenses().isEmpty());
    QVERIFY(!w.hasLicense(u"bla"_s));
}

void LicensesManagerTest::shouldParseLicenses_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QStringList>("licenses");
    {
        const QStringList licenses{"auditing"_L1,
                                   "canned-responses"_L1,
                                   "ldap-enterprise"_L1,
                                   "livechat-enterprise"_L1,
                                   "voip-enterprise"_L1,
                                   "omnichannel-mobile-enterprise"_L1,
                                   "engagement-dashboard"_L1,
                                   "push-privacy"_L1,
                                   "scalability"_L1,
                                   "teams-mention"_L1,
                                   "saml-enterprise"_L1,
                                   "oauth-enterprise"_L1,
                                   "device-management"_L1,
                                   "federation"_L1,
                                   "videoconference-enterprise"_L1,
                                   "message-read-receipt"_L1,
                                   "outlook-calendar"_L1,
                                   "hide-watermark"_L1,
                                   "custom-roles"_L1,
                                   "accessibility-certification"_L1};
        QTest::newRow("test1.json") << u"test1"_s << licenses;
    }
}

void LicensesManagerTest::shouldParseLicenses()
{
    QFETCH(QString, fileName);
    QFETCH(QStringList, licenses);

    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/licensesmanager/"_L1 + fileName + ".json"_L1;
    const QJsonObject fields = AutoTestHelper::loadJsonObject(originalJsonFile);

    LicensesManager m;
    m.parseLicenses(fields);
    const bool equal = (licenses == m.licenses());
    if (!equal) {
        qDebug() << " current value " << m.licenses();
        qDebug() << " expected value " << licenses;
    }
    QVERIFY(equal);
}

#include "moc_licensesmanagertest.cpp"
