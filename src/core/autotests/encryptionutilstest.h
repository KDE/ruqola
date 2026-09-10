/*
  SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class EncryptionUtilsTest : public QObject
{
    Q_OBJECT
public:
    explicit EncryptionUtilsTest(QObject *parent = nullptr);
    ~EncryptionUtilsTest() override = default;

private Q_SLOTS:
    void shouldExportJWKKey();
    void shouldRoundTripGeneratedKeyPairAsRocketChatDoes();
    void shouldExportSessionKeyJwkForBothAesFlavours();
    void shouldDeriveMasterKeyTheWayRocketChatDoes();

    void shouldRejectWrongKeyAndIvSizes();
    void shouldGenerateRandomPassword();
    void shouldGenerateRoomKeyId();
};
