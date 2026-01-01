/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "encryption/encryptionutils.h"
#include <QWidget>

class QTextEdit;
class EncryptionTestGui : public QWidget
{
    Q_OBJECT
public:
    explicit EncryptionTestGui(QWidget *parent = nullptr);
    ~EncryptionTestGui() override = default;

private:
    QTextEdit *const mTextEdit;
    QTextEdit *const mTextEditResult;
    QByteArray mMasterKey;
    QString mPassword;
    QString mSalt;
    QByteArray mEncryptedPrivateKey;
    QByteArray mDecryptedPrivateKey;
    EncryptionUtils::RSAKeyPair mRsaKeyPair;
    QByteArray mSessionKey;
    QByteArray mEncryptedSessionKey;
    QByteArray mDecryptedSessionKey;
    QByteArray mEncryptedMessage;
    QByteArray mDecryptedMessage;
};
