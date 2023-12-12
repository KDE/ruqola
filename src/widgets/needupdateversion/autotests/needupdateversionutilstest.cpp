/*
  SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "needupdateversionutilstest.h"
#include "needupdateversion/needupdateversionutils.h"
#include <QStandardPaths>
#include <QTest>
QTEST_GUILESS_MAIN(NeedUpdateVersionUtilsTest)
NeedUpdateVersionUtilsTest::NeedUpdateVersionUtilsTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
    qRegisterMetaType<NeedUpdateVersionUtils::ObsoleteVersion>();
}

void NeedUpdateVersionUtilsTest::shouldReturnObsoleteVersionStatus()
{
    QFETCH(QString, str);
    QFETCH(NeedUpdateVersionUtils::ObsoleteVersion, status);
    QCOMPARE(NeedUpdateVersionUtils::obsoleteVersionStatus(str, QDate(2023, 12, 12)), status);
}

void NeedUpdateVersionUtilsTest::shouldReturnObsoleteVersionStatus_data()
{
    QTest::addColumn<QString>("str");
    QTest::addColumn<NeedUpdateVersionUtils::ObsoleteVersion>("status");
    QTest::addRow("empty") << QString() << NeedUpdateVersionUtils::ObsoleteVersion::Unknown;
    QTest::addRow("test1") << QStringLiteral("foo (23.08.2)") << NeedUpdateVersionUtils::ObsoleteVersion::NotObsoleteYet;
    QTest::addRow("test2") << QStringLiteral("(23.08.2)") << NeedUpdateVersionUtils::ObsoleteVersion::NotObsoleteYet;
    QTest::addRow("test3") << QStringLiteral("(20.08.2)") << NeedUpdateVersionUtils::ObsoleteVersion::OlderThan12Months;
    QTest::addRow("test4") << QStringLiteral("(23.06.2)") << NeedUpdateVersionUtils::ObsoleteVersion::OlderThan6Months;
}

#include "moc_needupdateversionutilstest.cpp"
