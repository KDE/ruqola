/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

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

#include "fileattachmentstest.h"
#include "fileattachments.h"
#include <QTest>
QTEST_GUILESS_MAIN(FileAttachmentsTest)
FileAttachmentsTest::FileAttachmentsTest(QObject *parent)
    : QObject(parent)
{
}

void FileAttachmentsTest::shouldHaveDefaultValues()
{
    FileAttachments w;
    QVERIFY(w.isEmpty());
    QCOMPARE(w.offset(), 0);
    QCOMPARE(w.total(), 0);
    QCOMPARE(w.filesCount(), 0);
}

//TODO implement it
#if 0
void FileAttachmentsTest::shouldLoadThreads_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("threadsCount");
    QTest::addColumn<int>("total");
    QTest::addColumn<int>("offset");

    QTest::addRow("empty") << QStringLiteral("threads-empty") << 0 << 0 << 0;
}

void FileAttachmentsTest::shouldLoadThreads()
{
    QFETCH(QString, name);
    QFETCH(int, threadsCount);
    QFETCH(int, total);
    QFETCH(int, offset);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/threads/") + name + QLatin1String(".json");
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject obj = doc.object();

    Threads m;
    m.parseThreads(obj);
    QCOMPARE(m.threadsCount(), threadsCount);
    QCOMPARE(m.total(), total);
    QCOMPARE(m.offset(), offset);
}

#endif
