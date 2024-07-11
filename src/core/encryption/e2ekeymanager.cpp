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

QString E2eKeyManager::generateRandomPassword() const
{
    QString randomStr;
    for (int i = 0; i < 15; i++) {
        const int d = rand() % 200; // Generate a random ASCII value between 0 and 199
        if (d >= 33 && d <= 123) {
            randomStr.append(QLatin1Char(static_cast<char>(d))); // Convert the ASCII value to a character for valid range
        } else {
            randomStr.append(QString::number(d % 10)); // Keep the last digit for numbers outside the valid range
        }
    }
    return randomStr;
}

void E2eKeyManager::fetchMyKeys()
{
    auto job = new RocketChatRestApi::FetchMyKeysJob(this);
    mAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::FetchMyKeysJob::fetchMyKeysDone, this, [this](const QJsonObject &json) {
        qDebug() << " RocketChatRestApi::FetchMyKeysJob result " << json << "account name: " << mAccount->accountName();
        Q_EMIT fetchMyKeysDone(json);
    });
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
