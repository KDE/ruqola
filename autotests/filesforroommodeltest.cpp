/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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
    QSignalSpy rowInsertedSpy(&w, &FilesForRoomModel::rowsInserted);
    // (if it had 0 columns, it would have to emit column insertions, too much trouble)
    QCOMPARE(w.rowCount(), 0);
    QCOMPARE(rowInsertedSpy.count(), 0);
}

void FilesForRoomModelTest::shouldAddFiles()
{
    FilesForRoomModel w;
    QVector<File> mFiles;
    for (int i = 0; i < 10; ++i) {
        File f;
        f.setName(QStringLiteral("name%1").arg(i));
        f.setDescription(QStringLiteral("description%1").arg(i));
        f.setUserId(QStringLiteral("userid%1").arg(i));
        mFiles.append(f);
    }
    QSignalSpy rowInsertedSpy(&w, &FilesForRoomModel::rowsInserted);



    w.setFiles(mFiles);
    QCOMPARE(w.rowCount(), 10);
    QCOMPARE(rowInsertedSpy.count(), 1);


    mFiles.clear();
    for (int i = 0; i < 3; ++i) {
        File f;
        f.setName(QStringLiteral("name%1").arg(i));
        f.setDescription(QStringLiteral("description%1").arg(i));
        f.setUserId(QStringLiteral("userid%1").arg(i));
        mFiles.append(f);
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
        f.setName(QStringLiteral("name%1").arg(i));
        f.setDescription(QStringLiteral("description%1").arg(i));
        f.setUserId(QStringLiteral("userid%1").arg(i));
        mFiles.append(f);
    }

    w.setFiles(mFiles);
    QCOMPARE(w.rowCount(), 10);
    for (int i = 0; i < 10; ++i) {
        QCOMPARE(w.data(w.index(i), FilesForRoomModel::UserName).toString(), QStringLiteral("name%1").arg(i));
        QCOMPARE(w.data(w.index(i), FilesForRoomModel::Description).toString(), QStringLiteral("description%1").arg(i));
        QCOMPARE(w.data(w.index(i), FilesForRoomModel::UserId).toString(), QStringLiteral("userid%1").arg(i));
    }
    mFiles.clear();
    for (int i = 0; i < 3; ++i) {
        File f;
        f.setName(QStringLiteral("name%1").arg(i));
        f.setDescription(QStringLiteral("description%1").arg(i));
        f.setUserId(QStringLiteral("userid%1").arg(i));
        mFiles.append(f);
    }
    w.setFiles(mFiles);
    for (int i = 0; i < 3; ++i) {
        QCOMPARE(w.data(w.index(i), FilesForRoomModel::UserName).toString(), QStringLiteral("name%1").arg(i));
        QCOMPARE(w.data(w.index(i), FilesForRoomModel::Description).toString(), QStringLiteral("description%1").arg(i));
        QCOMPARE(w.data(w.index(i), FilesForRoomModel::UserId).toString(), QStringLiteral("userid%1").arg(i));
    }
}
