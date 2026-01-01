/*
  SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2ekeymanager.h"
#include "config-ruqola.h"
#include "connection.h"
#include "e2e/fetchmykeysjob.h"
#if USE_E2E_SUPPORT
#include "encryptionutils.h"
#endif
#include "rocketchataccount.h"
#include "rocketchataccountsettings.h"
#include "ruqola_encryption_debug.h"
#include "ruqolaserverconfig.h"

// https://docs.rocket.chat/docs/end-to-end-encryption-specifications
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

QString E2eKeyManager::generateRandomPassword() const
{
#if USE_E2E_SUPPORT
    return EncryptionUtils::generateRandomPassword();
#else
    return {};
#endif
}

E2eKeyManager::Status E2eKeyManager::status() const
{
    return mStatus;
}

void E2eKeyManager::setStatus(Status newStatus)
{
    mStatus = newStatus;
}

void E2eKeyManager::fetchMyKeys()
{
    auto job = new RocketChatRestApi::FetchMyKeysJob(this);
    mAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::FetchMyKeysJob::fetchMyKeysDone, this, [this](const QJsonObject &json) {
        qDebug() << " RocketChatRestApi::FetchMyKeysJob result " << json << "account name: " << mAccount->accountName();
        verifyExistingKey(json);
        Q_EMIT verifyKeyDone();
    });
    if (!job->start()) {
        qCDebug(RUQOLA_ENCRYPTION_LOG) << "Impossible to start fetchmykeys job";
    }
}

void E2eKeyManager::verifyExistingKey(const QJsonObject &json)
{
    // TODO
    // return status value
}

bool E2eKeyManager::keySaved() const
{
    if (mAccount) {
        return mAccount->settings()->keySaved();
    }
    return false;
}

void E2eKeyManager::setKeySaved(bool newKeySaved)
{
    if (mAccount) {
        mAccount->settings()->setKeySaved(newKeySaved);
    }
}

E2eKeyManager::Status E2eKeyManager::needToDecodeEncryptionKey() const
{
    if (!mAccount) {
        return Status::Unknown;
    }
    if (mAccount->ruqolaServerConfig()->encryptionEnabled()) {
        // TODO check if we have decoded key stored.
        // TODO check NeedToDecryptKey
        return Status::NeedToGenerateKey;
    }
    return mStatus;
}

#include "moc_e2ekeymanager.cpp"
