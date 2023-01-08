/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "autocorrectionutilstest.h"
#include "autocorrectionutils.h"
#include <QTest>
QTEST_MAIN(AutoCorrectionUtilsTest)
AutoCorrectionUtilsTest::AutoCorrectionUtilsTest(QObject *parent)
    : QObject{parent}
{
}

void AutoCorrectionUtilsTest::shouldConvertToLibreOfficeFilename()
{
    QCOMPARE(TextAutoCorrection::AutoCorrectionUtils::libreofficeFile(QStringLiteral("Fr_fr")), QStringLiteral("acor_Fr_fr.dat"));
}

void AutoCorrectionUtilsTest::shouldSplitString()
{
    QFETCH(QString, words);
    QFETCH(QStringList, result);
    const QStringList resultLst{TextAutoCorrection::AutoCorrectionUtils::wordsFromSentence(words)};
    const bool equal = (resultLst == result);
    if (!equal) {
        qDebug() << "resultLst" << resultLst;
        qDebug() << "expected" << result;
    }
    QVERIFY(equal);
}

void AutoCorrectionUtilsTest::shouldSplitString_data()
{
    QTest::addColumn<QString>("words");
    QTest::addColumn<QStringList>("result");
    QTest::addRow("empty") << QString() << QStringList();
    QTest::addRow("1 word") << QStringLiteral("blabla") << QStringList({QStringLiteral("blabla")});
    QTest::addRow("no word") << QStringLiteral(" ") << QStringList();
    QTest::addRow("2 words") << QStringLiteral("blabla foo") << QStringList({QStringLiteral("blabla foo"), QStringLiteral("foo")});
    QTest::addRow("3 words") << QStringLiteral("blabla foo la")
                             << QStringList({QStringLiteral("blabla foo la"), QStringLiteral("foo la"), QStringLiteral("la")});
    QTest::addRow("1 word") << QStringLiteral("l'blabla") << QStringList({QStringLiteral("l'blabla")});
}
