/*
  SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>
  SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>
  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "encryptionutils.h"
#include "ruqola_encryption_debug.h"
#include <QByteArray>
// https://docs.rocket.chat/customer-center/security-center/end-to-end-encryption-specifications

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRandomGenerator>

using namespace Qt::Literals::StringLiterals;
QByteArray EncryptionUtils::exportJWKKey(RSA *rsaKey)
{
#if 0
    code javascript
    const key = await crypto.subtle.generateKey(
      { name: 'AES-CBC', length: 256 },
      true,
      ['encrypt', 'decrypt']
    );

    const jwkKey = await exportJWKKey(key);
    console.log(jwkKey);

#endif

    const BIGNUM *n, *e, *d;
    RSA_get0_key(rsaKey, &n, &e, &d);

    if (!n || !e) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << " Impossible to get RSA";
        return {};
    }

    // Convert BIGNUM to base64url strings
    QByteArray nBytes(BN_num_bytes(n), 0);
    BN_bn2bin(n, reinterpret_cast<unsigned char *>(nBytes.data()));
    const QString nBase64Url = QString::fromLatin1(nBytes.toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals));

    QByteArray eBytes(BN_num_bytes(e), 0);
    BN_bn2bin(e, reinterpret_cast<unsigned char *>(eBytes.data()));
    const QString eBase64Url = QString::fromLatin1(eBytes.toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals));

    QJsonObject jwkObj;
    jwkObj["kty"_L1] = "RSA"_L1;
    jwkObj["n"_L1] = nBase64Url;
    jwkObj["e"_L1] = eBase64Url;

    QJsonDocument doc(jwkObj);
    return doc.toJson(QJsonDocument::Compact);
}

void EncryptionUtils::generateRSAKey()
{
    int ret = 0;
    RSA *rsa = nullptr;
    BIGNUM *bne = nullptr;
    BIO *bp_public = nullptr;
    BIO *bp_private = nullptr;

    int bits = 2048;
    unsigned long e = RSA_F4; // équivalent à 0x10001

    bne = BN_new();
    ret = BN_set_word(bne, e);
    if (ret != 1) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Error when generating exponent";
        return;
    }

    rsa = RSA_new();
    ret = RSA_generate_key_ex(rsa, bits, bne, nullptr);
    if (ret != 1) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Error during generate key";
        return;
    }

    bp_public = BIO_new_file("public_key.pem", "w+");
    ret = PEM_write_bio_RSAPublicKey(bp_public, rsa);
    if (ret != 1) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Error when saving public key";
        return;
    }

    bp_private = BIO_new_file("private_key.pem", "w+");
    ret = PEM_write_bio_RSAPrivateKey(bp_private, rsa, nullptr, nullptr, 0, nullptr, nullptr);
    if (ret != 1) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Error when saving private key";
        return;
    }

    // Libérer la mémoire
    BIO_free_all(bp_public);
    BIO_free_all(bp_private);
    RSA_free(rsa);
    BN_free(bne);
}

QString EncryptionUtils::encodePrivateKey(const QString &privateKey, const QString &password, const QString &userId)
{
    const QByteArray masterKey = getMasterKey(password, userId);
    return {};
}

QByteArray EncryptionUtils::getMasterKey(const QString &password, const QString &userId)
{
    if (password.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Password can't be null. It's a bug";
        return {};
    }

    if (userId.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "UserId can't be null. It's a bug";
        return {};
    }

    const QByteArray baseKey = importRawKey(password.toUtf8(), userId.toUtf8(), 1000);
    if (baseKey.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Failed to derive base key from password!";
        return {};
    }

    const QByteArray masterKey = deriveKey(userId.toUtf8(), baseKey, 1000, 32);
    if (masterKey.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Master key derivation failed!";
        return {};
    }

    return masterKey;

#if 0
    async getMasterKey(password: string): Promise<void | CryptoKey> {
            if (password == null) {
                    alert('You should provide a password');
            }

            // First, create a PBKDF2 "key" containing the password
            let baseKey;
            try {
                    baseKey = await importRawKey(toArrayBuffer(password));
            } catch (error) {
                    this.setState(E2EEState.ERROR);
                    return this.error('Error creating a key based on user password: ', error);
            }

            // Derive a key from the password
            try {
                    return await deriveKey(toArrayBuffer(Meteor.userId()), baseKey);
            } catch (error) {
                    this.setState(E2EEState.ERROR);
                    return this.error('Error deriving baseKey: ', error);
            }
    }
    // TODO
    return {};
#endif
}

QByteArray EncryptionUtils::encryptAES_CBC(const QByteArray &data, const QByteArray &key, const QByteArray &iv)
{
    EVP_CIPHER_CTX *ctx;
    int len;
    int ciphertext_len;
    unsigned char ciphertext[128];

    if (!(ctx = EVP_CIPHER_CTX_new()))
        return {};

    if (1
        != EVP_EncryptInit_ex(ctx,
                              EVP_aes_256_cbc(),
                              NULL,
                              reinterpret_cast<const unsigned char *>(key.data()),
                              reinterpret_cast<const unsigned char *>(iv.data())))
        return {};

    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, reinterpret_cast<const unsigned char *>(data.data()), data.size()))
        return {};
    ciphertext_len = len;

    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        return {};
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return QByteArray(reinterpret_cast<char *>(ciphertext), ciphertext_len);
}

QByteArray EncryptionUtils::generateRandomIV(int size)
{
    QByteArray iv(size, 0);

    if (RAND_bytes(reinterpret_cast<unsigned char *>(iv.data()), size) != 1) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Failed to generate random IV using OpenSSL!";
        return {};
    }

    return iv;
}

QString EncryptionUtils::generateRandomText(int length)
{
    const static QString characters = QStringLiteral("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()-_=+[]{}|;:,.<>?");
    QString randomText;
    const int charSize = characters.size();

    for (int i = 0; i < length; ++i) {
        const int index = QRandomGenerator::global()->bounded(charSize);
        randomText.append(characters.at(index));
    }

    return randomText;
}

QByteArray EncryptionUtils::deriveKey(const QByteArray &salt, const QByteArray &baseKey, int iterations, int keyLength)
{
    QByteArray derivedKey(keyLength, 0); // Allocate memory for the derived key

    // Use OpenSSL's PKCS5_PBKDF2_HMAC for PBKDF2 key derivation
    const int result = PKCS5_PBKDF2_HMAC(baseKey.data(),
                                         baseKey.size(), // Input key (password)
                                         reinterpret_cast<const unsigned char *>(salt.data()),
                                         salt.size(), // Salt
                                         iterations, // Number of iterations
                                         EVP_sha256(), // Hash function (SHA-256)
                                         keyLength, // Output key length (in bytes)
                                         reinterpret_cast<unsigned char *>(derivedKey.data()) // Output buffer for the key
    );

    if (result != 1) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Key derivation failed!";
        return QByteArray();
    }

    return derivedKey;
}

#if 0
QByteArray aesEncrypt(const QByteArray& plaintext, const QByteArray& key, const QByteArray& iv) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    int len;
    QByteArray ciphertext(plaintext.size() + AES_BLOCK_SIZE, 0);  // Ciphertext buffer

    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, reinterpret_cast<const unsigned char*>(key.data()), reinterpret_cast<const unsigned char*>(iv.data()))) {
        qWarning() << "Encryption init failed";
        return QByteArray();
    }

    if (1 != EVP_EncryptUpdate(ctx, reinterpret_cast<unsigned char*>(ciphertext.data()), &len, reinterpret_cast<const unsigned char*>(plaintext.data()), plaintext.size())) {
        qWarning() << "Encryption update failed";
        return QByteArray();
    }

    int ciphertext_len = len;

    if (1 != EVP_EncryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(ciphertext.data()) + len, &len)) {
        qWarning() << "Encryption final failed";
        return QByteArray();
    }

    ciphertext_len += len;
    ciphertext.resize(ciphertext_len);

    EVP_CIPHER_CTX_free(ctx);
    return ciphertext;
}

QByteArray aesDecrypt(const QByteArray& ciphertext, const QByteArray& key, const QByteArray& iv) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    int len;
    QByteArray plaintext(ciphertext.size(), 0);  // Plaintext buffer

    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, reinterpret_cast<const unsigned char*>(key.data()), reinterpret_cast<const unsigned char*>(iv.data()))) {
        qWarning() << "Decryption init failed";
        return QByteArray();
    }

    if (1 != EVP_DecryptUpdate(ctx, reinterpret_cast<unsigned char*>(plaintext.data()), &len, reinterpret_cast<const unsigned char*>(ciphertext.data()), ciphertext.size())) {
        qWarning() << "Decryption update failed";
        return QByteArray();
    }

    int plaintext_len = len;

    if (1 != EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(plaintext.data()) + len, &len)) {
        qWarning() << "Decryption final failed";
        return QByteArray();
    }

    plaintext_len += len;
    plaintext.resize(plaintext_len);

    EVP_CIPHER_CTX_free(ctx);
    return plaintext;
}

/// TEST
void aesExample() {
QByteArray key = deriveKey("mysalt", "mypassword", 1000, 32);  // Derive a key
QByteArray iv = QByteArray::fromHex("00112233445566778899aabbccddeeff");  // Example IV (16 bytes for AES)

QByteArray plaintext = "Hello, AES CBC Encryption!";

QByteArray ciphertext = aesEncrypt(plaintext, key, iv);
qDebug() << "Ciphertext:" << ciphertext.toHex();

QByteArray decryptedText = aesDecrypt(ciphertext, key, iv);
qDebug() << "Decrypted Text:" << decryptedText;
}

#endif

EncryptionUtils::EncryptionInfo EncryptionUtils::splitVectorAndEcryptedData(const QByteArray &cipherText)
{
    EncryptionUtils::EncryptionInfo info;
    if (!cipherText.isEmpty()) {
        // TODO add more check
        info.vector = cipherText.left(16);
        info.encryptedData = cipherText.last(16);
    }
    return info;
}

QByteArray EncryptionUtils::joinVectorAndEcryptedData(const EncryptionUtils::EncryptionInfo &info)
{
    return info.vector + info.encryptedData;
}

QVector<uint8_t> EncryptionUtils::toArrayBuffer(const QByteArray &ba)
{
    const QVector<uint8_t> byteVector(ba.constBegin(), ba.constEnd());
    return byteVector;
}

// return crypto.subtle.importKey(
//         'jwk',
//         keyData,
//         {
//                 name: 'RSA-OAEP',
//                 modulusLength: 2048,
//                 publicExponent: new Uint8Array([0x01, 0x00, 0x01]),
//                 hash: { name: 'SHA-256' },
//         },
//         true,
//         keyUsages,
// );
void EncryptionUtils::importRSAKey()
{
    // TODO
}

// return crypto.subtle.importKey('jwk', keyData, { name: 'AES-CBC' }, true, keyUsages);
void EncryptionUtils::importAESKey()
{
#if 0
    export async function importAESKey(keyData, keyUsages = ['encrypt', 'decrypt']) {
            return crypto.subtle.importKey('jwk', keyData, { name: 'AES-CBC' }, true, keyUsages);
    }

#endif

    // TODO
}

// crypto.subtle.importKey('raw', keyData, { name: 'PBKDF2' }, false, keyUsages);
QByteArray EncryptionUtils::importRawKey(const QByteArray &keyData, const QByteArray &salt, int iterations)
{
#if 0
    export async function importRawKey(keyData, keyUsages = ['deriveKey']) {
            return crypto.subtle.importKey('raw', keyData, { name: 'PBKDF2' }, false, keyUsages);
    }
#endif

#if 0
    QByteArray iv = generateRandomIV(16);
    QByteArray data = generateRandomText(16).toUtf8();
    QByteArray cipherText = encryptAES_CBC(data, key, iv);
#endif

    const QByteArray baseKey = deriveKey(keyData, salt, iterations);

    return baseKey;
}

bool EncryptionUtils::EncryptionInfo::isValid() const
{
    return !vector.isEmpty() && !encryptedData.isEmpty();
}

bool EncryptionUtils::EncryptionInfo::operator==(const EncryptionUtils::EncryptionInfo &other) const
{
    return other.vector == vector && other.encryptedData == encryptedData;
}

QString EncryptionUtils::generateRandomPassword()
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
