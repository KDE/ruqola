/*
  SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

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

    {
        EncryptionUtils::EncryptionInfo info;
        QTest::addRow("empty") << QByteArray() << info;
    }
    {
        EncryptionUtils::EncryptionInfo info;
        QTest::addRow("too-short") << QByteArray("1234567890123456") << info;
    }
    {
        EncryptionUtils::EncryptionInfo info;
        info.vector = QByteArray("1234567890abcdef");
        info.encryptedData = QByteArray("cipher-payload");
        QTest::addRow("iv-and-payload") << QByteArray(info.vector + info.encryptedData) << info;
    }
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

void EncryptionUtilsTest::shouldGenerateRandomPassword()
{
    const QString password1 = EncryptionUtils::generateRandomPassword();
    const QString password2 = EncryptionUtils::generateRandomPassword();

    QCOMPARE(password1.size(), 30);
    QCOMPARE(password2.size(), 30);
    QVERIFY(password1 != password2);
}

#include "moc_encryptionutilstest.cpp"
