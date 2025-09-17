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
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRandomGenerator>

using namespace Qt::Literals::StringLiterals;

/**
 * @brief Exports an RSA public key in JWK (JSON Web Key) format.
 *
 * This function extracts the modulus and public exponent from the given OpenSSL RSA key,
 * encodes them using base64url (without padding), and constructs a JWK-compliant JSON object.
 * The resulting JSON contains all fields required for interoperability with the Web Crypto API,
 * and is returned as a compact UTF-8 encoded QByteArray.
 *
 * @param rsaKey Pointer to the OpenSSL RSA key.
 * @return A QByteArray containing the JWK JSON representation of the public key,
 *         or an empty QByteArray on error.
 *
 * Example output:
 *
 * {
 *   "kty": "RSA",
 *
 *   "n": "<base64url modulus>",
 *
 *   "e": "<base64url exponent>",
 *
 *   "alg": "RSA-OAEP-256",
 *
 *   "key_ops": ["encrypt"],
 *
 *   "ext": true
 * }
 *
 * General steps of encoding/decoding for E2EE of the RSA public key part:
 *
 * use generateRsaKey() => QByteArray(PEM)
 *
 * use publicKeyFromPEM() => RSA(QByteArray(PEM))
 *
 * use exportJWKPublicKey() => JWK(RSA)
 */
QByteArray EncryptionUtils::exportJWKPublicKey(RSA *rsaKey)
{
    const BIGNUM *n, *e, *d;
    RSA_get0_key(rsaKey, &n, &e, &d);

    if (!n || !e) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Impossible to get RSA";
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
    jwkObj[QStringLiteral("kty")] = QStringLiteral("RSA");
    jwkObj[QStringLiteral("n")] = nBase64Url;
    jwkObj[QStringLiteral("e")] = eBase64Url;
    jwkObj[QStringLiteral("alg")] = QStringLiteral("RSA-OAEP-256");
    jwkObj[QStringLiteral("key_ops")] = QJsonArray() << QStringLiteral("encrypt");
    jwkObj[QStringLiteral("ext")] = true;

    QJsonDocument doc(jwkObj);
    return doc.toJson(QJsonDocument::Compact);
}

EncryptionUtils::RSAKeyPair EncryptionUtils::generateRSAKey()
{
    RSAKeyPair keyPair;

    int ret = 0;
    RSA *rsa = nullptr;
    BIGNUM *bne = nullptr;
    BIO *bp_public = nullptr;
    BIO *bp_private = nullptr;

    BIO *pubBio = BIO_new(BIO_s_mem());
    BIO *privBio = BIO_new(BIO_s_mem());

    int bits = 2048;
    unsigned long e = RSA_F4; // équivalent à 0x10001

    bne = BN_new();
    ret = BN_set_word(bne, e);
    if (ret != 1) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Error when generating exponent";
        return {};
    }

    rsa = RSA_new();
    ret = RSA_generate_key_ex(rsa, bits, bne, nullptr);
    if (ret != 1) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Error during generate key";
        return {};
    }

    /* bp_public = BIO_new_file("public_key.pem", "w+");
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
    } */

    PEM_write_bio_RSA_PUBKEY(pubBio, rsa);
    PEM_write_bio_RSAPrivateKey(privBio, rsa, nullptr, nullptr, 0, nullptr, nullptr);

    BUF_MEM *pubBuf = nullptr;
    BUF_MEM *privBuf = nullptr;

    BIO_get_mem_ptr(pubBio, &pubBuf);
    BIO_get_mem_ptr(privBio, &privBuf);

    keyPair.publicKey = QByteArray(pubBuf->data, pubBuf->length);
    keyPair.privateKey = QByteArray(privBuf->data, privBuf->length);

    // Libérer la mémoire
    // BIO_free_all(bp_public);
    // BIO_free_all(bp_private);
    BIO_free_all(pubBio);
    BIO_free_all(privBio);
    RSA_free(rsa);
    BN_free(bne);

    return keyPair;
}

QByteArray EncryptionUtils::encryptPrivateKey(const QByteArray &privateKey, const QByteArray &masterKey)
{
    if (privateKey.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Private key is empty";
        return {};
    }

    if (masterKey.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Master key is empty";
        return {};
    }

    const QByteArray iv = generateRandomIV(16);
    const QByteArray ciphertext = encryptAES_CBC_256(privateKey, masterKey, iv);

    if (ciphertext.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Encryption of the private key failed, cipherText is empty";
        return {};
    }

    QByteArray encrypted;
    encrypted.append(iv);
    encrypted.append(ciphertext);

    return encrypted;
}

QByteArray EncryptionUtils::decryptPrivateKey(const QByteArray &encryptedPrivateKey, const QByteArray &masterKey)
{
    if (encryptedPrivateKey.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Encrypted private key is empty";
        return {};
    }

    if (masterKey.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Master key is empty";
        return {};
    }

    const QByteArray iv = encryptedPrivateKey.left(16);
    const QByteArray cipherText = encryptedPrivateKey.mid(16);

    if (iv.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Decryption of the private key failed, 'iv' is empty";
        return {};
    }
    if (cipherText.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Decryption of the private key failed, 'cipherText' is empty";
        return {};
    }

    const QByteArray plainText = decryptAES_CBC_256(cipherText, masterKey, iv);

    if (plainText.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Decryption of the cipherText failed, plainText is empty";
        return {};
    }
    return plainText;
}

/**
 * @brief Derives the master key from the user's password and user ID.
 *
 * This function uses a password-based key derivation function (PBKDF2) to generate
 * a 256-bit (32-byte) AES master key from the provided password and user ID.
 * The master key is used to encrypt and decrypt the user's private RSA key.
 *
 * @param password The user's E2EE password.
 * @param salt user's unique identifier, sometimes called pepper if its constant.
 * @return A 32-byte (256-bit) master key as a QByteArray, or an empty QByteArray on failure.
 */
QByteArray EncryptionUtils::getMasterKey(const QString &password, const QString &salt)
{
    if (password.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Password can't be null. It's a bug";
        return {};
    }

    if (salt.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Salt(userId) can't be null. It's a bug";
        return {};
    }

    const QByteArray masterKey = deriveKey(salt.toUtf8(), password.toUtf8(), 1000, 32);
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

/**
 * @brief Generates a random 16-byte (128-bit) session key for AES encryption.
 *
 * @return A QByteArray containing 16 random bytes suitable for use as an AES-128 session key.
 */
QByteArray EncryptionUtils::generateSessionKey()
{
    return generateRandomIV(16);
}

/**
 * @brief Converts public key from QByteArray to RSA.
 * @param QByteArray &pem
 *
 */
RSA *EncryptionUtils::publicKeyFromPEM(const QByteArray &pem)
{
    BIO *bio = BIO_new_mem_buf(pem.constData(), pem.size());
    if (!bio) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "BIO_new_mem_buf failed!";
        return nullptr;
    }

    RSA *rsa = PEM_read_bio_RSA_PUBKEY(bio, nullptr, nullptr, nullptr);
    if (!rsa) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "PEM_read_bio_RSA_PUBKEY failed!";
        return nullptr;
    }

    BIO_free(bio);
    return rsa;
}

/**
 * @brief Converts private key from QByteArray to RSA.
 * @param QByteArray &pem
 *
 */
RSA *EncryptionUtils::privateKeyFromPEM(const QByteArray &pem)
{
    BIO *bio = BIO_new_mem_buf(pem.constData(), pem.size());
    if (!bio) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "BIO_new_mem_buf failed!";
        return nullptr;
    }

    RSA *rsa = PEM_read_bio_RSAPrivateKey(bio, nullptr, nullptr, nullptr);
    if (!rsa) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "PEM_read_bio_RSAPrivateKey failed!";
        BIO_free(bio);
        return nullptr;
    }

    BIO_free(bio);
    return rsa;
}

QByteArray EncryptionUtils::encryptSessionKey(const QByteArray &sessionKey, RSA *publicKey)
{
    QByteArray encryptedSessionKey(RSA_size(publicKey), 0);
    int bytes = RSA_public_encrypt(sessionKey.size(),
                                   reinterpret_cast<const unsigned char *>(sessionKey.constData()),
                                   reinterpret_cast<unsigned char *>(encryptedSessionKey.data()),
                                   publicKey,
                                   RSA_PKCS1_OAEP_PADDING);
    if (bytes == -1) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Session key encryption failed!";
        return {};
    }
    encryptedSessionKey.resize(bytes);
    return encryptedSessionKey;
}

QByteArray EncryptionUtils::decryptSessionKey(const QByteArray &encryptedSessionKey, RSA *privateKey)
{
    QByteArray decryptedSessionKey(RSA_size(privateKey), 0);
    int bytes = RSA_private_decrypt(encryptedSessionKey.size(),
                                    reinterpret_cast<const unsigned char *>(encryptedSessionKey.constData()),
                                    reinterpret_cast<unsigned char *>(decryptedSessionKey.data()),
                                    privateKey,
                                    RSA_PKCS1_OAEP_PADDING);
    if (bytes == -1) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Session key decryption failed!";
        return {};
    }
    decryptedSessionKey.resize(bytes);
    return decryptedSessionKey;
}

/**
 * @brief Encrypts a message using AES-128-CBC.
 * @param plainText The message to encrypt.
 * @param sessionKey The 16-byte session key.
 * @return The IV prepended to the ciphertext.
 */
QByteArray EncryptionUtils::encryptMessage(const QByteArray &plainText, const QByteArray &sessionKey)
{
    if (plainText.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "QByteArray EncryptionUtils::encryptMessage, plaintext is empty!";
        return {};
    }
    if (sessionKey.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "QByteArray EncryptionUtils::encryptMessage, session key is empty!";
        return {};
    }

    QByteArray iv = generateRandomIV(16);
    QByteArray cipherText = encryptAES_CBC_128(plainText, sessionKey, iv);

    if (cipherText.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "QByteArray EncryptionUtils::encryptMessage, message encryption failed, cipher text is empty!";
        return {};
    }

    QByteArray result;
    result.append(iv);
    result.append(cipherText);
    return result;
}

/**
 * @brief Decrypts a message using AES-128-CBC.
 * @param encrypted The message to decrypt.
 * @param sessionKey The 16-byte session key.
 * @return The decrypted message.
 */
QByteArray EncryptionUtils::decryptMessage(const QByteArray &encrypted, const QByteArray &sessionKey)
{
    if (encrypted.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "QByteArray EncryptionUtils::decryptMessage, encrypted message is empty!";
        return {};
    }
    if (sessionKey.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "QByteArray EncryptionUtils::decryptMessage, session key is empty!";
        return {};
    }

    const QByteArray iv = encrypted.left(16);
    const QByteArray cipherText = encrypted.mid(16);

    qDebug() << cipherText << "QByteArray cipherText = encrypted.mid(16)";

    const QByteArray plainText = decryptAES_CBC_128(cipherText, sessionKey, iv);

    qDebug() << plainText << "QByteArray plainText = decryptAES_CBC_128(cipherText, sessionKey, iv);";

    if (plainText.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "QByteArray EncryptionUtils::decryptMessage, message decryption failed, plain text is empty";
        return {};
    }

    return plainText;
}

QByteArray EncryptionUtils::decryptAES_CBC_256(const QByteArray &data, const QByteArray &key, const QByteArray &iv)
{
    EVP_CIPHER_CTX *ctx;
    int len;
    int plaintext_len;

    QByteArray plaintext(data.size(), 0);

    ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
        return {};

    if (1
        != EVP_DecryptInit_ex(ctx,
                              EVP_aes_256_cbc(),
                              nullptr,
                              reinterpret_cast<const unsigned char *>(key.data()),
                              reinterpret_cast<const unsigned char *>(iv.data()))) {
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }

    if (1
        != EVP_DecryptUpdate(ctx,
                             reinterpret_cast<unsigned char *>(plaintext.data()),
                             &len,
                             reinterpret_cast<const unsigned char *>(data.data()),
                             data.size())) {
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }
    plaintext_len = len;

    if (1 != EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char *>(plaintext.data()) + len, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }
    plaintext_len += len;
    plaintext.resize(plaintext_len);

    EVP_CIPHER_CTX_free(ctx);
    return plaintext;
}

QByteArray EncryptionUtils::encryptAES_CBC_256(const QByteArray &data, const QByteArray &key, const QByteArray &iv)
{
    EVP_CIPHER_CTX *ctx;
    int len;
    int ciphertext_len;

    int max_out_len = data.size() + EVP_CIPHER_block_size(EVP_aes_256_cbc());
    QByteArray cipherText(max_out_len, 0);

    if (!(ctx = EVP_CIPHER_CTX_new()))
        return {};

    if (1
        != EVP_EncryptInit_ex(ctx,
                              EVP_aes_256_cbc(),
                              NULL,
                              reinterpret_cast<const unsigned char *>(key.data()),
                              reinterpret_cast<const unsigned char *>(iv.data()))) {
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }

    if (1
        != EVP_EncryptUpdate(ctx,
                             reinterpret_cast<unsigned char *>(cipherText.data()),
                             &len,
                             reinterpret_cast<const unsigned char *>(data.data()),
                             data.size())) {
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }
    ciphertext_len = len;

    if (1 != EVP_EncryptFinal_ex(ctx, reinterpret_cast<unsigned char *>(cipherText.data()) + len, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }
    ciphertext_len += len;
    cipherText.resize(ciphertext_len);
    EVP_CIPHER_CTX_free(ctx);

    return cipherText;
}

QByteArray EncryptionUtils::encryptAES_CBC_128(const QByteArray &data, const QByteArray &key, const QByteArray &iv)
{
    EVP_CIPHER_CTX *ctx;
    int len;
    int ciphertext_len;

    int max_out_len = data.size() + EVP_CIPHER_block_size(EVP_aes_128_cbc());
    QByteArray cipherText(max_out_len, 0);

    if (!(ctx = EVP_CIPHER_CTX_new()))
        return {};

    if (1
        != EVP_EncryptInit_ex(ctx,
                              EVP_aes_128_cbc(),
                              NULL,
                              reinterpret_cast<const unsigned char *>(key.data()),
                              reinterpret_cast<const unsigned char *>(iv.data()))) {
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }

    if (1
        != EVP_EncryptUpdate(ctx,
                             reinterpret_cast<unsigned char *>(cipherText.data()),
                             &len,
                             reinterpret_cast<const unsigned char *>(data.data()),
                             data.size())) {
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }
    ciphertext_len = len;

    if (1 != EVP_EncryptFinal_ex(ctx, reinterpret_cast<unsigned char *>(cipherText.data()) + len, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }
    ciphertext_len += len;
    cipherText.resize(ciphertext_len);
    EVP_CIPHER_CTX_free(ctx);

    return cipherText;
}

QByteArray EncryptionUtils::decryptAES_CBC_128(const QByteArray &cipherText, const QByteArray &key, const QByteArray &iv)
{
    EVP_CIPHER_CTX *ctx;
    int len;
    int plainTextLen;

    QByteArray plainText(cipherText.size(), 0);

    if (!(ctx = EVP_CIPHER_CTX_new()))
        return {};

    if (1
        != EVP_DecryptInit_ex(ctx,
                              EVP_aes_128_cbc(),
                              NULL,
                              reinterpret_cast<const unsigned char *>(key.data()),
                              reinterpret_cast<const unsigned char *>(iv.data()))) {
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }

    if (1
        != EVP_DecryptUpdate(ctx,
                             reinterpret_cast<unsigned char *>(plainText.data()),
                             &len,
                             reinterpret_cast<const unsigned char *>(cipherText.data()),
                             cipherText.size())) {
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }
    plainTextLen = len;

    if (1 != EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char *>(plainText.data()) + len, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }
    plainTextLen += len;
    plainText.resize(plainTextLen);
    EVP_CIPHER_CTX_free(ctx);

    return plainText;
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
    const static QString characters = u"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()-_=+[]{}|;:,.<>?"_s;
    QString randomText;
    const int charSize = characters.size();

    for (int i = 0; i < length; ++i) {
        const int index = QRandomGenerator::global()->bounded(charSize);
        randomText.append(characters.at(index));
    }

    return randomText;
}

/**
 * @brief Derives a cryptographic key using PBKDF2 (Password-Based Key Derivation Function 2).
 *
 * This function uses OpenSSL's PKCS5_PBKDF2_HMAC to generate a key from a password and a salt.
 * It is typically used to derive an AES key from a user's password and unique identifier (salt).
 *
 * @param pepper The constant salt value (user's id).
 * @param baseKey The base key ( user's password).
 * @param iterations Number of PBKDF2 iterations (higher is more secure but slower).
 * @param keyLength Desired length of the derived key in bytes (e.g., 32 for AES-256).
 * @return The derived key as a QByteArray, or an empty QByteArray on failure.
 */
QByteArray EncryptionUtils::deriveKey(const QByteArray &pepper, const QByteArray &baseKey, int iterations, int keyLength)
{
    QByteArray derivedKey(keyLength, 0); // Allocate memory for the derived key

    // Use OpenSSL's PKCS5_PBKDF2_HMAC for PBKDF2 key derivation
    const int result = PKCS5_PBKDF2_HMAC(baseKey.data(),
                                         baseKey.size(), // Input key (password)
                                         reinterpret_cast<const unsigned char *>(pepper.data()),
                                         pepper.size(), // Salt
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

/* QJsonObject EncryptionUtils::exportPublicKeyJWK(const RSA *rsaKey)
{
    const BIGNUM *n, *e;
    RSA_get0_key(rsaKey, &n, &e, nullptr);

    auto b64url = [](const BIGNUM *bn) {
        QByteArray bytes(BN_num_bytes(bn), 0);
        BN_bn2bin(bn, reinterpret_cast<unsigned char *>(bytes.data()));
        return QString::fromLatin1(bytes.toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals));
    };

    QJsonObject jwk;
    jwk["kty"] = "RSA";
    jwk["n"] = b64url(n);
    jwk["e"] = b64url(e);
    jwk["alg"] = "RSA-OAEP-256";
    jwk["key_ops"] = QJsonArray{"encrypt"};
    jwk["ext"] = true;
    return jwk;
} */

/* QJsonObject EncryptionUtils::exportEncryptedPrivateKeyJWK(const QByteArray &encryptedPrivateKey)
{
    QJsonObject jwk;
    jwk["kty"] = "oct"; // "oct" for a symmetric (opaque) blob
    jwk["alg"] = "A256CBC"; // or whatever encryption you used
    jwk["ciphertext"] = QString::fromLatin1(encryptedPrivateKey.toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals));
    jwk["ext"] = true;
    return jwk;
}

QJsonObject EncryptionUtils::exportKeyPairJWK(RSA *rsaKey, const QByteArray &encryptedPrivateKey)
{
    QJsonObject bundle;
    bundle["public_key"] = exportPublicKeyJWK(rsaKey);
    bundle["encrypted_private_key"] = exportEncryptedPrivateKeyJWK(encryptedPrivateKey);
    return bundle;
} */

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
