/*
   SPDX-FileCopyrightText: 2026 David Faure <david.faure@kdab.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QByteArray>
#include <QList>
#include <QObject>
#include <QPointer>

class RocketChatAccount;
class AbstractServerCheck;

/**
 * Runs the server checks one after another in a given room and reports each
 * result. Checks run sequentially so their test messages don't interfere.
 */
class LIBRUQOLAWIDGETS_TESTS_EXPORT ServerCheckListManager : public QObject
{
    Q_OBJECT
public:
    explicit ServerCheckListManager(RocketChatAccount *account, const QByteArray &roomId, QObject *parent = nullptr);
    ~ServerCheckListManager() override;

    [[nodiscard]] int checkCount() const;
    [[nodiscard]] QString checkName(int index) const;

    /** Starts running all checks from the first one. */
    void start();

Q_SIGNALS:
    void checkStarted(int index);
    void checkFinished(int index, bool passed, const QString &details);
    void allFinished();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void runNext();

    QList<AbstractServerCheck *> mChecks;
    QPointer<RocketChatAccount> mAccount;
    const QByteArray mRoomId;
    int mCurrentIndex = -1;
};
