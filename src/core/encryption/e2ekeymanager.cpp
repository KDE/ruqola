/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2ekeymanager.h"
#include "connection.h"
#include "e2e/fetchmykeysjob.h"
#include "rocketchataccount.h"
#include "ruqola_encryption_debug.h"
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
    const int numberChar = 30;
    QString randomStr;
    for (int i = 0; i < numberChar; i++) {
        const int d = rand() % 200; // Generate a random ASCII value between 0 and 199
        if (d >= 33 && d <= 123) {
            randomStr.append(QLatin1Char(static_cast<char>(d))); // Convert the ASCII value to a character for valid range
        } else {
            randomStr.append(QString::number(d % 10)); // Keep the last digit for numbers outside the valid range
        }
    }
    return randomStr;
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

E2eKeyManager::Status E2eKeyManager::needToDecodeEncryptionKey() const
{
    if (!mAccount) {
        return Status::Unknown;
    }
    if (mAccount->encryptionEnabled()) {
        // TODO check if we have decoded key stored.
        // TODO check NeedToDecryptKey
        return Status::NeedToGenerateKey;
    }
    return mStatus;
}

#include "moc_e2ekeymanager.cpp"
