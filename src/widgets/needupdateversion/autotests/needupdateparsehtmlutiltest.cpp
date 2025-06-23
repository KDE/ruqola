/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "needupdateparsehtmlutiltest.h"
using namespace Qt::Literals::StringLiterals;

#include "needupdateversion/needupdateparsehtmlutil.h"
#include <QTest>

QTEST_MAIN(NeedUpdateParseHtmlUtilTest)

NeedUpdateParseHtmlUtilTest::NeedUpdateParseHtmlUtilTest(QObject *parent)
    : QObject{parent}
{
}

void NeedUpdateParseHtmlUtilTest::shouldExtraDate()
{
    QFETCH(QString, name);
    QFETCH(QString, dateStr);

    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_NEEDUPDATEDIR) + u"/"_s + name + u".html"_s;

    QFile f(originalJsonFile);
    [&]() {
        QVERIFY(f.open(QIODevice::ReadOnly));
    }();
    const auto content = f.readAll();
    f.close();

    QCOMPARE(NeedUpdateParseHtmlUtil::extractDate(QString::fromUtf8(content)), dateStr);
}

void NeedUpdateParseHtmlUtilTest::shouldExtraDate_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("dateStr");

    QTest::addRow("test1") << u"test1"_s << u"2024-12-28"_s;
}

#include "moc_needupdateparsehtmlutiltest.cpp"
