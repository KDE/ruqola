/*
   SPDX-FileCopyrightText: 2017-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "restapiutiltest.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
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
    QTest::newRow("withoutscheme") << u"foo.kde.org"_s << u"https://foo.kde.org"_s;
    QTest::newRow("withhttps") << u"https://foo.kde.org"_s << u"https://foo.kde.org"_s;
    QTest::newRow("withhttp") << u"http://foo.kde.org"_s << u"http://foo.kde.org"_s;
}

void RestApiUtilTest::shouldAdaptUrl()
{
    QFETCH(QString, input);
    QFETCH(QString, output);
    QCOMPARE(RestApiUtil::adaptUrl(input), output);
}

void RestApiUtilTest::shouldConvertRestApiUrlExtensionType()
{
    QCOMPARE(RestApiUtil::restApiUrlExtensionType(RestApiUtil::RestApiUrlExtensionType::V1), u"/api/v1/"_s);
    QCOMPARE(RestApiUtil::restApiUrlExtensionType(RestApiUtil::RestApiUrlExtensionType::Apps), u"/api/apps/"_s);
    QCOMPARE(RestApiUtil::restApiUrlExtensionType(RestApiUtil::RestApiUrlExtensionType::NoExtension), u"/api/"_s);
}

#include "moc_restapiutiltest.cpp"
