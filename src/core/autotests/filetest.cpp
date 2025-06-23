/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "filetest.h"
#include "attachments/file.h"
#include "ruqola_autotest_helper.h"
using namespace Qt::Literals::StringLiterals;
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
    QVERIFY(f.uploadedDateTimeStr().isEmpty());
    QCOMPARE(f.uploadedAt(), -1);
    QCOMPARE(f.complete(), false);
    QVERIFY(f.typeGroup().isEmpty());
    QVERIFY(f.roomId().isEmpty());
}

void FileTest::shouldAssignValue()
{
    File f;
    const QString url = u"foo"_s;
    const QString name = u"bla"_s;
    const QString description = u"des"_s;
    const QByteArray userId = "ble"_ba;
    const QString mimetype = u"ble1"_s;
    const qint64 timeUploaded = 55;
    const QByteArray fileId("blabla");
    bool complete = false;
    f.setUrl(url);
    f.setFileName(name);
    f.setDescription(description);
    f.setUserId(userId);
    f.setMimeType(mimetype);
    f.setUploadedAt(timeUploaded);
    f.setFileId(fileId);
    f.setComplete(complete);

    QCOMPARE(f.url(), url);
    QCOMPARE(f.fileName(), name);
    QCOMPARE(f.description(), description);
    QCOMPARE(f.userId(), userId);
    QCOMPARE(f.mimeType(), mimetype);
    QCOMPARE(f.uploadedAt(), timeUploaded);
    QCOMPARE(f.fileId(), fileId);
    QCOMPARE(f.complete(), complete);

    complete = true;
    f.setComplete(complete);
    QCOMPARE(f.complete(), complete);
}

void FileTest::shouldCopyValue()
{
    File f;
    const QString url = u"foo"_s;
    const QString name = u"bla"_s;
    const QString description = u"des"_s;
    const QByteArray userId = "ble"_ba;
    const QString mimetype = u"ble1"_s;
    const QByteArray fileId("blabla");
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
        expected.setFileName(u"191135.jpg"_s);
        expected.setUrl(u"/ufs/FileSystem:Uploads/ybWLKB4FpCkzQXsa/191135.jpg"_s);
        expected.setUserId("aX7va58FuNuq4bcti"_ba);
        expected.setDescription(QString());
        expected.setMimeType(u"image/jpeg"_s);
        expected.setUploadedAt(1507828418338);
        expected.setFileId("ybWLKB4FepCkzQXsa"_ba);
        expected.setRoomId("GENERAL"_ba);
        QTest::newRow("roomfile1") << u"roomfile1"_s << expected << false;
    }
    {
        // RestAPI
        File expected;
        expected.setFileName(u"Clipboard - February 7, 2018 8:59 AM"_s);
        expected.setUrl(u"/ufs/FileSystem:Uploads/AoqRSa6GMt3wXCeSo/Clipboard%20-%20February%207,%202018%208:59%20AM"_s);
        expected.setUserId("vEETYfDxakqpM88Zt"_ba);
        expected.setDescription(QString());
        expected.setMimeType(u"image/png"_s);
        expected.setUploadedAt(1517990371911);
        expected.setFileId("AoqRSa6GMt3wXCeSo"_ba);
        expected.setUserName(u"bli"_s);
        expected.setPath(u"/ufs/FileSystem:Uploads/AoqRSa6GMt3wXCeSo/Clipboard%20-%20February%207,%202018%208:59%20AM"_s);
        expected.setComplete(true);
        expected.setRoomId("GENERAL"_ba);
        QTest::newRow("roomfile1-restapi") << u"roomfile1"_s << expected << true;
    }
}

void FileTest::shouldParseFile()
{
    QFETCH(QString, fileName);
    QFETCH(File, expectedFile);
    QFETCH(bool, usingRestApi);
    QString originalJsonFile;
    if (usingRestApi) {
        originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/json/restapi/"_L1 + fileName + ".json"_L1;
    } else {
        originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/json/"_L1 + fileName + ".json"_L1;
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
