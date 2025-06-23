/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "filesforroomfilterproxymodeltest.h"
using namespace Qt::Literals::StringLiterals;

#include "model/filesforroomfilterproxymodel.h"
#include "model/filesforroommodel.h"
#include <QTest>
QTEST_GUILESS_MAIN(FilesForRoomFilterProxyModelTest)

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
    QList<File> mFiles;
    for (int i = 0; i < 10; ++i) {
        File f;
        f.setFileName(u"name%1"_s.arg(i));
        f.setDescription(u"description%1"_s.arg(i));
        f.setUserId(u"userid%1"_s.arg(i).toLatin1());
        mFiles.append(std::move(f));
    }
    model.setFiles(mFiles);
    QCOMPARE(model.rowCount(), 10);
    QCOMPARE(proxy.rowCount(), 10);

    mFiles.clear();
    for (int i = 0; i < 3; ++i) {
        File f;
        f.setFileName(u"name%1"_s.arg(i));
        f.setDescription(u"description%1"_s.arg(i));
        f.setUserId(u"userid%1"_s.arg(i).toLatin1());
        mFiles.append(std::move(f));
    }
    model.setFiles(mFiles);
    QCOMPARE(model.rowCount(), 3);
    QCOMPARE(proxy.rowCount(), 3);
}

#include "moc_filesforroomfilterproxymodeltest.cpp"
