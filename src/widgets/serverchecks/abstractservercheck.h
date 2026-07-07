/*
   SPDX-FileCopyrightText: 2026 David Faure <david.faure@kdab.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "messages/message.h"
#include <QByteArray>
#include <QObject>
#include <QPointer>

class RocketChatAccount;
class MessagesModel;
class QTimer;

/**
 * Base class for a single "does the server behave as expected" check.
 *
 * A check sends something to a room (message, edit, file…) and then observes
 * how it arrives in the model, to decide pass/fail. Each check emits finished()
 * exactly once. A watchdog timer turns a check that never completes into a
 * failure instead of hanging the checklist.
 */
class LIBRUQOLAWIDGETS_TESTS_EXPORT AbstractServerCheck : public QObject
{
    Q_OBJECT
public:
    explicit AbstractServerCheck(RocketChatAccount *account, const QByteArray &roomId, QObject *parent = nullptr);
    ~AbstractServerCheck() override;

    /** Human-readable name shown in the checklist. */
    [[nodiscard]] virtual QString name() const = 0;

    /** Runs the check. Emits finished() when done (or on timeout). */
    void start();

Q_SIGNALS:
    void finished(bool passed, const QString &details);

protected:
    /** Actually start sending/observing. Called by start() after the watchdog is armed. */
    virtual void doStart() = 0;

    /** Watchdog timeout in ms. Override for slow async operations (search, link preview). */
    [[nodiscard]] virtual int watchdogTimeoutMs() const;

    /** Called whenever the room's message model changes (rows inserted or data changed). */
    virtual void checkState() = 0;

    void connectModel();
    void reportPassed();
    void reportFailed(const QString &details);

    [[nodiscard]] bool isFinished() const;
    [[nodiscard]] MessagesModel *messageModel() const;

    /** Finds a server-confirmed (non-pending) message whose text matches, empty messageId if none. */
    [[nodiscard]] Message findConfirmedMessageByText(const QString &text) const;

    /** Best-effort cleanup of a test message we created (not part of pass/fail). */
    void deleteTestMessage(const QByteArray &messageId);

    /** Unique marker text so we can recognize our own test messages. */
    [[nodiscard]] static QString generateMarker();

    QPointer<RocketChatAccount> mAccount;
    const QByteArray mRoomId;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTimeout();
    QTimer *const mWatchdog;
    bool mFinished = false;
};
