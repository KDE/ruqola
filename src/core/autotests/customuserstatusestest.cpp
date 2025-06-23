/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customuserstatusestest.h"
#include "customusers/customuserstatus.h"
#include "customusers/customuserstatuses.h"
#include "ruqola_autotest_helper.h"
#include "utils.h"
using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(CustomUserStatusesTest)

CustomUserStatusesTest::CustomUserStatusesTest(QObject *parent)
    : QObject(parent)
{
}

void CustomUserStatusesTest::shouldHaveDefaultValues()
{
    CustomUserStatuses w;
    QVERIFY(w.customUserStatusList().isEmpty());
    QVERIFY(w.isEmpty());
    QCOMPARE(w.offset(), 0);
    QCOMPARE(w.count(), 0);
    QCOMPARE(w.total(), 0);
}

void CustomUserStatusesTest::shouldLoadUserCustomStatuses_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("customStatusesCount");
    QTest::addColumn<QList<CustomUserStatus>>("customUserStatus");

    {
        QList<CustomUserStatus> result;
        {
            CustomUserStatus f;
            f.setIdentifier("GZHpA5fENrWHRfaoN"_ba);
            f.setName(u"CM"_s);
            f.setStatusType(Utils::presenceStatusFromString(u"busy"_s));
            f.setUpdatedAt(1603106197911);
            result << f;
        }
        {
            CustomUserStatus f;
            f.setIdentifier("tygCbhbgCojk8G28G"_ba);
            f.setName(u"Vacation"_s);
            f.setStatusType(Utils::presenceStatusFromString(u"away"_s));
            f.setUpdatedAt(1588199612532);
            result << f;
        }
        QTest::addRow("customuser2") << u"customuser2"_s << 2 << result;
    }
}

void CustomUserStatusesTest::shouldLoadUserCustomStatuses()
{
    QFETCH(QString, name);
    QFETCH(int, customStatusesCount);
    QFETCH(QList<CustomUserStatus>, customUserStatus);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/customuserstatus/"_L1 + name + ".json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    CustomUserStatuses r;
    r.parseCustomUserStatuses(obj);
    bool compareCustom = r.customUserStatusList() == customUserStatus;
    if (!compareCustom) {
        qDebug() << "current : " << r.customUserStatusList();
        qDebug() << "expected: " << customUserStatus;
    }
    QVERIFY(compareCustom);
    QCOMPARE(r.count(), customStatusesCount);
}

void CustomUserStatusesTest::shouldUpdateUserCustomStatuses_data()
{
    QTest::addColumn<QString>("filename");
    QTest::addColumn<QString>("updatefilename");
    QTest::addColumn<QList<CustomUserStatus>>("customUserStatusInit");
    QTest::addColumn<QList<CustomUserStatus>>("customUserStatusAfterUpdating");

    {
        QList<CustomUserStatus> result;
        {
            CustomUserStatus f;
            f.setIdentifier("GZHpA5fENrWHRfaoN"_ba);
            f.setName(u"CM"_s);
            f.setStatusType(Utils::presenceStatusFromString(u"busy"_s));
            f.setUpdatedAt(1603106197911);
            result << f;
        }
        {
            CustomUserStatus f;
            f.setIdentifier("tygCbhbgCojk8G28G"_ba);
            f.setName(u"Vacation"_s);
            f.setStatusType(Utils::presenceStatusFromString(u"away"_s));
            f.setUpdatedAt(1588199612532);
            result << f;
        }

        QList<CustomUserStatus> updating;
        {
            CustomUserStatus f;
            f.setIdentifier("GZHpA5fENrWHRfaoN"_ba);
            f.setName(u"CMs"_s);
            f.setStatusType(Utils::presenceStatusFromString(u"away"_s));
            f.setUpdatedAt(1603106197911);
            updating << f;
        }
        {
            CustomUserStatus f;
            f.setIdentifier("tygCbhbgCojk8G28G"_ba);
            f.setName(u"Vacation"_s);
            f.setStatusType(Utils::presenceStatusFromString(u"away"_s));
            f.setUpdatedAt(1588199612532);
            updating << f;
        }

        QTest::addRow("customuser2") << u"customuser2"_s << u"customuser2-updating"_s << result << updating;
    }

    {
        QList<CustomUserStatus> result;
        {
            CustomUserStatus f;
            f.setIdentifier("GZHpA5fENrWHRfaoN"_ba);
            f.setName(u"CM"_s);
            f.setStatusType(Utils::presenceStatusFromString(u"busy"_s));
            f.setUpdatedAt(1603106197911);
            result << f;
        }
        {
            CustomUserStatus f;
            f.setIdentifier("tygCbhbgCojk8G28G"_ba);
            f.setName(u"Vacation"_s);
            f.setStatusType(Utils::presenceStatusFromString(u"away"_s));
            f.setUpdatedAt(1588199612532);
            result << f;
        }

        QList<CustomUserStatus> adding;
        {
            CustomUserStatus f;
            f.setIdentifier("GZHpA5fENrWHRfaoN"_ba);
            f.setName(u"CM"_s);
            f.setStatusType(Utils::presenceStatusFromString(u"busy"_s));
            f.setUpdatedAt(1603106197911);
            adding << f;
        }
        {
            CustomUserStatus f;
            f.setIdentifier("tygCbhbgCojk8G28G"_ba);
            f.setName(u"Vacation"_s);
            f.setStatusType(Utils::presenceStatusFromString(u"away"_s));
            f.setUpdatedAt(1588199612532);
            adding << f;
        }
        {
            CustomUserStatus f;
            f.setIdentifier("ycbgxsKtP7sv8HNiS"_ba);
            f.setName(u"bla"_s);
            f.setStatusType(Utils::presenceStatusFromString(u"away"_s));
            f.setUpdatedAt(1603108134976);
            adding << f;
        }
        QTest::addRow("customuser2-adding") << u"customuser2"_s << u"customuser2-adding"_s << result << adding;
    }
}

void CustomUserStatusesTest::shouldUpdateUserCustomStatuses()
{
    QFETCH(QString, filename);
    QFETCH(QString, updatefilename);
    QFETCH(QList<CustomUserStatus>, customUserStatusInit);
    QFETCH(QList<CustomUserStatus>, customUserStatusAfterUpdating);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/customuserstatus/"_L1 + filename + ".json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    // Compare init
    CustomUserStatuses r;
    r.parseCustomUserStatuses(obj);
    bool compareCustom = r.customUserStatusList() == customUserStatusInit;
    if (!compareCustom) {
        qDebug() << "current : " << r.customUserStatusList();
        qDebug() << "expected: " << customUserStatusInit;
    }
    QVERIFY(compareCustom);

    // Compare after updating
    const QString updatingJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/customuserstatus/"_L1 + updatefilename + ".json"_L1;
    const QJsonArray array = AutoTestHelper::loadJsonArrayObject(updatingJsonFile);

    r.updateCustomUserStatues(array);
    compareCustom = r.customUserStatusList() == customUserStatusAfterUpdating;
    if (!compareCustom) {
        qDebug() << "current : " << r.customUserStatusList();
        qDebug() << "expected: " << customUserStatusAfterUpdating;
    }
    QVERIFY(compareCustom);
}

void CustomUserStatusesTest::shouldDeleteUserCustomStatuses_data()
{
    QTest::addColumn<QString>("filename");
    QTest::addColumn<QString>("updatefilename");
    QTest::addColumn<QList<CustomUserStatus>>("customUserStatusInit");
    QTest::addColumn<QList<CustomUserStatus>>("customUserStatusAfterDeleting");

    {
        QList<CustomUserStatus> result;
        {
            CustomUserStatus f;
            f.setIdentifier("GZHpA5fENrWHRfaoN"_ba);
            f.setName(u"CM"_s);
            f.setStatusType(Utils::presenceStatusFromString(u"busy"_s));
            f.setUpdatedAt(1603106197911);
            result << f;
        }
        {
            CustomUserStatus f;
            f.setIdentifier("tygCbhbgCojk8G28G"_ba);
            f.setName(u"Vacation"_s);
            f.setStatusType(Utils::presenceStatusFromString(u"away"_s));
            f.setUpdatedAt(1588199612532);
            result << f;
        }

        QList<CustomUserStatus> updating;
        {
            CustomUserStatus f;
            f.setIdentifier("tygCbhbgCojk8G28G"_ba);
            f.setName(u"Vacation"_s);
            f.setStatusType(Utils::presenceStatusFromString(u"away"_s));
            f.setUpdatedAt(1588199612532);
            updating << f;
        }
        QTest::addRow("customuser2") << u"customuser2"_s << u"customuser2-deleting"_s << result << updating;
    }
}

void CustomUserStatusesTest::shouldDeleteUserCustomStatuses()
{
    QFETCH(QString, filename);
    QFETCH(QString, updatefilename);
    QFETCH(QList<CustomUserStatus>, customUserStatusInit);
    QFETCH(QList<CustomUserStatus>, customUserStatusAfterDeleting);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/customuserstatus/"_L1 + filename + ".json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    // Compare init
    CustomUserStatuses r;
    r.parseCustomUserStatuses(obj);
    bool compareCustom = r.customUserStatusList() == customUserStatusInit;
    if (!compareCustom) {
        qDebug() << "current : " << r.customUserStatusList();
        qDebug() << "expected: " << customUserStatusInit;
    }
    QVERIFY(compareCustom);

    // Compare after updating
    const QString updatingJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/customuserstatus/"_L1 + updatefilename + ".json"_L1;
    const QJsonArray array = AutoTestHelper::loadJsonArrayObject(updatingJsonFile);

    r.deleteCustomUserStatuses(array);
    compareCustom = r.customUserStatusList() == customUserStatusAfterDeleting;
    if (!compareCustom) {
        qDebug() << "current : " << r.customUserStatusList();
        qDebug() << "expected: " << customUserStatusAfterDeleting;
    }
    QVERIFY(compareCustom);
}

#include "moc_customuserstatusestest.cpp"
