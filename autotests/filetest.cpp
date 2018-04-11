/*
   Copyright (C) 2018 Montel Laurent <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "filetest.h"
#include "file.h"

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(FileTest)

FileTest::FileTest(QObject *parent)
    : QObject(parent)
{
}

void FileTest::shouldHaveDefaultValue()
{
    File f;
    QVERIFY(f.name().isEmpty());
    QVERIFY(f.description().isEmpty());
    QVERIFY(f.userId().isEmpty());
    QVERIFY(f.url().isEmpty());
    QVERIFY(f.mimeType().isEmpty());
    QVERIFY(f.fileId().isEmpty());
    QVERIFY(f.rid().isEmpty());
    QCOMPARE(f.uploadedAt(), -1);
}

void FileTest::shouldAssignValue()
{
    File f;
    const QString url = QStringLiteral("foo");
    const QString name = QStringLiteral("bla");
    const QString description = QStringLiteral("des");
    const QString userId = QStringLiteral("ble");
    const QString mimetype = QStringLiteral("ble1");
    const qint64 timeUploaded = 55;
    const QString fileId = QStringLiteral("blabla");
    const QString rId = QStringLiteral("blabla22");
    f.setUrl(url);
    f.setName(name);
    f.setDescription(description);
    f.setUserId(userId);
    f.setMimeType(mimetype);
    f.setUploadedAt(timeUploaded);
    f.setFileId(fileId);
    f.setRid(rId);

    QCOMPARE(f.url(), url);
    QCOMPARE(f.name(), name);
    QCOMPARE(f.description(), description);
    QCOMPARE(f.userId(), userId);
    QCOMPARE(f.mimeType(), mimetype);
    QCOMPARE(f.uploadedAt(), timeUploaded);
    QCOMPARE(f.fileId(), fileId);
    QCOMPARE(f.rid(), rId);
}

void FileTest::shouldCopyValue()
{
    File f;
    const QString url = QStringLiteral("foo");
    const QString name = QStringLiteral("bla");
    const QString description = QStringLiteral("des");
    const QString userId = QStringLiteral("ble");
    const QString mimetype = QStringLiteral("ble1");
    const QString fileId = QStringLiteral("blabla");
    const QString rId = QStringLiteral("blabla22");
    const qint64 timeUploaded = 55;
    f.setUrl(url);
    f.setName(name);
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
    File expected;
    expected.setName(QStringLiteral("191135.jpg"));
    expected.setUrl(QStringLiteral("/ufs/FileSystem:Uploads/ybWLKB4FpCkzQXsa/191135.jpg"));
    expected.setUserId(QStringLiteral("aX7va58FuNuq4bcti"));
    expected.setDescription(QString());
    expected.setMimeType(QStringLiteral("image/jpeg"));
    expected.setUploadedAt(1507828418338);
    expected.setFileId(QStringLiteral("ybWLKB4FepCkzQXsa"));
    expected.setRid(QStringLiteral("GENERAL"));
    QTest::newRow("roomfile1") << QStringLiteral("roomfile1") << expected;
}

void FileTest::shouldParseFile()
{
    QFETCH(QString, fileName);
    QFETCH(File, expectedFile);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QStringLiteral("/json/") + fileName + QStringLiteral(".json");
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject fields = doc.object();

    File newFile;
    newFile.parseFile(fields);
    const bool equal = (newFile == expectedFile);
    if (!equal) {
        qDebug() << " current value " << newFile;
        qDebug() << " expected value " << expectedFile;
    }
    QVERIFY(equal);
}
