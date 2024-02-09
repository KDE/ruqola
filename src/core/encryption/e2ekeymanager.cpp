/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2ekeymanager.h"
#include "connection.h"
#include "e2e/fetchmykeysjob.h"
#include "rocketchataccount.h"
#include "ruqola_encryption_debug.h"

E2eKeyManager::E2eKeyManager(RocketChatAccount *account, QObject *parent)
    : QObject{parent}
    , mAccount(account)
{
}

E2eKeyManager::~E2eKeyManager() = default;

void E2eKeyManager::decodeEncryptionKey()
{
    // TODO
}

void E2eKeyManager::fetchMyKeys()
{
    auto job = new RocketChatRestApi::FetchMyKeysJob(this);
    mAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::FetchMyKeysJob::fetchMyKeysDone, this, &E2eKeyManager::fetchMyKeysDone);
    if (!job->start()) {
        qCDebug(RUQOLA_ENCRYPTION_LOG) << "Impossible to start fetchmykeys job";
    }
}

bool E2eKeyManager::needToDecodeEncryptionKey() const
{
    if (!mAccount) {
        return false;
    }
    if (mAccount->encryptionEnabled()) {
        // TODO check if we have decoded key stored.
        return true;
    }
    return false;
}

#include "moc_e2ekeymanager.cpp"
