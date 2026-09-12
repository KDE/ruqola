/*
   SPDX-FileCopyrightText: 2026 Ruqola contributors
   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolacore_export.h"
#include <QJsonObject>
#include <QObject>
#include <QPointer>
#include <QQueue>
#include <functional>
#include <memory>

/** One cached, versioned wallet entry for login and E2E passwords.
 * All completions are asynchronous and guarded by the caller's context object.
 * A failed wallet operation suspends backend access until retry() is requested;
 * an already loaded cache remains readable, including pending changes.
 */
class LIBRUQOLACORE_EXPORT AccountCredentialStore : public QObject
{
    Q_OBJECT
public:
    enum class Kind {
        Login,
        Encryption
    };
    enum class Error {
        None,
        NotFound,
        Unavailable,
        InvalidData
    };
    struct Result {
        Error error = Error::None;
        QByteArray data;
    };
    using Callback = std::function<void(const Result &)>;

    // Implementations must complete asynchronously and cancel callbacks on destruction.
    class LIBRUQOLACORE_EXPORT Backend
    {
    public:
        virtual ~Backend();
        virtual void read(const QString &service, const QString &key, Callback callback) = 0;
        virtual void write(const QString &service, const QString &key, const QByteArray &data, Callback callback) = 0;
        virtual void remove(const QString &service, const QString &key, Callback callback) = 0;
    };

    explicit AccountCredentialStore(std::unique_ptr<Backend> backend,
                                    const QStringList &legacyAccounts,
                                    QObject *parent = nullptr,
                                    const QString &removalJournalPath = {});
    ~AccountCredentialStore() override;
    static AccountCredentialStore *self();

    void read(const QString &account, Kind kind, QObject *context, Callback callback);
    void write(const QString &account, Kind kind, const QString &password, QObject *context, Callback callback = {});
    // False means deletion intent could not be saved; callers must retain account settings.
    bool remove(const QString &account, Kind kind, QObject *context, Callback callback = {});
    bool removeAccount(const QString &account, QObject *context, Callback callback = {});
    void retry();

Q_SIGNALS:
    void accessFailed();
    void retryRequested();

private:
    enum class State {
        Unloaded,
        Ready,
        Failed
    };
    enum class Operation {
        Read,
        Write,
        Remove,
        RemoveAccount
    };
    struct Request {
        Operation operation;
        QString account;
        Kind kind;
        QString password;
        QPointer<QObject> context;
        Callback callback;
        QString removalId;
    };
    void enqueue(Request request);
    void process();
    void load();
    void migrateNext();
    void saveMigration();
    void cleanupNext();
    void fail(Error error);
    void ready();
    void complete(const Request &request, const Result &result);
    void retainChange(Request request);
    Result cachedResult(const Request &request) const;
    bool enqueueRemoval(Request request);
    bool loadPendingRemovals();
    bool finishRemoval(const Request &request);
    static QString field(Kind kind);
    static bool validDocument(const QJsonObject &document);

    std::unique_ptr<Backend> mBackend;
    QStringList mLegacyAccounts;
    const QString mRemovalJournalPath;
    QQueue<Request> mRequests;
    QQueue<Request> mPendingChanges;
    QJsonObject mDocument;
    QStringList mCleanupKeys;
    qsizetype mMigrationIndex = 0;
    State mState = State::Unloaded;
    Error mError = Error::None;
    bool mBusy = false;
    bool mCacheAvailable = false;
    bool mJournalLoaded = false;
};
