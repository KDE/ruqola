/*
   SPDX-FileCopyrightText: 2018-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "filesforroommodeltest.h"
#include "model/filesforroommodel.h"
#include <QSignalSpy>
#include <QTest>

QTEST_GUILESS_MAIN(FilesForRoomModelTest)

FilesForRoomModelTest::FilesForRoomModelTest(QObject *parent)
    : QObject(parent)
{
}

void FilesForRoomModelTest::shouldHaveDefaultValue()
{
    FilesForRoomModel w;
    QVERIFY(w.fileAttachments());
    QSignalSpy rowInsertedSpy(&w, &FilesForRoomModel::rowsInserted);
    // (if it had 0 columns, it would have to emit column insertions, too much trouble)
    QCOMPARE(w.rowCount(), 0);
    QCOMPARE(rowInsertedSpy.count(), 0);
    QVERIFY(!w.loadMoreFilesInProgress());
}

void FilesForRoomModelTest::shouldAddFiles()
{
    FilesForRoomModel w;
    QVector<File> mFiles;
    for (int i = 0; i < 10; ++i) {
        File f;
        f.setFileName(QStringLiteral("name%1").arg(i));
        f.setDescription(QStringLiteral("description%1").arg(i));
        f.setUserId(QStringLiteral("userid%1").arg(i));
        mFiles.append(std::move(f));
    }
    QSignalSpy rowInsertedSpy(&w, &FilesForRoomModel::rowsInserted);

    w.setFiles(mFiles);
    QCOMPARE(w.rowCount(), 10);
    QCOMPARE(rowInsertedSpy.count(), 1);

    mFiles.clear();
    for (int i = 0; i < 3; ++i) {
        File f;
        f.setFileName(QStringLiteral("name%1").arg(i));
        f.setDescription(QStringLiteral("description%1").arg(i));
        f.setUserId(QStringLiteral("userid%1").arg(i));
        mFiles.append(std::move(f));
    }
    w.setFiles(mFiles);
    QCOMPARE(w.rowCount(), 3);
    QCOMPARE(rowInsertedSpy.count(), 2);
}

void FilesForRoomModelTest::shouldVerifyData()
{
    FilesForRoomModel w;
    QVector<File> mFiles;
    for (int i = 0; i < 10; ++i) {
        File f;
        f.setFileName(QStringLiteral("name%1").arg(i));
        f.setDescription(QStringLiteral("description%1").arg(i));
        f.setUserId(QStringLiteral("userid%1").arg(i));
        mFiles.append(std::move(f));
    }

    w.setFiles(mFiles);
    QCOMPARE(w.rowCount(), 10);
    for (int i = 0; i < 10; ++i) {
        QCOMPARE(w.data(w.index(i), FilesForRoomModel::FileName).toString(), QStringLiteral("name%1").arg(i));
        QCOMPARE(w.data(w.index(i), FilesForRoomModel::Description).toString(), QStringLiteral("description%1").arg(i));
        QCOMPARE(w.data(w.index(i), FilesForRoomModel::UserId).toString(), QStringLiteral("userid%1").arg(i));
    }
    mFiles.clear();
    for (int i = 0; i < 3; ++i) {
        File f;
        f.setFileName(QStringLiteral("name%1").arg(i));
        f.setDescription(QStringLiteral("description%1").arg(i));
        f.setUserId(QStringLiteral("userid%1").arg(i));
        mFiles.append(std::move(f));
    }
    w.setFiles(mFiles);
    for (int i = 0; i < 3; ++i) {
        QCOMPARE(w.data(w.index(i), FilesForRoomModel::FileName).toString(), QStringLiteral("name%1").arg(i));
        QCOMPARE(w.data(w.index(i), FilesForRoomModel::Description).toString(), QStringLiteral("description%1").arg(i));
        QCOMPARE(w.data(w.index(i), FilesForRoomModel::UserId).toString(), QStringLiteral("userid%1").arg(i));
    }
}
