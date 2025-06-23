/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "fileattachmentstest.h"
using namespace Qt::Literals::StringLiterals;

#include "attachments/fileattachments.h"
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

// TODO implement it
#if 0
void FileAttachmentsTest::shouldLoadThreads_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("threadsCount");
    QTest::addColumn<int>("total");
    QTest::addColumn<int>("offset");

    QTest::addRow("empty") << u"threads-empty"_s << 0 << 0 << 0;
}

void FileAttachmentsTest::shouldLoadThreads()
{
    QFETCH(QString, name);
    QFETCH(int, threadsCount);
    QFETCH(int, total);
    QFETCH(int, offset);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/threads/"_L1 + name + ".json"_L1;
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

#include "moc_fileattachmentstest.cpp"
