/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"

#include <QObject>
class RocketChatAccount;
class LIBRUQOLACORE_EXPORT E2eKeyManager : public QObject
{
    Q_OBJECT
public:
    enum Status {
        Unknown = 0,
        NeedToDecryptKey,
        NeedToGenerateKey,
    };
    Q_ENUM(Status)
    explicit E2eKeyManager(RocketChatAccount *account, QObject *parent = nullptr);
    ~E2eKeyManager() override;

    void decodeEncryptionKey();

    void fetchMyKeys();

    [[nodiscard]] E2eKeyManager::Status needToDecodeEncryptionKey() const;

    [[nodiscard]] QString generateRandomPassword() const;

    [[nodiscard]] Status status() const;
    void setStatus(Status newStatus);

Q_SIGNALS:
    void needDecodeEncryptionKey();
    void failedDecodeEncryptionKey();
    void fetchMyKeysDone(const QJsonObject &replyObject);

private:
    LIBRUQOLACORE_NO_EXPORT void verifyExistingKey(const QJsonObject &json);
    Status mStatus = Status::Unknown;
    RocketChatAccount *const mAccount;
};
