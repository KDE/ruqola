/*
   Copyright (c) 2018-2021 Laurent Montel <montel@kde.org>

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

#include "filesforroomfilterproxymodeltest.h"
#include "model/filesforroomfilterproxymodel.h"
#include "model/filesforroommodel.h"
#include <QTest>
QTEST_MAIN(FilesForRoomFilterProxyModelTest)

FilesForRoomFilterProxyModelTest::FilesForRoomFilterProxyModelTest(QObject *parent)
    : QObject(parent)
{
}

void FilesForRoomFilterProxyModelTest::shouldHaveDefaultValue()
{
}

void FilesForRoomFilterProxyModelTest::shouldHaveDefaultValues()
{
    FilesForRoomModel model;
    FilesForRoomFilterProxyModel proxy(&model);
    QCOMPARE(proxy.rowCount(), 0);
    QCOMPARE(proxy.filterCaseSensitivity(), Qt::CaseInsensitive);
    QCOMPARE(proxy.filterRole(), FilesForRoomModel::FileName);
    QCOMPARE(proxy.sortRole(), FilesForRoomModel::SortByTimeStamp);
}

void FilesForRoomFilterProxyModelTest::shouldAssignValue()
{
    FilesForRoomFilterProxyModel proxy;
    FilesForRoomModel model;
    proxy.setSourceModel(&model);
    QVector<File> mFiles;
    for (int i = 0; i < 10; ++i) {
        File f;
        f.setFileName(QStringLiteral("name%1").arg(i));
        f.setDescription(QStringLiteral("description%1").arg(i));
        f.setUserId(QStringLiteral("userid%1").arg(i));
        mFiles.append(f);
    }
    model.setFiles(mFiles);
    QCOMPARE(model.rowCount(), 10);
    QCOMPARE(proxy.rowCount(), 10);

    mFiles.clear();
    for (int i = 0; i < 3; ++i) {
        File f;
        f.setFileName(QStringLiteral("name%1").arg(i));
        f.setDescription(QStringLiteral("description%1").arg(i));
        f.setUserId(QStringLiteral("userid%1").arg(i));
        mFiles.append(f);
    }
    model.setFiles(mFiles);
    QCOMPARE(model.rowCount(), 3);
    QCOMPARE(proxy.rowCount(), 3);
}
