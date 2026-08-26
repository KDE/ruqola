/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "importaccountjobtest.h"
#include "importexportdata/importdata/importaccountjob.h"
#include "localdatabase/localdatabaseutils.h"
#include <KZip>
#include <QDir>
#include <QSignalSpy>
#include <QStandardPaths>
#include <QTemporaryDir>
#include <QTest>

using namespace Qt::Literals::StringLiterals;

QTEST_GUILESS_MAIN(ImportAccountJobTest)

namespace
{
QString accountConfigPath(const QString &accountName)
{
    return QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + u"/ruqola/"_s + accountName;
}

// Generates the archive layout produced by ExportAccountJob for a single account.
bool createTestArchive(const QString &zipFileName, const QString &accountName)
{
    KZip zip(zipFileName);
    if (!zip.open(QIODevice::WriteOnly)) {
        return false;
    }
    const bool ok = zip.writeFile(u"accounts"_s, accountName.toUtf8()) && zip.writeFile(accountName + u"/config/ruqola.conf"_s, "[General]\n")
        && zip.writeFile(accountName + u"/database/rooms/"_s + accountName + u".sqlite"_s, "rooms-database")
        && zip.writeFile(accountName + u"/database/messages/room1.sqlite"_s, "messages-database");
    return ok && zip.close();
}

bool runImportJob(const QString &zipFileName)
{
    // The job deletes itself once finished.
    auto job = new ImportAccountJob(zipFileName);
    QSignalSpy doneSpy(job, &ImportAccountJob::importDone);
    QSignalSpy failedSpy(job, &ImportAccountJob::importFailed);
    job->start();
    return doneSpy.wait() && failedSpy.isEmpty();
}
}

ImportAccountJobTest::ImportAccountJobTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
    // These locations survive between runs, start from a known state.
    QDir(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + u"/ruqola"_s).removeRecursively();
    QDir(LocalDatabaseUtils::localDatabasePath()).removeRecursively();
}

void ImportAccountJobTest::shouldHaveDefaultValues()
{
    const ImportAccountJob w({});

    // TODO
}

void ImportAccountJobTest::shouldImportAccount()
{
    QTemporaryDir tmpDir;
    QVERIFY(tmpDir.isValid());
    const QString zipFileName = tmpDir.filePath(u"import.zip"_s);
    QVERIFY(createTestArchive(zipFileName, u"account1"_s));

    QVERIFY(runImportJob(zipFileName));

    QVERIFY(QFileInfo::exists(accountConfigPath(u"account1"_s) + u"/ruqola.conf"_s));
    QVERIFY(QFileInfo::exists(LocalDatabaseUtils::localRoomsDatabasePath() + u"account1/account1.sqlite"_s));
    // Message databases are named after the room, they must not be renamed.
    QVERIFY(QFileInfo::exists(LocalDatabaseUtils::localMessagesDatabasePath() + u"account1/room1.sqlite"_s));
}

void ImportAccountJobTest::shouldRenameDatabaseFilesWhenAccountAlreadyExists()
{
    QTemporaryDir tmpDir;
    QVERIFY(tmpDir.isValid());
    const QString zipFileName = tmpDir.filePath(u"import.zip"_s);
    QVERIFY(createTestArchive(zipFileName, u"account2"_s));

    // An account with this name is already installed => it is imported as "account2_1"
    QVERIFY(QDir().mkpath(accountConfigPath(u"account2"_s)));
    QVERIFY(runImportJob(zipFileName));

    QVERIFY(QFileInfo::exists(accountConfigPath(u"account2_1"_s) + u"/ruqola.conf"_s));
    // The database file must follow the new account name, otherwise the account starts without database.
    QVERIFY(QFileInfo::exists(LocalDatabaseUtils::localRoomsDatabasePath() + u"account2_1/account2_1.sqlite"_s));
    QVERIFY(!QFileInfo::exists(LocalDatabaseUtils::localRoomsDatabasePath() + u"account2_1/account2.sqlite"_s));
}

#include "moc_importaccountjobtest.cpp"
