/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "filetest.h"
#include "attachments/file.h"
#include "ruqola_autotest_helper.h"

QTEST_GUILESS_MAIN(FileTest)

FileTest::FileTest(QObject *parent)
    : QObject(parent)
{
}

void FileTest::shouldHaveDefaultValue()
{
    File f;
    QVERIFY(f.fileName().isEmpty());
    QVERIFY(f.description().isEmpty());
    QVERIFY(f.userId().isEmpty());
    QVERIFY(f.url().isEmpty());
    QVERIFY(f.mimeType().isEmpty());
    QVERIFY(f.fileId().isEmpty());
    QVERIFY(f.rid().isEmpty());
    QVERIFY(f.uploadedDateTimeStr().isEmpty());
    QCOMPARE(f.uploadedAt(), -1);
    QCOMPARE(f.complete(), false);
    QVERIFY(f.typeGroup().isEmpty());
}

void FileTest::shouldAssignValue()
{
    File f;
    const QString url = QStringLiteral("foo");
    const QString name = QStringLiteral("bla");
    const QString description = QStringLiteral("des");
    const QByteArray userId = QByteArrayLiteral("ble");
    const QString mimetype = QStringLiteral("ble1");
    const qint64 timeUploaded = 55;
    const QString fileId = QStringLiteral("blabla");
    const QString rId = QStringLiteral("blabla22");
    bool complete = false;
    f.setUrl(url);
    f.setFileName(name);
    f.setDescription(description);
    f.setUserId(userId);
    f.setMimeType(mimetype);
    f.setUploadedAt(timeUploaded);
    f.setFileId(fileId);
    f.setRid(rId);
    f.setComplete(complete);

    QCOMPARE(f.url(), url);
    QCOMPARE(f.fileName(), name);
    QCOMPARE(f.description(), description);
    QCOMPARE(f.userId(), userId);
    QCOMPARE(f.mimeType(), mimetype);
    QCOMPARE(f.uploadedAt(), timeUploaded);
    QCOMPARE(f.fileId(), fileId);
    QCOMPARE(f.rid(), rId);
    QCOMPARE(f.complete(), complete);

    complete = true;
    f.setComplete(complete);
    QCOMPARE(f.complete(), complete);
}

void FileTest::shouldCopyValue()
{
    File f;
    const QString url = QStringLiteral("foo");
    const QString name = QStringLiteral("bla");
    const QString description = QStringLiteral("des");
    const QByteArray userId = QByteArrayLiteral("ble");
    const QString mimetype = QStringLiteral("ble1");
    const QString fileId = QStringLiteral("blabla");
    const QString rId = QStringLiteral("blabla22");
    const qint64 timeUploaded = 55;
    bool complete = true;
    f.setComplete(complete);
    f.setUrl(url);
    f.setFileName(name);
    f.setDescription(description);
    f.setUserId(userId);
    f.setMimeType(mimetype);
    f.setUploadedAt(timeUploaded);
    f.setFileId(fileId);
    f.setRid(rId);

    File f2 = f;
    QCOMPARE(f2, f);
}

void FileTest::shouldParseFile_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<File>("expectedFile");
    QTest::addColumn<bool>("usingRestApi");
    {
        File expected;
        expected.setFileName(QStringLiteral("191135.jpg"));
        expected.setUrl(QStringLiteral("/ufs/FileSystem:Uploads/ybWLKB4FpCkzQXsa/191135.jpg"));
        expected.setUserId(QByteArrayLiteral("aX7va58FuNuq4bcti"));
        expected.setDescription(QString());
        expected.setMimeType(QStringLiteral("image/jpeg"));
        expected.setUploadedAt(1507828418338);
        expected.setFileId(QStringLiteral("ybWLKB4FepCkzQXsa"));
        expected.setRid(QStringLiteral("GENERAL"));
        QTest::newRow("roomfile1") << QStringLiteral("roomfile1") << expected << false;
    }
    {
        // RestAPI
        File expected;
        expected.setFileName(QStringLiteral("Clipboard - February 7, 2018 8:59 AM"));
        expected.setUrl(QStringLiteral("/ufs/FileSystem:Uploads/AoqRSa6GMt3wXCeSo/Clipboard%20-%20February%207,%202018%208:59%20AM"));
        expected.setUserId(QByteArrayLiteral("vEETYfDxakqpM88Zt"));
        expected.setDescription(QString());
        expected.setMimeType(QStringLiteral("image/png"));
        expected.setUploadedAt(1517990371911);
        expected.setFileId(QStringLiteral("AoqRSa6GMt3wXCeSo"));
        expected.setRid(QStringLiteral("GENERAL"));
        expected.setUserName(QStringLiteral("bli"));
        expected.setComplete(true);
        QTest::newRow("roomfile1-restapi") << QStringLiteral("roomfile1") << expected << true;
    }
}

void FileTest::shouldParseFile()
{
    QFETCH(QString, fileName);
    QFETCH(File, expectedFile);
    QFETCH(bool, usingRestApi);
    QString originalJsonFile;
    if (usingRestApi) {
        originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + QLatin1StringView("/json/restapi/") + fileName + QLatin1StringView(".json");
    } else {
        originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + QLatin1StringView("/json/") + fileName + QLatin1StringView(".json");
    }
    const QJsonObject fields = AutoTestHelper::loadJsonObject(originalJsonFile);

    File newFile;
    newFile.parseFile(fields, usingRestApi);
    const bool equal = (newFile == expectedFile);
    if (!equal) {
        qDebug() << " current value " << newFile;
        qDebug() << " expected value " << expectedFile;
    }
    QVERIFY(equal);
}

#include "moc_filetest.cpp"
