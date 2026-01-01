/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "personalaccesstokeninfostest.h"
#include "personalaccesstokens/personalaccesstokeninfos.h"
#include "ruqola_autotest_helper.h"
#include <QJsonObject>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(PersonalAccessTokenInfosTest)

PersonalAccessTokenInfosTest::PersonalAccessTokenInfosTest(QObject *parent)
    : QObject{parent}
{
}

void PersonalAccessTokenInfosTest::shouldHaveDefaultValues()
{
    PersonalAccessTokenInfos w;
    QCOMPARE(w.count(), 0);
    QVERIFY(w.isEmpty());
}

void PersonalAccessTokenInfosTest::shouldLoadPersonalAccessTokenInfos_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<PersonalAccessTokenInfos>("personalAccessTokenInfos");
    {
        PersonalAccessTokenInfos info;
        QTest::addRow("empty-token") << u"empty-token"_s << info;
    }
    {
        PersonalAccessTokenInfos info;
        QList<PersonalAccessTokenInfo> lst;
        {
            PersonalAccessTokenInfo i;
            i.setBypassTwoFactor(false);
            i.setLastTokenPart("I6mHG3"_L1);
            i.setName("test1"_L1);
            i.setCreatedAt(1741856600248);
            lst.append(i);
        }
        {
            PersonalAccessTokenInfo i;
            i.setBypassTwoFactor(false);
            i.setLastTokenPart("ClGA8G"_L1);
            i.setName("test2"_L1);
            i.setCreatedAt(1741856613045);
            lst.append(i);
        }
        {
            PersonalAccessTokenInfo i;
            i.setBypassTwoFactor(false);
            i.setLastTokenPart("eM7RyS"_L1);
            i.setName("test3"_L1);
            i.setCreatedAt(1741856625084);
            lst.append(i);
        }
        {
            PersonalAccessTokenInfo i;
            i.setBypassTwoFactor(true);
            i.setLastTokenPart("nGVprC"_L1);
            i.setName("test4"_L1);
            i.setCreatedAt(1741856839672);
            lst.append(i);
        }
        info.setPersonalAccessTokenInfos(std::move(lst));
        QTest::addRow("four-tokens") << u"four-tokens"_s << info;
    }
}

void PersonalAccessTokenInfosTest::shouldLoadPersonalAccessTokenInfos()
{
    QFETCH(QString, name);
    QFETCH(PersonalAccessTokenInfos, personalAccessTokenInfos);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/personalaccesstokeninfos/"_L1 + name + ".json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    PersonalAccessTokenInfos r;
    r.parsePersonalAccessTokenInfos(obj);
    const bool equal = (r == personalAccessTokenInfos);
    if (!equal) {
        qDebug() << "ACTUAL " << r;
        qDebug() << "EXPECTED " << personalAccessTokenInfos;
    }
    QVERIFY(equal);
}

#include "moc_personalaccesstokeninfostest.cpp"
