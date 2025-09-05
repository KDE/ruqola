/*
  SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "encryptionutilstest.h"
#include "encryption/encryptionutils.h"
#include <QTest>
QTEST_GUILESS_MAIN(EncryptionUtilsTest)
EncryptionUtilsTest::EncryptionUtilsTest(QObject *parent)
    : QObject{parent}
{
}

void EncryptionUtilsTest::shouldExportJWKKey()
{
    // TODO
}

void EncryptionUtilsTest::shouldSplitVectorAndEcryptedData_data()
{
    QTest::addColumn<QByteArray>("encryptedData");
    QTest::addColumn<EncryptionUtils::EncryptionInfo>("encryptionInfo");

    EncryptionUtils::EncryptionInfo info;
    QTest::addRow("empty") << QByteArray() << info;
}

void EncryptionUtilsTest::shouldSplitVectorAndEcryptedData()
{
    QFETCH(QByteArray, encryptedData);
    QFETCH(EncryptionUtils::EncryptionInfo, encryptionInfo);
    QCOMPARE(EncryptionUtils::splitVectorAndEcryptedData(encryptedData), encryptionInfo);
}

void EncryptionUtilsTest::shouldJoinVectorAndEcryptedData_data()
{
    QTest::addColumn<EncryptionUtils::EncryptionInfo>("encryptionInfo");
    QTest::addColumn<QByteArray>("encryptedData");

    {
        EncryptionUtils::EncryptionInfo info;
        QTest::addRow("empty") << info << QByteArray();
    }
    {
        EncryptionUtils::EncryptionInfo info;
        info.encryptedData = "blafoo-z";
        info.vector = "AAAPPLLLAPPPAPAPPAPA";
        QTest::addRow("test1") << info << QByteArray(info.vector + info.encryptedData);
    }
}

void EncryptionUtilsTest::shouldJoinVectorAndEcryptedData()
{
    QFETCH(EncryptionUtils::EncryptionInfo, encryptionInfo);
    QFETCH(QByteArray, encryptedData);
    QCOMPARE(EncryptionUtils::joinVectorAndEcryptedData(encryptionInfo), encryptedData);
}

#include "moc_encryptionutilstest.cpp"
