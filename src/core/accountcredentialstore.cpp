/*
   SPDX-FileCopyrightText: 2026 Ruqola contributors
   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "accountcredentialstore.h"
#include "managerdatapaths.h"
#include "ruqola_password_core_debug.h"

#include <QCoreApplication>
#include <QDir>
#include <QDirIterator>
#include <QJsonArray>
#include <QJsonDocument>
#include <QLockFile>
#include <QSettings>
#include <QStandardPaths>
#include <QTimer>
#include <QUuid>
#include <qt6keychain/keychain.h>
#include <utility>

using namespace Qt::Literals::StringLiterals;
namespace
{
// A separate service prevents collisions with arbitrary legacy account names.
const QString service = u"Ruqola Credentials"_s;
const QString entry = u"accounts-v1"_s;
const QString legacyService = u"Ruqola"_s;

class KeychainBackend : public QObject, public AccountCredentialStore::Backend
{
public:
    using Result = AccountCredentialStore::Result;
    using Error = AccountCredentialStore::Error;
    using Callback = AccountCredentialStore::Callback;

    void read(const QString &serviceName, const QString &key, Callback callback) override
    {
        if (!lock(callback)) {
            return;
        }
        auto job = new QKeychain::ReadPasswordJob(serviceName, this);
        job->setKey(key);
        job->setInsecureFallback(false);
        connect(job, &QKeychain::Job::finished, this, [job, callback = std::move(callback)] {
            callback({error(job), job->binaryData()});
        });
        job->start();
    }

    void write(const QString &serviceName, const QString &key, const QByteArray &data, Callback callback) override
    {
        if (!lock(callback)) {
            return;
        }
        auto job = new QKeychain::WritePasswordJob(serviceName, this);
        job->setKey(key);
        job->setBinaryData(data);
        job->setInsecureFallback(false);
        connect(job, &QKeychain::Job::finished, this, [job, callback = std::move(callback)] {
            callback({error(job), {}});
        });
        job->start();
    }

    void remove(const QString &serviceName, const QString &key, Callback callback) override
    {
        if (!lock(callback)) {
            return;
        }
        auto job = new QKeychain::DeletePasswordJob(serviceName, this);
        job->setKey(key);
        job->setInsecureFallback(false);
        connect(job, &QKeychain::Job::finished, this, [job, callback = std::move(callback)] {
            callback({error(job), {}});
        });
        job->start();
    }

private:
    static Error error(QKeychain::Job *job)
    {
        if (job->error() != QKeychain::NoError && job->error() != QKeychain::EntryNotFound) {
            qCWarning(RUQOLA_PASSWORD_CORE_LOG) << "Keychain operation failed with error code" << static_cast<int>(job->error());
        }
        switch (job->error()) {
        case QKeychain::NoError:
            return Error::None;
        case QKeychain::EntryNotFound:
            return Error::NotFound;
        default:
            return Error::Unavailable;
        }
    }

    bool lock(const Callback &callback)
    {
        // Tests using the global instance must never touch the user's real wallet.
        if (!QStandardPaths::isTestModeEnabled()) {
            if (!mLock) {
                const QString directory = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + u"/ruqola"_s;
                if (QDir().mkpath(directory)) {
                    mLock = std::make_unique<QLockFile>(directory + u"/credentials.lock"_s);
                    mLock->setStaleLockTime(0);
                }
            }
            // Hold the lock for the lifetime of the cache, including migration and writes.
            if (mLock && (mLock->isLocked() || mLock->tryLock(0))) {
                return true;
            }
        }
        QTimer::singleShot(0, this, [callback] {
            callback({Error::Unavailable, {}});
        });
        return false;
    }
    std::unique_ptr<QLockFile> mLock;
};
}

AccountCredentialStore::Backend::~Backend() = default;
AccountCredentialStore::~AccountCredentialStore() = default;

AccountCredentialStore::AccountCredentialStore(std::unique_ptr<Backend> backend,
                                               const QStringList &legacyAccounts,
                                               QObject *parent,
                                               const QString &removalJournalPath)
    : QObject(parent)
    , mBackend(std::move(backend))
    , mLegacyAccounts(legacyAccounts)
    , mRemovalJournalPath(removalJournalPath)
{
    mLegacyAccounts.removeAll(QString());
    mLegacyAccounts.removeDuplicates();
    mJournalLoaded = loadPendingRemovals();
    if (!mJournalLoaded) {
        mState = State::Failed;
        mError = Error::Unavailable;
        QTimer::singleShot(0, this, &AccountCredentialStore::accessFailed);
    }
    // Also resume cleanup when no enabled account asks for a password on startup.
    QTimer::singleShot(0, this, &AccountCredentialStore::process);
}

AccountCredentialStore *AccountCredentialStore::self()
{
    static QPointer<AccountCredentialStore> instance;
    if (!instance) {
        QStringList accounts;
        QDirIterator it(ManagerDataPaths::self()->path(ManagerDataPaths::Config, {}),
                        {u"ruqola.conf"_s},
                        QDir::Files | QDir::NoSymLinks,
                        QDirIterator::Subdirectories);
        while (it.hasNext()) {
            const QSettings settings(it.next(), QSettings::IniFormat);
            accounts.append(settings.value("accountName"_L1).toString());
        }
        const QString journalPath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + u"/ruqola/credential-removals.ini"_s;
        instance = new AccountCredentialStore(std::make_unique<KeychainBackend>(), accounts, QCoreApplication::instance(), journalPath);
    }
    return instance;
}

QString AccountCredentialStore::field(Kind kind)
{
    return kind == Kind::Login ? u"login"_s : u"encryption"_s;
}

void AccountCredentialStore::read(const QString &account, Kind kind, QObject *context, Callback callback)
{
    enqueue({Operation::Read, account, kind, {}, context, std::move(callback), {}});
}

void AccountCredentialStore::write(const QString &account, Kind kind, const QString &password, QObject *context, Callback callback)
{
    enqueue({Operation::Write, account, kind, password, context, std::move(callback), {}});
}

bool AccountCredentialStore::remove(const QString &account, Kind kind, QObject *context, Callback callback)
{
    return enqueueRemoval({Operation::Remove, account, kind, {}, context, std::move(callback), {}});
}

bool AccountCredentialStore::removeAccount(const QString &account, QObject *context, Callback callback)
{
    return enqueueRemoval({Operation::RemoveAccount, account, Kind::Login, {}, context, std::move(callback), {}});
}

bool AccountCredentialStore::loadPendingRemovals()
{
    if (mRemovalJournalPath.isEmpty()) {
        return true;
    }
    QSettings journal(mRemovalJournalPath, QSettings::IniFormat);
    journal.beginGroup(u"removals"_s);
    QQueue<Request> requests;
    for (const auto &id : journal.childKeys()) {
        const auto record = journal.value(id).toStringList();
        if (record.size() != 2 || record.at(0).isEmpty() || (record.at(1) != "account"_L1 && record.at(1) != "login"_L1 && record.at(1) != "encryption"_L1)) {
            return false;
        }
        requests.enqueue({record.at(1) == "account"_L1 ? Operation::RemoveAccount : Operation::Remove,
                          record.at(0),
                          record.at(1) == "encryption"_L1 ? Kind::Encryption : Kind::Login,
                          {},
                          {},
                          {},
                          id});
    }
    if (journal.status() != QSettings::NoError) {
        return false;
    }
    // A removed account's config may already be gone before its legacy wallet
    // entries were migrated. Include its identity so those entries get cleaned up too.
    for (const auto &request : requests) {
        if (!mLegacyAccounts.contains(request.account)) {
            mLegacyAccounts.append(request.account);
        }
    }
    // All records are deletions, so their order is immaterial. Replay them before
    // new writes, including re-adding an account with the same name.
    requests.append(mRequests);
    mRequests = std::move(requests);
    return true;
}

bool AccountCredentialStore::enqueueRemoval(Request request)
{
    if (request.account.isEmpty()) {
        complete(request, {Error::NotFound, {}});
        return true;
    }
    if (!mJournalLoaded) {
        complete(request, {Error::Unavailable, {}});
        Q_EMIT accessFailed();
        return false;
    }
    if (!mRemovalJournalPath.isEmpty()) {
        QSettings journal(mRemovalJournalPath, QSettings::IniFormat);
        request.removalId = QUuid::createUuid().toString(QUuid::WithoutBraces);
        // Only identity and deletion scope are persisted here, never passwords.
        journal.setValue(u"removals/"_s + request.removalId,
                         QStringList{request.account, request.operation == Operation::RemoveAccount ? u"account"_s : field(request.kind)});
        journal.sync();
        if (journal.status() != QSettings::NoError) {
            complete(request, {Error::Unavailable, {}});
            qCWarning(RUQOLA_PASSWORD_CORE_LOG) << "Cannot persist credential removal request";
            Q_EMIT accessFailed();
            return false;
        }
    }
    enqueue(std::move(request));
    return true;
}

bool AccountCredentialStore::finishRemoval(const Request &request)
{
    if (request.removalId.isEmpty()) {
        return true;
    }
    QSettings journal(mRemovalJournalPath, QSettings::IniFormat);
    journal.remove(u"removals/"_s + request.removalId);
    journal.sync();
    // Block subsequent writes if this fails: a stale removal record must never
    // survive a new password for a re-created account and delete it on restart.
    return journal.status() == QSettings::NoError;
}

AccountCredentialStore::Result AccountCredentialStore::cachedResult(const Request &request) const
{
    const QString name = field(request.kind);
    auto value = mDocument.value("accounts"_L1).toObject().value(request.account).toObject().value(name);
    for (const auto &pending : mPendingChanges) {
        if (pending.account != request.account) {
            continue;
        }
        if (pending.operation == Operation::RemoveAccount || (pending.operation == Operation::Remove && pending.kind == request.kind)) {
            value = QJsonValue();
        } else if (pending.operation == Operation::Write && pending.kind == request.kind) {
            value = pending.password;
        }
    }
    return {value.isString() ? Error::None : Error::NotFound, value.toString().toUtf8()};
}

void AccountCredentialStore::enqueue(Request request)
{
    if (request.account.isEmpty()) {
        complete(request, {Error::NotFound, {}});
        return;
    }
    mRequests.enqueue(std::move(request));
    QTimer::singleShot(0, this, &AccountCredentialStore::process);
}

void AccountCredentialStore::complete(const Request &request, const Result &result)
{
    if (request.context && request.callback) {
        QTimer::singleShot(0, request.context, [callback = request.callback, result] {
            callback(result);
        });
    }
}

void AccountCredentialStore::retainChange(Request request)
{
    if (request.operation != Operation::Read) {
        // Preserve write/delete intent for an explicit retry, but complete each
        // caller only once. No pending passwords are written outside the wallet.
        request.callback = {};
        request.context = nullptr;
        mPendingChanges.enqueue(std::move(request));
    }
}

void AccountCredentialStore::retry()
{
    if (mState == State::Failed && !mBusy) {
        process();
        mRequests = std::exchange(mPendingChanges, {});
        mState = State::Unloaded;
        mError = Error::None;
        if (!mJournalLoaded) {
            mJournalLoaded = loadPendingRemovals();
            if (!mJournalLoaded) {
                fail(Error::Unavailable);
                return;
            }
        }
        Q_EMIT retryRequested();
        QTimer::singleShot(0, this, &AccountCredentialStore::process);
    }
}

void AccountCredentialStore::fail(Error error)
{
    mState = State::Failed;
    mError = error;
    mBusy = false;
    // Do not log payloads or backend error strings, which can contain secret data.
    qCWarning(RUQOLA_PASSWORD_CORE_LOG) << "Credential store access failed; automatic wallet access suspended";
    process();
    Q_EMIT accessFailed();
}

void AccountCredentialStore::ready()
{
    mCacheAvailable = true;
    mState = State::Ready;
    mBusy = false;
    process();
}

bool AccountCredentialStore::validDocument(const QJsonObject &document)
{
    if (document.value("version"_L1) != QJsonValue(1) || !document.value("accounts"_L1).isObject()) {
        return false;
    }
    const auto accounts = document.value("accounts"_L1).toObject();
    for (auto it = accounts.begin(); it != accounts.end(); ++it) {
        if (it.key().isEmpty() || !it.value().isObject()) {
            return false;
        }
        const auto credentials = it.value().toObject();
        for (const auto &name : {u"login"_s, u"encryption"_s}) {
            if (credentials.contains(name) && !credentials.value(name).isString()) {
                return false;
            }
        }
    }
    if (document.contains("legacyKeys"_L1)) {
        if (!document.value("legacyKeys"_L1).isArray()) {
            return false;
        }
        for (const auto &key : document.value("legacyKeys"_L1).toArray()) {
            if (!key.isString() || key.toString().isEmpty()) {
                return false;
            }
            const QString legacyKey = key.toString();
            const bool hasLogin = accounts.value(legacyKey).toObject().value("login"_L1).isString();
            const bool hasEncryption =
                legacyKey.endsWith("-encrypted"_L1) && accounts.value(legacyKey.chopped(10)).toObject().value("encryption"_L1).isString();
            // Cleanup may only delete entries whose credentials are in this document.
            if (!hasLogin && !hasEncryption) {
                return false;
            }
        }
    }
    return true;
}

void AccountCredentialStore::load()
{
    mBusy = true;
    qCInfo(RUQOLA_PASSWORD_CORE_LOG) << "Reading consolidated credential entry";
    mBackend->read(service, entry, [this](const Result &result) {
        if (result.error == Error::NotFound) {
            mCacheAvailable = false;
            mDocument = {{u"version"_s, 1}, {u"accounts"_s, QJsonObject()}};
            qCInfo(RUQOLA_PASSWORD_CORE_LOG) << "Starting credential migration for" << mLegacyAccounts.size() << "accounts";
            mMigrationIndex = 0;
            mCleanupKeys.clear();
            migrateNext();
        } else if (result.error != Error::None) {
            fail(result.error);
        } else {
            const auto document = QJsonDocument::fromJson(result.data);
            if (!document.isObject() || !validDocument(document.object())) {
                fail(Error::InvalidData);
                return;
            }
            mDocument = document.object();
            mCleanupKeys.clear();
            for (const auto &key : mDocument.value("legacyKeys"_L1).toArray()) {
                mCleanupKeys.append(key.toString());
            }
            cleanupNext();
        }
    });
}

void AccountCredentialStore::migrateNext()
{
    if (mMigrationIndex == mLegacyAccounts.size() * 2) {
        saveMigration();
        return;
    }
    const QString account = mLegacyAccounts.at(mMigrationIndex / 2);
    const Kind kind = mMigrationIndex % 2 == 0 ? Kind::Login : Kind::Encryption;
    const QString key = account + (kind == Kind::Encryption ? u"-encrypted"_s : QString());
    ++mMigrationIndex;
    qCInfo(RUQOLA_PASSWORD_CORE_LOG) << "Reading legacy credential" << mMigrationIndex << "of" << mLegacyAccounts.size() * 2;
    mBackend->read(legacyService, key, [this, account, kind, key](const Result &result) {
        if (result.error != Error::None && result.error != Error::NotFound) {
            // Never turn cancellation or a backend failure into a missing password.
            fail(result.error);
            return;
        }
        if (result.error == Error::None) {
            auto accounts = mDocument.value("accounts"_L1).toObject();
            auto credentials = accounts.value(account).toObject();
            credentials.insert(field(kind), QString::fromUtf8(result.data));
            accounts.insert(account, credentials);
            mDocument.insert(u"accounts"_s, accounts);
            if (!mCleanupKeys.contains(key)) {
                mCleanupKeys.append(key);
            }
        }
        migrateNext();
    });
}

void AccountCredentialStore::saveMigration()
{
    qCInfo(RUQOLA_PASSWORD_CORE_LOG) << "Saving consolidated credentials before legacy cleanup";
    mDocument.insert(u"legacyKeys"_s, QJsonArray::fromStringList(mCleanupKeys));
    const QByteArray data = QJsonDocument(mDocument).toJson(QJsonDocument::Compact);
    mBackend->write(service, entry, data, [this, data](const Result &result) {
        if (result.error != Error::None) {
            fail(result.error);
            return;
        }
        qCInfo(RUQOLA_PASSWORD_CORE_LOG) << "Verifying consolidated credentials";
        mBackend->read(service, entry, [this, data](const Result &verification) {
            if (verification.error != Error::None || verification.data != data) {
                fail(verification.error == Error::None ? Error::InvalidData : verification.error);
                return;
            }
            cleanupNext();
        });
    });
}

void AccountCredentialStore::cleanupNext()
{
    if (mCleanupKeys.isEmpty()) {
        if (!mDocument.contains("legacyKeys"_L1)) {
            ready();
            return;
        }
        auto document = mDocument;
        document.remove("legacyKeys"_L1);
        mBackend->write(service, entry, QJsonDocument(document).toJson(QJsonDocument::Compact), [this, document](const Result &result) {
            if (result.error != Error::None) {
                fail(result.error);
                return;
            }
            mDocument = document;
            qCInfo(RUQOLA_PASSWORD_CORE_LOG) << "Credential migration and cleanup complete";
            ready();
        });
        return;
    }
    qCInfo(RUQOLA_PASSWORD_CORE_LOG) << "Removing migrated legacy credential; remaining:" << mCleanupKeys.size();
    const QString key = mCleanupKeys.takeFirst();
    mBackend->remove(legacyService, key, [this](const Result &result) {
        if (result.error != Error::None && result.error != Error::NotFound) {
            fail(result.error);
            return;
        }
        // The persisted cleanup list makes interrupted deletion safe to resume.
        cleanupNext();
    });
}

void AccountCredentialStore::process()
{
    if (mBusy || mRequests.isEmpty()) {
        return;
    }
    if (mState == State::Unloaded) {
        load();
        return;
    }
    if (mState == State::Failed) {
        const auto requests = std::exchange(mRequests, {});
        for (const auto &request : requests) {
            if (request.operation == Operation::Read && mCacheAvailable) {
                complete(request, cachedResult(request));
            } else {
                complete(request, {mError, {}});
                retainChange(request);
            }
        }
        return;
    }
    const Request request = mRequests.dequeue();
    auto accounts = mDocument.value("accounts"_L1).toObject();
    auto credentials = accounts.value(request.account).toObject();
    const QString name = field(request.kind);
    if (request.operation == Operation::Read) {
        complete(request, cachedResult(request));
        QTimer::singleShot(0, this, &AccountCredentialStore::process);
        return;
    }
    if (request.operation == Operation::Write) {
        credentials.insert(name, request.password);
    } else {
        credentials.remove(name);
    }
    if (request.operation == Operation::RemoveAccount || credentials.isEmpty()) {
        accounts.remove(request.account);
    } else {
        accounts.insert(request.account, credentials);
    }
    auto document = mDocument;
    document.insert(u"accounts"_s, accounts);
    if (document == mDocument) {
        if (!finishRemoval(request)) {
            complete(request, {Error::Unavailable, {}});
            retainChange(request);
            fail(Error::Unavailable);
            return;
        }
        complete(request, {});
        QTimer::singleShot(0, this, &AccountCredentialStore::process);
        return;
    }
    mBusy = true;
    mBackend->write(service, entry, QJsonDocument(document).toJson(QJsonDocument::Compact), [this, document, request](const Result &result) {
        if (result.error != Error::None) {
            complete(request, result);
            retainChange(request);
            fail(result.error);
            return;
        }
        mDocument = document;
        if (!finishRemoval(request)) {
            complete(request, {Error::Unavailable, {}});
            retainChange(request);
            fail(Error::Unavailable);
            return;
        }
        complete(request, result);
        ready();
    });
}

#include "moc_accountcredentialstore.cpp"
