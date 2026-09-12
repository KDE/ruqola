/*
   SPDX-FileCopyrightText: 2026 Ruqola contributors
   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "accountcredentialstoretest.h"
#include "accountcredentialstore.h"
#include "encryption/e2ekeymanager.h"
#include "rocketchataccount.h"
#include "rocketchataccountsettings.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QSettings>
#include <QSignalSpy>
#include <QStandardPaths>
#include <QTemporaryDir>
#include <QTest>
#include <QTimer>
#include <utility>

using namespace Qt::Literals::StringLiterals;
using Store = AccountCredentialStore;
using Error = Store::Error;
using Kind = Store::Kind;

namespace
{
const QString service = u"Ruqola Credentials"_s;
const QString entry = u"accounts-v1"_s;
const QString legacyService = u"Ruqola"_s;

struct Wallet {
    QMap<QPair<QString, QString>, QByteArray> entries;
    QStringList operations;
    int reads = 0;
    int writes = 0;
    int deletes = 0;
    int failRead = -1;
    int failWrite = -1;
    int failDelete = -1;
    bool corruptWrites = false;
    std::function<void()> afterWrite;
};

class FakeBackend : public QObject, public Store::Backend
{
public:
    explicit FakeBackend(Wallet &wallet)
        : mWallet(wallet)
    {
    }
    void read(const QString &serviceName, const QString &key, Store::Callback callback) override
    {
        QTimer::singleShot(0, this, [this, serviceName, key, callback] {
            mWallet.operations.append(u"read:"_s + serviceName + u':' + key);
            ++mWallet.reads;
            if (mWallet.reads == mWallet.failRead) {
                callback({Error::Unavailable, {}});
            } else if (mWallet.entries.contains({serviceName, key})) {
                callback({Error::None, mWallet.entries.value({serviceName, key})});
            } else {
                callback({Error::NotFound, {}});
            }
        });
    }
    void write(const QString &serviceName, const QString &key, const QByteArray &data, Store::Callback callback) override
    {
        QTimer::singleShot(0, this, [this, serviceName, key, data, callback] {
            mWallet.operations.append(u"write:"_s + serviceName + u':' + key);
            ++mWallet.writes;
            if (mWallet.writes == mWallet.failWrite) {
                callback({Error::Unavailable, {}});
            } else {
                mWallet.entries.insert({serviceName, key}, mWallet.corruptWrites ? QByteArray("corrupt") : data);
                if (mWallet.afterWrite) {
                    const auto hook = std::exchange(mWallet.afterWrite, {});
                    hook();
                }
                callback({});
            }
        });
    }
    void remove(const QString &serviceName, const QString &key, Store::Callback callback) override
    {
        QTimer::singleShot(0, this, [this, serviceName, key, callback] {
            mWallet.operations.append(u"delete:"_s + serviceName + u':' + key);
            ++mWallet.deletes;
            if (mWallet.deletes == mWallet.failDelete) {
                callback({Error::Unavailable, {}});
            } else {
                callback({mWallet.entries.remove({serviceName, key}) ? Error::None : Error::NotFound, {}});
            }
        });
    }

private:
    Wallet &mWallet;
};

QByteArray payload(const QJsonObject &accounts = {})
{
    return QJsonDocument(QJsonObject{{u"version"_s, 1}, {u"accounts"_s, accounts}}).toJson(QJsonDocument::Compact);
}

QJsonObject accounts(const Wallet &wallet)
{
    return QJsonDocument::fromJson(wallet.entries.value({service, entry})).object().value("accounts"_L1).toObject();
}
}

void AccountCredentialStoreTest::shouldReadOnceForAllAccounts()
{
    Wallet wallet;
    wallet.entries.insert(
        {service, entry},
        payload({{u"a"_s, QJsonObject{{u"login"_s, u"password"_s}, {u"encryption"_s, u"key"_s}}}, {u"b"_s, QJsonObject{{u"login"_s, u"other"_s}}}}));
    Store store(std::make_unique<FakeBackend>(wallet), {u"a"_s, u"b"_s});
    QList<QByteArray> results;
    const auto callback = [&results](const Store::Result &result) {
        QCOMPARE(result.error, Error::None);
        results.append(result.data);
    };
    store.read(u"a"_s, Kind::Login, this, callback);
    store.read(u"b"_s, Kind::Login, this, callback);
    store.read(u"a"_s, Kind::Encryption, this, callback);
    QVERIFY(results.isEmpty());
    QTRY_COMPARE(results.size(), 3);
    QCOMPARE(results, QList<QByteArray>({"password", "other", "key"}));
    QCOMPARE(wallet.reads, 1);
    QCOMPARE(wallet.writes, 0);
    store.read(u"a"_s, Kind::Login, this, callback);
    QTRY_COMPARE(results.size(), 4);
    QCOMPARE(wallet.reads, 1);
}

void AccountCredentialStoreTest::shouldMigrateAndCleanLegacyEntries()
{
    Wallet wallet;
    wallet.entries.insert({legacyService, u"a"_s}, "password");
    wallet.entries.insert({legacyService, u"a-encrypted"_s}, "key");
    wallet.entries.insert({legacyService, u"disabled"_s}, QString::fromUtf8("pässword").toUtf8());
    // Account names can equal the new entry's key: its service is separate.
    wallet.entries.insert({legacyService, entry}, "reserved-name");
    Store store(std::make_unique<FakeBackend>(wallet), {u"a"_s, u"disabled"_s, entry});
    bool done = false;
    store.read(u"a"_s, Kind::Login, this, [&done](const Store::Result &result) {
        QCOMPARE(result.error, Error::None);
        QCOMPARE(result.data, "password");
        done = true;
    });
    QTRY_VERIFY(done);
    QCOMPARE(wallet.entries.size(), 1);
    QCOMPARE(accounts(wallet).value("disabled"_L1).toObject().value("login"_L1).toString(), QString::fromUtf8("pässword"));
    QCOMPARE(accounts(wallet).value(entry).toObject().value("login"_L1).toString(), u"reserved-name"_s);
    QCOMPARE(accounts(wallet).value("a"_L1).toObject().value("encryption"_L1).toString(), u"key"_s);
    const auto verificationIndex = wallet.operations.lastIndexOf(u"read:Ruqola Credentials:accounts-v1"_s);
    QVERIFY(verificationIndex > wallet.operations.indexOf(u"write:Ruqola Credentials:accounts-v1"_s));
    QVERIFY(wallet.operations.indexOf(u"delete:Ruqola:a"_s) > verificationIndex);
    QVERIFY(!QJsonDocument::fromJson(wallet.entries.value({service, entry})).object().contains("legacyKeys"_L1));
    const int reads = wallet.reads;
    done = false;
    Store restarted(std::make_unique<FakeBackend>(wallet), {u"a"_s, u"disabled"_s});
    restarted.read(u"disabled"_s, Kind::Login, this, [&done](const Store::Result &result) {
        QCOMPARE(result.error, Error::None);
        done = true;
    });
    QTRY_VERIFY(done);
    QCOMPARE(wallet.reads, reads + 1);
}

void AccountCredentialStoreTest::shouldStopAfterCancellationAndRetryExplicitly()
{
    Wallet wallet;
    wallet.failRead = 1;
    wallet.entries.insert({service, entry}, payload());
    Store store(std::make_unique<FakeBackend>(wallet), {u"a"_s, u"b"_s});
    QSignalSpy failures(&store, &Store::accessFailed);
    int completed = 0;
    const auto failed = [&completed](const Store::Result &result) {
        QCOMPARE(result.error, Error::Unavailable);
        ++completed;
    };
    store.read(u"a"_s, Kind::Login, this, failed);
    store.read(u"b"_s, Kind::Encryption, this, failed);
    QTRY_COMPARE(completed, 2);
    store.write(u"a"_s, Kind::Login, u"new"_s, this, failed);
    QTRY_COMPARE(completed, 3);
    QCOMPARE(wallet.reads, 1);
    QCOMPARE(wallet.writes, 0);
    QCOMPARE(failures.count(), 1);
    store.retry();
    store.write(u"a"_s, Kind::Login, u"new"_s, this, [&completed](const Store::Result &result) {
        QCOMPARE(result.error, Error::None);
        ++completed;
    });
    QTRY_COMPARE(completed, 4);
    QCOMPARE(wallet.reads, 2);
    QCOMPARE(accounts(wallet).value("a"_L1).toObject().value("login"_L1).toString(), u"new"_s);
}

void AccountCredentialStoreTest::shouldPreserveLegacyOnMigrationFailure_data()
{
    QTest::addColumn<int>("readFailure");
    QTest::addColumn<bool>("writeFailure");
    QTest::addColumn<bool>("corruptWrites");
    QTest::newRow("legacy-read-denied") << 2 << false << false;
    QTest::newRow("write-denied") << -1 << true << false;
    QTest::newRow("verification-denied") << 4 << false << false;
    QTest::newRow("verification-mismatch") << -1 << false << true;
}

void AccountCredentialStoreTest::shouldPreserveLegacyOnMigrationFailure()
{
    QFETCH(int, readFailure);
    QFETCH(bool, writeFailure);
    QFETCH(bool, corruptWrites);
    Wallet wallet;
    wallet.failRead = readFailure;
    wallet.failWrite = writeFailure ? 1 : -1;
    wallet.corruptWrites = corruptWrites;
    wallet.entries.insert({legacyService, u"a"_s}, "password");
    Store store(std::make_unique<FakeBackend>(wallet), {u"a"_s});
    bool done = false;
    store.read(u"a"_s, Kind::Login, this, [&done](const Store::Result &result) {
        QVERIFY(result.error != Error::None);
        done = true;
    });
    QTRY_VERIFY(done);
    QCOMPARE(wallet.entries.value({legacyService, u"a"_s}), "password");
    QCOMPARE(wallet.deletes, 0);
}

void AccountCredentialStoreTest::shouldResumeInterruptedCleanup()
{
    Wallet wallet;
    wallet.entries.insert({legacyService, u"a"_s}, "password");
    wallet.entries.insert({legacyService, u"a-encrypted"_s}, "key");
    wallet.failDelete = 2;
    {
        Store store(std::make_unique<FakeBackend>(wallet), {u"a"_s});
        bool done = false;
        store.read(u"a"_s, Kind::Login, this, [&done](const Store::Result &result) {
            QCOMPARE(result.error, Error::Unavailable);
            done = true;
        });
        QTRY_VERIFY(done);
        QVERIFY(wallet.entries.contains({service, entry}));
        QVERIFY(!wallet.entries.contains({legacyService, u"a"_s}));
        QVERIFY(wallet.entries.contains({legacyService, u"a-encrypted"_s}));
    }
    Store restarted(std::make_unique<FakeBackend>(wallet), {u"a"_s});
    bool done = false;
    restarted.read(u"a"_s, Kind::Encryption, this, [&done](const Store::Result &result) {
        QCOMPARE(result.error, Error::None);
        QCOMPARE(result.data, "key");
        done = true;
    });
    QTRY_VERIFY(done);
    QCOMPARE(wallet.entries.size(), 1);
}

void AccountCredentialStoreTest::shouldRejectInvalidData_data()
{
    QTest::addColumn<QByteArray>("data");
    QTest::newRow("corrupt") << QByteArray("invalid");
    QTest::newRow("future-version") << QByteArray(R"({"version":2,"accounts":{}})");
    QTest::newRow("bad-password") << QByteArray(R"({"version":1,"accounts":{"a":{"login":42}}})");
    QTest::newRow("unbacked-cleanup") << QByteArray(R"({"version":1,"accounts":{},"legacyKeys":["a"]})");
    QTest::newRow("bad-cleanup") << QByteArray(R"({"version":1,"accounts":{},"legacyKeys":[42]})");
}

void AccountCredentialStoreTest::shouldRejectInvalidData()
{
    QFETCH(QByteArray, data);
    Wallet wallet;
    wallet.entries.insert({service, entry}, data);
    wallet.entries.insert({legacyService, u"a"_s}, "legacy");
    Store store(std::make_unique<FakeBackend>(wallet), {u"a"_s});
    bool done = false;
    store.write(u"a"_s, Kind::Login, u"new"_s, this, [&done](const Store::Result &result) {
        QCOMPARE(result.error, Error::InvalidData);
        done = true;
    });
    QTRY_VERIFY(done);
    QCOMPARE(wallet.reads, 1);
    QCOMPARE(wallet.writes, 0);
    QCOMPARE(wallet.deletes, 0);
    QCOMPARE(wallet.entries.value({service, entry}), data);
}

void AccountCredentialStoreTest::shouldSerializeUpdatesAndRemoveCredentials()
{
    Wallet wallet;
    wallet.entries.insert({service, entry}, payload());
    Store store(std::make_unique<FakeBackend>(wallet), {});
    int completed = 0;
    const auto callback = [&completed](const Store::Result &result) {
        QCOMPARE(result.error, Error::None);
        ++completed;
    };
    store.write(u"a"_s, Kind::Login, u"password"_s, this, callback);
    store.write(u"b"_s, Kind::Login, u"other"_s, this, callback);
    store.write(u"a"_s, Kind::Encryption, u"key"_s, this, callback);
    QTRY_COMPARE(completed, 3);
    QCOMPARE(accounts(wallet).size(), 2);
    QCOMPARE(accounts(wallet).value("a"_L1).toObject().size(), 2);
    store.remove(u"a"_s, Kind::Encryption, this, callback);
    store.removeAccount(u"b"_s, this, callback);
    QTRY_COMPARE(completed, 5);
    QCOMPARE(accounts(wallet).size(), 1);
    QCOMPARE(accounts(wallet).value("a"_L1).toObject(), QJsonObject({{u"login"_s, u"password"_s}}));
    store.write(u"a"_s, Kind::Login, u"password"_s, this, callback);
    QTRY_COMPARE(completed, 6);
    QCOMPARE(wallet.writes, 5); // Unchanged E2E/login passwords do not prompt for another write.
    store.read(u"a"_s, Kind::Encryption, this, [&completed](const Store::Result &result) {
        QCOMPARE(result.error, Error::NotFound);
        ++completed;
    });
    QTRY_COMPARE(completed, 7);
    QCOMPARE(wallet.reads, 1);
}

void AccountCredentialStoreTest::shouldPreserveDataAfterFailedWrite()
{
    Wallet wallet;
    const auto original = payload({{u"a"_s, QJsonObject{{u"login"_s, u"old"_s}}}});
    wallet.entries.insert({service, entry}, original);
    wallet.failWrite = 1;
    Store store(std::make_unique<FakeBackend>(wallet), {});
    bool done = false;
    store.write(u"a"_s, Kind::Login, u"new"_s, this, [&done](const Store::Result &result) {
        QCOMPARE(result.error, Error::Unavailable);
        done = true;
    });
    QTRY_VERIFY(done);
    QCOMPARE(wallet.entries.value({service, entry}), original);
    store.retry();
    done = false;
    store.read(u"a"_s, Kind::Login, this, [&done](const Store::Result &result) {
        QCOMPARE(result.error, Error::None);
        QCOMPARE(result.data, "new");
        done = true;
    });
    QTRY_VERIFY(done);
}

void AccountCredentialStoreTest::shouldGuardCallbacksAndPersistAfterCallerDestruction()
{
    Wallet wallet;
    wallet.entries.insert({service, entry}, payload());
    Store store(std::make_unique<FakeBackend>(wallet), {});
    auto context = std::make_unique<QObject>();
    bool called = false;
    store.write(u"a"_s, Kind::Login, u"password"_s, context.get(), [&called](const Store::Result &) {
        called = true;
    });
    context.reset();
    bool done = false;
    store.read(u"a"_s, Kind::Login, this, [&done](const Store::Result &result) {
        QCOMPARE(result.data, "password");
        done = true;
    });
    QTRY_VERIFY(done);
    QVERIFY(!called);
}

void AccountCredentialStoreTest::shouldNotOverwriteNewPasswordWithPendingRead()
{
    QStandardPaths::setTestModeEnabled(true);
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    const auto fileName = directory.filePath(u"ruqola.conf"_s);
    {
        QSettings config(fileName, QSettings::IniFormat);
        config.setValue("accountName"_L1, u"a"_s);
    }
    Wallet wallet;
    wallet.entries.insert({service, entry}, payload({{u"a"_s, QJsonObject{{u"login"_s, u"old"_s}}}}));
    Store store(std::make_unique<FakeBackend>(wallet), {});
    RocketChatAccountSettings settings(fileName, nullptr, &store);
    QSignalSpy available(&settings, &RocketChatAccountSettings::passwordAvailable);
    settings.setPassword(u"new"_s);
    bool done = false;
    store.read(u"a"_s, Kind::Login, this, [&done](const Store::Result &result) {
        QCOMPARE(result.data, "new");
        done = true;
    });
    QTRY_VERIFY(done);
    QCOMPARE(settings.password(), u"new"_s);
    QCOMPARE(available.count(), 0);
    settings.logout();
    QCOMPARE(settings.password(), u"new"_s);
}

void AccountCredentialStoreTest::shouldLoadPasswordWhenAccountEnabled()
{
    QStandardPaths::setTestModeEnabled(true);
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    const auto fileName = directory.filePath(u"ruqola.conf"_s);
    {
        QSettings config(fileName, QSettings::IniFormat);
        config.setValue("accountName"_L1, u"a"_s);
        config.setValue("enabled"_L1, false);
    }
    Wallet wallet;
    wallet.entries.insert({service, entry}, payload({{u"a"_s, QJsonObject{{u"login"_s, u"password"_s}}}}));
    Store store(std::make_unique<FakeBackend>(wallet), {});
    RocketChatAccountSettings settings(fileName, nullptr, &store);
    QCOMPARE(wallet.reads, 0);
    QSignalSpy available(&settings, &RocketChatAccountSettings::passwordAvailable);
    settings.setAccountEnabled(true);
    QTRY_COMPARE(available.count(), 1);
    QCOMPARE(settings.password(), u"password"_s);
}

void AccountCredentialStoreTest::shouldReloadSettingsOnExplicitRetry()
{
    QStandardPaths::setTestModeEnabled(true);
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    const auto fileName = directory.filePath(u"ruqola.conf"_s);
    {
        QSettings config(fileName, QSettings::IniFormat);
        config.setValue("accountName"_L1, u"a"_s);
    }
    Wallet wallet;
    wallet.entries.insert({service, entry}, payload({{u"a"_s, QJsonObject{{u"login"_s, u"password"_s}}}}));
    wallet.failRead = 1;
    Store store(std::make_unique<FakeBackend>(wallet), {});
    RocketChatAccountSettings settings(fileName, nullptr, &store);
    QSignalSpy failures(&store, &Store::accessFailed);
    QSignalSpy available(&settings, &RocketChatAccountSettings::passwordAvailable);
    QTRY_COMPARE(failures.count(), 1);
    QVERIFY(settings.password().isEmpty());
    store.retry();
    QTRY_COMPARE(available.count(), 1);
    QCOMPARE(settings.password(), u"password"_s);
    QCOMPARE(wallet.reads, 2);
}

void AccountCredentialStoreTest::shouldCompleteEncryptionReadOnFailure()
{
    QStandardPaths::setTestModeEnabled(true);
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    const auto fileName = directory.filePath(u"ruqola.conf"_s);
    {
        QSettings config(fileName, QSettings::IniFormat);
        config.setValue("accountName"_L1, u"a"_s);
    }
    Wallet wallet;
    wallet.failRead = 1;
    Store store(std::make_unique<FakeBackend>(wallet), {});
    RocketChatAccount account(fileName);
    E2eKeyManager manager(&account, nullptr, &store);
    QSignalSpy verified(&manager, &E2eKeyManager::verifyKeyDone);
    manager.verifyExistingKeyForTest({{u"public_key"_s, u"public"_s}, {u"private_key"_s, u"cHJpdmF0ZQ=="_s}});
    QTRY_COMPARE(verified.count(), 1);
    QCOMPARE(manager.status(), E2eKeyManager::Status::NeedToDecryptKey);
    QCOMPARE(wallet.reads, 1);
}

void AccountCredentialStoreTest::shouldIgnoreEncryptionReadAfterReset()
{
    QStandardPaths::setTestModeEnabled(true);
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    const auto fileName = directory.filePath(u"ruqola.conf"_s);
    {
        QSettings config(fileName, QSettings::IniFormat);
        config.setValue("accountName"_L1, u"a"_s);
    }
    Wallet wallet;
    wallet.entries.insert({service, entry}, payload({{u"a"_s, QJsonObject{{u"login"_s, u"password"_s}, {u"encryption"_s, u"old-key"_s}}}}));
    Store store(std::make_unique<FakeBackend>(wallet), {});
    RocketChatAccount account(fileName);
    E2eKeyManager manager(&account, nullptr, &store);
    QSignalSpy verified(&manager, &E2eKeyManager::verifyKeyDone);
    manager.verifyExistingKeyForTest({{u"public_key"_s, u"public"_s}, {u"private_key"_s, u"cHJpdmF0ZQ=="_s}});
    manager.resetKeys();
    bool done = false;
    store.read(u"a"_s, Kind::Encryption, this, [&done](const Store::Result &result) {
        QCOMPARE(result.error, Error::NotFound);
        done = true;
    });
    QTRY_VERIFY(done);
    QCOMPARE(manager.status(), E2eKeyManager::Status::Unknown);
    QCOMPARE(verified.count(), 0);
    QCOMPARE(accounts(wallet).value("a"_L1).toObject(), QJsonObject({{u"login"_s, u"password"_s}}));
}

void AccountCredentialStoreTest::shouldRemoveBothPasswordsAfterSettingsDestruction()
{
    QStandardPaths::setTestModeEnabled(true);
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    const auto fileName = directory.filePath(u"ruqola.conf"_s);
    {
        QSettings config(fileName, QSettings::IniFormat);
        config.setValue("accountName"_L1, u"a"_s);
    }
    Wallet wallet;
    wallet.entries.insert({service, entry}, payload({{u"a"_s, QJsonObject{{u"login"_s, u"password"_s}, {u"encryption"_s, u"key"_s}}}}));
    Store store(std::make_unique<FakeBackend>(wallet), {});
    {
        RocketChatAccountSettings settings(fileName, nullptr, &store);
        settings.removeSettings();
    }
    bool done = false;
    store.read(u"a"_s, Kind::Login, this, [&done](const Store::Result &result) {
        QCOMPARE(result.error, Error::NotFound);
        done = true;
    });
    QTRY_VERIFY(done);
    QVERIFY(accounts(wallet).isEmpty());
}

void AccountCredentialStoreTest::shouldReadCachedCredentialsAfterWriteFailure()
{
    Wallet wallet;
    wallet.entries.insert(
        {service, entry},
        payload({{u"a"_s, QJsonObject{{u"login"_s, u"old"_s}, {u"encryption"_s, u"key"_s}}}, {u"b"_s, QJsonObject{{u"login"_s, u"other"_s}}}}));
    wallet.failWrite = 1;
    Store store(std::make_unique<FakeBackend>(wallet), {});
    int completed = 0;
    store.write(u"a"_s, Kind::Login, u"new"_s, this, [&completed](const Store::Result &result) {
        QCOMPARE(result.error, Error::Unavailable);
        ++completed;
    });
    store.read(u"b"_s, Kind::Login, this, [&completed](const Store::Result &result) {
        QCOMPARE(result.error, Error::None);
        QCOMPARE(result.data, "other");
        ++completed;
    });
    store.read(u"a"_s, Kind::Login, this, [&completed](const Store::Result &result) {
        QCOMPARE(result.error, Error::None);
        QCOMPARE(result.data, "new");
        ++completed;
    });
    // Pending resets/removals must mask old passwords in the cached document.
    store.remove(u"a"_s, Kind::Encryption, this);
    store.removeAccount(u"b"_s, this);
    const auto missing = [&completed](const Store::Result &result) {
        QCOMPARE(result.error, Error::NotFound);
        ++completed;
    };
    store.read(u"a"_s, Kind::Encryption, this, missing);
    store.read(u"b"_s, Kind::Login, this, missing);
    QTRY_COMPARE(completed, 5);
    QCOMPARE(wallet.reads, 1);
    QCOMPARE(wallet.writes, 1);
    QCOMPARE(accounts(wallet).size(), 2); // No successful backend mutation yet.
}

void AccountCredentialStoreTest::shouldNotAnnounceUnchangedPasswordsOnRetry()
{
    QStandardPaths::setTestModeEnabled(true);
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    const auto fileName = directory.filePath(u"ruqola.conf"_s);
    {
        QSettings config(fileName, QSettings::IniFormat);
        config.setValue("accountName"_L1, u"a"_s);
    }
    Wallet wallet;
    wallet.entries.insert({service, entry}, payload({{u"a"_s, QJsonObject{{u"login"_s, u"password"_s}}}}));
    Store store(std::make_unique<FakeBackend>(wallet), {});
    RocketChatAccountSettings settings(fileName, nullptr, &store);
    QSignalSpy available(&settings, &RocketChatAccountSettings::passwordAvailable);
    QSignalSpy failures(&store, &Store::accessFailed);
    QTRY_COMPARE(available.count(), 1);
    wallet.failWrite = 1;
    store.write(u"b"_s, Kind::Login, u"other"_s, this);
    QTRY_COMPARE(failures.count(), 1);
    store.retry();
    bool done = false;
    store.read(u"b"_s, Kind::Login, this, [&done](const Store::Result &result) {
        QCOMPARE(result.error, Error::None);
        done = true;
    });
    QTRY_VERIFY(done);
    QCOMPARE(available.count(), 1);
    QCOMPARE(settings.password(), u"password"_s);
}

void AccountCredentialStoreTest::shouldResumePersistedRemovals_data()
{
    QTest::addColumn<bool>("wholeAccount");
    QTest::addColumn<bool>("failInitialRead");
    QTest::newRow("account-locked-on-load") << true << true;
    QTest::newRow("account-write-failed") << true << false;
    QTest::newRow("encryption-locked-on-load") << false << true;
    QTest::newRow("encryption-write-failed") << false << false;
}

void AccountCredentialStoreTest::shouldResumePersistedRemovals()
{
    QFETCH(bool, wholeAccount);
    QFETCH(bool, failInitialRead);
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    const auto journalPath = directory.filePath(u"removals.ini"_s);
    Wallet wallet;
    wallet.entries.insert({service, entry}, payload({{u"a"_s, QJsonObject{{u"login"_s, u"login-secret"_s}, {u"encryption"_s, u"encryption-secret"_s}}}}));
    wallet.failRead = failInitialRead ? 1 : -1;
    wallet.failWrite = failInitialRead ? -1 : 1;
    {
        Store store(std::make_unique<FakeBackend>(wallet), {}, nullptr, journalPath);
        QSignalSpy failed(&store, &Store::accessFailed);
        if (wholeAccount) {
            QVERIFY(store.removeAccount(u"a"_s, this));
        } else {
            QVERIFY(store.remove(u"a"_s, Kind::Encryption, this));
        }
        QTRY_COMPARE(failed.count(), 1);
        QCOMPARE(accounts(wallet).value("a"_L1).toObject().size(), 2);
        QFile journal(journalPath);
        QVERIFY(journal.open(QIODevice::ReadOnly));
        const auto bytes = journal.readAll();
        QVERIFY(!bytes.contains("login-secret"));
        QVERIFY(!bytes.contains("encryption-secret"));
    }
    // No account read or explicit retry: startup must clean up even with zero accounts.
    Store restarted(std::make_unique<FakeBackend>(wallet), {}, nullptr, journalPath);
    QTRY_VERIFY(!accounts(wallet).value("a"_L1).toObject().contains("encryption"_L1));
    QCOMPARE(accounts(wallet).contains("a"_L1), !wholeAccount);
    if (!wholeAccount) {
        QCOMPARE(accounts(wallet).value("a"_L1).toObject().value("login"_L1).toString(), u"login-secret"_s);
    }
    QTRY_VERIFY(QSettings(journalPath, QSettings::IniFormat).allKeys().isEmpty());
}

void AccountCredentialStoreTest::shouldRemoveLegacyCredentialsAfterAccountConfigIsGone()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    const auto journalPath = directory.filePath(u"removals.ini"_s);
    const auto accountPath = directory.filePath(u"ruqola.conf"_s);
    {
        QSettings settings(accountPath, QSettings::IniFormat);
        settings.setValue("accountName"_L1, u"a"_s);
    }
    Wallet wallet;
    wallet.entries.insert({legacyService, u"a"_s}, "password");
    wallet.entries.insert({legacyService, u"a-encrypted"_s}, "key");
    wallet.failRead = 1;
    {
        Store store(std::make_unique<FakeBackend>(wallet), {u"a"_s}, nullptr, journalPath);
        RocketChatAccountSettings settings(accountPath, nullptr, &store);
        QSignalSpy failed(&store, &Store::accessFailed);
        QVERIFY(settings.removeSettings());
        QVERIFY(!QFileInfo::exists(accountPath));
        QTRY_COMPARE(failed.count(), 1);
    }
    // The journal, not the now-deleted config, supplies the legacy account identity.
    Store restarted(std::make_unique<FakeBackend>(wallet), {}, nullptr, journalPath);
    QTRY_VERIFY(QSettings(journalPath, QSettings::IniFormat).allKeys().isEmpty());
    QCOMPARE(wallet.entries.size(), 1);
    QVERIFY(accounts(wallet).isEmpty());
}

void AccountCredentialStoreTest::shouldReplayRemovalBeforeReaddingAccount()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    const auto journalPath = directory.filePath(u"removals.ini"_s);
    Wallet wallet;
    wallet.entries.insert({service, entry}, payload({{u"a"_s, QJsonObject{{u"login"_s, u"old"_s}, {u"encryption"_s, u"old-key"_s}}}}));
    wallet.failWrite = 1;
    {
        Store store(std::make_unique<FakeBackend>(wallet), {}, nullptr, journalPath);
        QSignalSpy failed(&store, &Store::accessFailed);
        QVERIFY(store.removeAccount(u"a"_s, this));
        QTRY_COMPARE(failed.count(), 1);
    }
    {
        Store restarted(std::make_unique<FakeBackend>(wallet), {}, nullptr, journalPath);
        bool done = false;
        restarted.write(u"a"_s, Kind::Login, u"new"_s, this, [&done](const Store::Result &result) {
            QCOMPARE(result.error, Error::None);
            done = true;
        });
        QTRY_VERIFY(done);
        QCOMPARE(accounts(wallet).value("a"_L1).toObject(), QJsonObject({{u"login"_s, u"new"_s}}));
        QVERIFY(QSettings(journalPath, QSettings::IniFormat).allKeys().isEmpty());
    }
    Store restartedAgain(std::make_unique<FakeBackend>(wallet), {}, nullptr, journalPath);
    bool done = false;
    restartedAgain.read(u"a"_s, Kind::Login, this, [&done](const Store::Result &result) {
        QCOMPARE(result.error, Error::None);
        QCOMPARE(result.data, "new");
        done = true;
    });
    QTRY_VERIFY(done);
}

void AccountCredentialStoreTest::shouldKeepAccountWhenRemovalCannotBeJournaled()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    // A regular file cannot be the journal's parent directory, even outside a sandbox.
    QFile blocker(directory.filePath(u"blocker"_s));
    QVERIFY(blocker.open(QIODevice::WriteOnly));
    blocker.close();
    const auto accountPath = directory.filePath(u"ruqola.conf"_s);
    {
        QSettings config(accountPath, QSettings::IniFormat);
        config.setValue("accountName"_L1, u"a"_s);
    }
    Wallet wallet;
    wallet.entries.insert({service, entry}, payload({{u"a"_s, QJsonObject{{u"login"_s, u"password"_s}}}}));
    Store store(std::make_unique<FakeBackend>(wallet), {}, nullptr, blocker.fileName() + u"/removals.ini"_s);
    RocketChatAccountSettings settings(accountPath, nullptr, &store);
    QVERIFY(!settings.removeSettings());
    QVERIFY(QFileInfo::exists(accountPath));
    QCOMPARE(wallet.writes, 0);
    // QSettings caches failed writes. Restoring disk access must not flush a
    // rejected deletion into the journal and remove this still-existing account.
    QVERIFY(QFile::remove(blocker.fileName()));
    QVERIFY(QDir().mkpath(blocker.fileName()));
    QSettings journal(blocker.fileName() + u"/removals.ini"_s, QSettings::IniFormat);
    journal.sync();
    QVERIFY(journal.allKeys().isEmpty());
}

void AccountCredentialStoreTest::shouldReplayFailedRemovalsOnRetry()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    const auto journalPath = directory.filePath(u"removals.ini"_s);
    Wallet wallet;
    wallet.entries.insert(
        {service, entry},
        payload({{u"a"_s, QJsonObject{{u"login"_s, u"password"_s}, {u"encryption"_s, u"key"_s}}}, {u"b"_s, QJsonObject{{u"login"_s, u"other"_s}}}}));
    wallet.failWrite = 1;
    Store store(std::make_unique<FakeBackend>(wallet), {}, nullptr, journalPath);
    QSignalSpy failed(&store, &Store::accessFailed);
    QVERIFY(store.remove(u"a"_s, Kind::Encryption, this));
    QVERIFY(store.removeAccount(u"b"_s, this));
    QTRY_COMPARE(failed.count(), 1);
    QCOMPARE(QSettings(journalPath, QSettings::IniFormat).allKeys().size(), 2);
    store.retry();
    bool done = false;
    store.read(u"a"_s, Kind::Login, this, [&done](const Store::Result &result) {
        QCOMPARE(result.error, Error::None);
        QCOMPARE(result.data, "password");
        done = true;
    });
    QTRY_VERIFY(done);
    QCOMPARE(accounts(wallet), QJsonObject({{u"a"_s, QJsonObject{{u"login"_s, u"password"_s}}}}));
    QVERIFY(QSettings(journalPath, QSettings::IniFormat).allKeys().isEmpty());
}

void AccountCredentialStoreTest::shouldResumeAfterFinalMigrationWriteFails()
{
    Wallet wallet;
    wallet.entries.insert({legacyService, u"a"_s}, "password");
    wallet.failWrite = 2; // Clearing the cleanup list, after legacy deletion succeeded.
    {
        Store store(std::make_unique<FakeBackend>(wallet), {u"a"_s});
        QSignalSpy failed(&store, &Store::accessFailed);
        store.read(u"a"_s, Kind::Login, this, [](const Store::Result &result) {
            QCOMPARE(result.error, Error::Unavailable);
        });
        QTRY_COMPARE(failed.count(), 1);
        QVERIFY(!wallet.entries.contains({legacyService, u"a"_s}));
    }
    Store restarted(std::make_unique<FakeBackend>(wallet), {u"a"_s});
    bool done = false;
    restarted.read(u"a"_s, Kind::Login, this, [&done](const Store::Result &result) {
        QCOMPARE(result.error, Error::None);
        QCOMPARE(result.data, "password");
        done = true;
    });
    QTRY_VERIFY(done);
    QVERIFY(!QJsonDocument::fromJson(wallet.entries.value({service, entry})).object().contains("legacyKeys"_L1));
}

void AccountCredentialStoreTest::shouldBlockRecreationUntilRemovalJournalClears()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    const auto journalPath = directory.filePath(u"removals.ini"_s);
    Wallet wallet;
    wallet.entries.insert({service, entry}, payload({{u"a"_s, QJsonObject{{u"login"_s, u"old"_s}}}}));
    QByteArray savedJournal;
    wallet.afterWrite = [&] {
        // Lose access to the journal after deleting the wallet entry, before
        // acknowledging the removal. A queued new password must not be saved yet.
        QFile journal(journalPath);
        QVERIFY(journal.open(QIODevice::ReadOnly));
        savedJournal = journal.readAll();
        journal.close();
        QVERIFY(journal.remove());
        QVERIFY(QDir().mkdir(journalPath));
    };
    Store store(std::make_unique<FakeBackend>(wallet), {}, nullptr, journalPath);
    QSignalSpy failed(&store, &Store::accessFailed);
    QVERIFY(store.removeAccount(u"a"_s, this));
    store.write(u"a"_s, Kind::Login, u"new"_s, this);
    QTRY_COMPARE(failed.count(), 1);
    QCOMPARE(wallet.writes, 1);
    QVERIFY(accounts(wallet).isEmpty());
    QVERIFY(QDir().rmdir(journalPath));
    {
        QFile journal(journalPath);
        QVERIFY(journal.open(QIODevice::WriteOnly));
        QCOMPARE(journal.write(savedJournal), savedJournal.size());
    }
    store.retry();
    bool done = false;
    store.read(u"a"_s, Kind::Login, this, [&done](const Store::Result &result) {
        QCOMPARE(result.error, Error::None);
        QCOMPARE(result.data, "new");
        done = true;
    });
    QTRY_VERIFY(done);
    QCOMPARE(accounts(wallet).value("a"_L1).toObject(), QJsonObject({{u"login"_s, u"new"_s}}));
    QVERIFY(QSettings(journalPath, QSettings::IniFormat).allKeys().isEmpty());
}

QTEST_GUILESS_MAIN(AccountCredentialStoreTest)
#include "moc_accountcredentialstoretest.cpp"
