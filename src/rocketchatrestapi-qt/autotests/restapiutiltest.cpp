/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "restapiutiltest.h"
#include "restapiutil.h"
#include <QTest>

QTEST_GUILESS_MAIN(RestApiUtilTest)
RestApiUtilTest::RestApiUtilTest(QObject *parent)
    : QObject(parent)
{
}

void RestApiUtilTest::shouldAdaptUrl_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");
    QTest::newRow("empty") << QString() << QString();
    QTest::newRow("withoutscheme") << QStringLiteral("foo.kde.org") << QStringLiteral("https://foo.kde.org");
    QTest::newRow("withhttps") << QStringLiteral("https://foo.kde.org") << QStringLiteral("https://foo.kde.org");
    QTest::newRow("withhttp") << QStringLiteral("http://foo.kde.org") << QStringLiteral("http://foo.kde.org");
}

void RestApiUtilTest::shouldAdaptUrl()
{
    QFETCH(QString, input);
    QFETCH(QString, output);
    QCOMPARE(RestApiUtil::adaptUrl(input), output);
}

void RestApiUtilTest::shouldConvertRestApiUrlExtensionType()
{
    QCOMPARE(RestApiUtil::restApiUrlExtensionType(RestApiUtil::RestApiUrlExtensionType::V1), QStringLiteral("/api/v1/"));
    QCOMPARE(RestApiUtil::restApiUrlExtensionType(RestApiUtil::RestApiUrlExtensionType::Apps), QStringLiteral("/api/apps/"));
    QCOMPARE(RestApiUtil::restApiUrlExtensionType(RestApiUtil::RestApiUrlExtensionType::NoExtension), QStringLiteral("/api/"));
}

#include "moc_restapiutiltest.cpp"
