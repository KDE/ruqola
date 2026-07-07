/*
   SPDX-FileCopyrightText: 2026 David Faure <david.faure@kdab.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QByteArray>
#include <QDialog>
#include <QList>
#include <QPointer>

class RocketChatAccount;
class ServerCheckListManager;
class QTreeWidget;
class QTreeWidgetItem;
class QPushButton;

/**
 * Dialog that runs a checklist of "does the server behave as expected" tests
 * (useful right after a server upgrade) and shows a pass/fail per check.
 *
 * The checks send temporary test messages to @p roomId and clean up after
 * themselves, so pick a scratch channel before running.
 */
class LIBRUQOLAWIDGETS_TESTS_EXPORT ServerCheckListDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ServerCheckListDialog(RocketChatAccount *account, const QByteArray &roomId, QWidget *parent = nullptr);
    ~ServerCheckListDialog() override;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void populateChecks();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRunChecks();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotCheckStarted(int index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotCheckFinished(int index, bool passed, const QString &details);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotAllFinished();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotPostResults();
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString buildResultMessage() const;
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();

    ServerCheckListManager *const mManager;
    QTreeWidget *const mTreeWidget;
    QPushButton *mRunButton = nullptr;
    QPushButton *mPostButton = nullptr;
    QList<QTreeWidgetItem *> mItems;
    QList<bool> mResults;
    QPointer<RocketChatAccount> mAccount;
    const QByteArray mRoomId;
};
