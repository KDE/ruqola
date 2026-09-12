/*
   SPDX-FileCopyrightText: 2026 Ruqola contributors
   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include <QObject>

class AccountCredentialStoreTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void shouldReadOnceForAllAccounts();
    void shouldReadCachedCredentialsAfterWriteFailure();
    void shouldResumePersistedRemovals_data();
    void shouldResumePersistedRemovals();
    void shouldRemoveLegacyCredentialsAfterAccountConfigIsGone();
    void shouldReplayRemovalBeforeReaddingAccount();
    void shouldBlockRecreationUntilRemovalJournalClears();
    void shouldKeepAccountWhenRemovalCannotBeJournaled();
    void shouldReplayFailedRemovalsOnRetry();
    void shouldResumeAfterFinalMigrationWriteFails();
    void shouldNotAnnounceUnchangedPasswordsOnRetry();
    void shouldMigrateAndCleanLegacyEntries();
    void shouldStopAfterCancellationAndRetryExplicitly();
    void shouldPreserveLegacyOnMigrationFailure_data();
    void shouldPreserveLegacyOnMigrationFailure();
    void shouldResumeInterruptedCleanup();
    void shouldRejectInvalidData_data();
    void shouldRejectInvalidData();
    void shouldSerializeUpdatesAndRemoveCredentials();
    void shouldPreserveDataAfterFailedWrite();
    void shouldGuardCallbacksAndPersistAfterCallerDestruction();
    void shouldNotOverwriteNewPasswordWithPendingRead();
    void shouldLoadPasswordWhenAccountEnabled();
    void shouldReloadSettingsOnExplicitRetry();
    void shouldCompleteEncryptionReadOnFailure();
    void shouldIgnoreEncryptionReadAfterReset();
    void shouldRemoveBothPasswordsAfterSettingsDestruction();
};
