/*
  SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>
  SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "encryptionutils.h"
#include "ruqola_encryption_debug.h"
#include <QByteArray>
// https://docs.rocket.chat/customer-center/security-center/end-to-end-encryption-specifications

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QRandomGenerator>
#include <QUuid>
#include <openssl/evp.h>

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
    if (!rsaKey) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "RSA key is null";
        return {};
    }

    const BIGNUM *n;
    const BIGNUM *e;
    const BIGNUM *d;
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

    const QJsonDocument doc(jwkObj);
    return doc.toJson(QJsonDocument::Compact);
}

QByteArray EncryptionUtils::exportJWKPrivateKey(RSA *rsaKey)
{
    if (!rsaKey) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "RSA key is null";
        return {};
    }

    const BIGNUM *n = nullptr;
    const BIGNUM *e = nullptr;
    const BIGNUM *d = nullptr;
    RSA_get0_key(rsaKey, &n, &e, &d);
    if (!n || !e || !d) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "exportJWKPrivateKey: missing required key components";
        return {};
    }

    const auto toBase64Url = [](const BIGNUM *bigNumber) -> QString {
        if (!bigNumber) {
            return {};
        }
        QByteArray bytes(BN_num_bytes(bigNumber), 0);
        BN_bn2bin(bigNumber, reinterpret_cast<unsigned char *>(bytes.data()));
        return QString::fromLatin1(bytes.toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals));
    };

    QJsonObject jwkObj;
    jwkObj[QStringLiteral("kty")] = QStringLiteral("RSA");
    jwkObj[QStringLiteral("n")] = toBase64Url(n);
    jwkObj[QStringLiteral("e")] = toBase64Url(e);
    jwkObj[QStringLiteral("d")] = toBase64Url(d);
    // The CRT parameters are optional in JWK but every WebCrypto implementation exports them.
    const std::pair<const char *, const BIGNUM *> crtParameters[] = {
        {"p", RSA_get0_p(rsaKey)},
        {"q", RSA_get0_q(rsaKey)},
        {"dp", RSA_get0_dmp1(rsaKey)},
        {"dq", RSA_get0_dmq1(rsaKey)},
        {"qi", RSA_get0_iqmp(rsaKey)},
    };
    for (const auto &[name, value] : crtParameters) {
        const QString encodedValue = toBase64Url(value);
        if (!encodedValue.isEmpty()) {
            jwkObj[QLatin1StringView(name)] = encodedValue;
        }
    }
    jwkObj[QStringLiteral("alg")] = QStringLiteral("RSA-OAEP-256");
    jwkObj[QStringLiteral("key_ops")] = QJsonArray() << QStringLiteral("decrypt");
    jwkObj[QStringLiteral("ext")] = true;

    const QJsonDocument doc(jwkObj);
    return doc.toJson(QJsonDocument::Compact);
}

QByteArray EncryptionUtils::encryptPrivateKeyV2(const QByteArray &privateKey, const QString &password, const QString &userId)
{
    if (privateKey.isEmpty() || password.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "encryptPrivateKeyV2: missing private key or password";
        return {};
    }
    // Port of Rocket.Chat's Keychain::encryptKey().
    constexpr int iterations = 100000;
    const QString salt = QStringLiteral("v2:%1:%2").arg(userId, QUuid::createUuid().toString(QUuid::WithoutBraces));
    const QByteArray masterKey = deriveMasterKey(salt, password, iterations);
    if (masterKey.isEmpty()) {
        return {};
    }
    // A 12-byte IV is what tells the other clients to read the key back as AES-GCM: a 16-byte one
    // is understood as the legacy AES-CBC layout.
    const QByteArray iv = generateRandomIV(12);
    const QByteArray ciphertext = encryptAES_GCM_256(privateKey, masterKey, iv);
    if (ciphertext.isEmpty()) {
        return {};
    }

    QJsonObject storedKey;
    storedKey[QStringLiteral("iv")] = QString::fromLatin1(iv.toBase64());
    storedKey[QStringLiteral("ciphertext")] = QString::fromLatin1(ciphertext.toBase64());
    storedKey[QStringLiteral("salt")] = salt;
    storedKey[QStringLiteral("iterations")] = iterations;
    return QJsonDocument(storedKey).toJson(QJsonDocument::Compact);
}

QByteArray EncryptionUtils::exportJWKEncryptedPrivateKey(const QByteArray &encryptedPrivateKey)
{
    QJsonObject jwkObj;
    jwkObj[QStringLiteral("kty")] = QStringLiteral("RSA");
    jwkObj[QStringLiteral("alg")] = QStringLiteral("RSA-OAEP-256");
    jwkObj[QStringLiteral("key_ops")] = QJsonArray() << QStringLiteral("decrypt");
    jwkObj[QStringLiteral("ext")] = true;

    // Store the encrypted private key as base64url
    const QString ePrivKeyBase64Url = QString::fromLatin1(encryptedPrivateKey.toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals));
    jwkObj[QStringLiteral("RSA-EPrivKey")] = ePrivKeyBase64Url;

    const QJsonDocument doc(jwkObj);
    return doc.toJson(QJsonDocument::Compact);
}

EncryptionUtils::RSAKeyPair EncryptionUtils::generateRSAKey()
{
    RSAKeyPair keyPair;

    int ret = 0;
    RSA *rsa = nullptr;
    BIGNUM *bne = nullptr;
    // BIO *bp_public = nullptr;
    // BIO *bp_private = nullptr;

    BIO *pubBio = BIO_new(BIO_s_mem());
    BIO *privBio = BIO_new(BIO_s_mem());
    if (!pubBio || !privBio) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Error when allocating the key buffers";
        BIO_free_all(pubBio);
        BIO_free_all(privBio);
        return {};
    }

    const int bits = 2048;
    const unsigned long e = RSA_F4; // équivalent à 0x10001

    bne = BN_new();
    ret = BN_set_word(bne, e);
    if (ret != 1) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Error when generating exponent";
        BN_free(bne);
        BIO_free_all(pubBio);
        BIO_free_all(privBio);
        return {};
    }

    rsa = RSA_new();
    if (!rsa) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Error when allocating the key";
        BN_free(bne);
        BIO_free_all(pubBio);
        BIO_free_all(privBio);
        return {};
    }
    ret = RSA_generate_key_ex(rsa, bits, bne, nullptr);
    if (ret != 1) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Error during generate key";
        BN_free(bne);
        RSA_free(rsa);
        BIO_free_all(pubBio);
        BIO_free_all(privBio);
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

    QByteArray iv = generateRandomIV(16);
    QByteArray ciphertext = encryptAES_CBC_256(privateKey, masterKey, iv);

    if (ciphertext.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Encryption of the private key failed, cipherText is empty";
        return {};
    }

    QByteArray encrypted;
    encrypted.append(std::move(iv));
    encrypted.append(std::move(ciphertext));

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

    // Never log 'iv'/'cipherText'/'masterKey' here: they are the user's private key material.
    if (iv.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Decryption of the private key failed, 'iv' is empty";
        return {};
    }
    if (cipherText.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Decryption of the private key failed, 'cipherText' is empty";
        return {};
    }

    QByteArray plainText = decryptAES_CBC_256(cipherText, masterKey, iv);

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

    QByteArray masterKey = deriveMasterKey(salt, password, 1000);
    if (masterKey.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Master key derivation failed!";
        return {};
    }

    return masterKey;
}

/**
 * @brief Turns a password or a salt into the bytes Rocket.Chat derives keys from.
 *
 * Rocket.Chat feeds PBKDF2 the code units of the string, one byte each (Binary.decode() in its
 * e2ee/binary.ts), and not its UTF-8 encoding. Both agree for ASCII but part company right after:
 * 'é' is the single byte 0xE9 there and two bytes in UTF-8, which derives a different master key
 * and leaves the private key of an account with an accented password impossible to unlock from the
 * other client.
 *
 * @param text The password or salt.
 * @return Its bytes, or an empty array when a character does not fit in one. Rocket.Chat throws a
 *         RangeError in that case, so no client can derive a key from such a password: mangling it
 *         into '?' would only turn that into a key nothing can reproduce.
 */
QByteArray EncryptionUtils::keyDerivationBytes(const QString &text)
{
    QByteArray bytes;
    bytes.reserve(text.size());
    for (const QChar character : text) {
        if (character.unicode() > 0xFF) {
            qCWarning(RUQOLA_ENCRYPTION_LOG) << "keyDerivationBytes: no Rocket.Chat client can derive a key from a text holding this character";
            return {};
        }
        bytes.append(static_cast<char>(character.unicode()));
    }
    return bytes;
}

QByteArray EncryptionUtils::deriveMasterKey(const QString &salt, const QString &password, int iterations)
{
    if (password.isEmpty() || salt.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "deriveMasterKey: password or salt is empty";
        return {};
    }
    const QByteArray passwordBytes = keyDerivationBytes(password);
    const QByteArray saltBytes = keyDerivationBytes(salt);
    // An empty conversion must never reach PBKDF2: it happily derives a key from no password at all.
    if (passwordBytes.isEmpty() || saltBytes.isEmpty()) {
        return {};
    }
    return deriveKey(saltBytes, passwordBytes, iterations, 32);
}

/**
 * @brief Generates a random 16-byte (128-bit) session key for AES encryption.
 *
 * @return A QByteArray containing 16 random bytes suitable for use as an AES-128 session key.
 */
QByteArray EncryptionUtils::generateSessionKey()
{
    return generateRandomIV(32);
}

/**
 * @brief Converts a raw AES session key to JWK JSON format.
 *
 * Rocket.Chat distributes session keys as the RSA-OAEP-encrypted bytes of a JWK
 * JSON string (not raw key bytes). This function produces the JSON payload that
 * must be encrypted before sharing with other participants so that both Ruqola
 * and Rocket.Chat web/mobile clients can import it.
 *
 * The key length picks the algorithm, the way Rocket.Chat's ALGORITHM_MAP does: 32 bytes is the
 * AES-GCM-256 of every room created nowadays, 16 bytes the AES-CBC-128 of the rooms keyed before
 * the GCM switch. Re-sharing such a legacy key has to keep announcing it as A128CBC, otherwise
 * the recipient imports it as GCM and can read nothing.
 *
 * @param rawKey The raw AES key: 32 bytes (A256GCM) or 16 bytes (A128CBC).
 * @return JWK JSON bytes, e.g.
 *   {"k":"<base64url>","alg":"A256GCM","ext":true,"key_ops":["encrypt","decrypt"],"kty":"oct"}
 */
QByteArray EncryptionUtils::sessionKeyToJWK(const QByteArray &rawKey)
{
    QString algorithm;
    if (rawKey.size() == 32) {
        algorithm = QStringLiteral("A256GCM");
    } else if (rawKey.size() == 16) {
        algorithm = QStringLiteral("A128CBC");
    } else {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "sessionKeyToJWK: expected a 16- or 32-byte key, got" << rawKey.size();
        return {};
    }
    QJsonObject jwk;
    jwk[QStringLiteral("k")] = QString::fromLatin1(rawKey.toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals));
    jwk[QStringLiteral("alg")] = algorithm;
    jwk[QStringLiteral("ext")] = true;
    jwk[QStringLiteral("key_ops")] = QJsonArray() << QStringLiteral("encrypt") << QStringLiteral("decrypt");
    jwk[QStringLiteral("kty")] = QStringLiteral("oct");
    return QJsonDocument(jwk).toJson(QJsonDocument::Compact);
}

/**
 * @brief Generates a room-specific key identifier (keyId).
 *
 * Matches Rocket.Chat's e2e.room implementation:
 *   this.keyID = crypto.randomUUID()
 *
 * The keyId is sent to the server via e2e.setRoomKeyID and is prepended to
 * every encrypted session key shared with room participants. During decryption
 * the keyId is used to look up the correct room key (current or from oldRoomKeys).
 *
 * @return A UUID string without braces, e.g. "550e8400-e29b-41d4-a716-446655440000".
 */
QString EncryptionUtils::generateRoomKeyId()
{
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}

/**
 * @brief Converts public key from QByteArray to RSA.
 * @param QByteArray &pem
 *
 */
RSA *EncryptionUtils::publicKeyFromPEM(const QByteArray &pem)
{
    if (pem.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "publicKeyFromPEM: pem is empty";
        return nullptr;
    }

    BIO *bio = BIO_new_mem_buf(pem.constData(), pem.size());
    if (!bio) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "BIO_new_mem_buf failed!";
        return nullptr;
    }

    RSA *rsa = PEM_read_bio_RSA_PUBKEY(bio, nullptr, nullptr, nullptr);
    if (!rsa) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "PEM_read_bio_RSA_PUBKEY failed!";
        BIO_free(bio);
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
    if (pem.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "privateKeyFromPEM: pem is empty";
        return nullptr;
    }

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
    if (sessionKey.isEmpty() || !publicKey) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Session key encryption failed: invalid input";
        return {};
    }

    const int rsaSize = RSA_size(publicKey);
    QByteArray padded(rsaSize, 0);
    if (RSA_padding_add_PKCS1_OAEP_mgf1(reinterpret_cast<unsigned char *>(padded.data()),
                                        rsaSize,
                                        reinterpret_cast<const unsigned char *>(sessionKey.constData()),
                                        sessionKey.size(),
                                        nullptr,
                                        0,
                                        EVP_sha256(),
                                        EVP_sha256())
        != 1) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Session key encryption failed: OAEP-SHA256 padding failed";
        return {};
    }

    QByteArray encryptedSessionKey(rsaSize, 0);
    const int bytes = RSA_public_encrypt(rsaSize,
                                         reinterpret_cast<const unsigned char *>(padded.constData()),
                                         reinterpret_cast<unsigned char *>(encryptedSessionKey.data()),
                                         publicKey,
                                         RSA_NO_PADDING);
    if (bytes != rsaSize) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Session key encryption failed!";
        return {};
    }

    encryptedSessionKey.resize(bytes);
    return encryptedSessionKey;
}

QByteArray EncryptionUtils::decryptSessionKey(const QByteArray &encryptedSessionKey, RSA *privateKey)
{
    if (encryptedSessionKey.isEmpty() || !privateKey) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Session key decryption failed: invalid input" << encryptedSessionKey << " privateKey " << privateKey;
        return {};
    }

    const int rsaSize = RSA_size(privateKey);
    if (encryptedSessionKey.size() != rsaSize) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Session key decryption failed: encrypted key size" << encryptedSessionKey.size() << "does not match RSA size"
                                         << rsaSize;
        return {};
    }

    QByteArray encoded(rsaSize, 0);
    const int encodedLen = RSA_private_decrypt(encryptedSessionKey.size(),
                                               reinterpret_cast<const unsigned char *>(encryptedSessionKey.constData()),
                                               reinterpret_cast<unsigned char *>(encoded.data()),
                                               privateKey,
                                               RSA_NO_PADDING);
    if (encodedLen != rsaSize) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Session key decryption failed!";
        return {};
    }

    auto decryptWithHash = [&](const EVP_MD *oaepMd, const EVP_MD *mgf1Md, const char *label) -> QByteArray {
        QByteArray out(rsaSize, 0);
        const int decodedLen = RSA_padding_check_PKCS1_OAEP_mgf1(reinterpret_cast<unsigned char *>(out.data()),
                                                                 out.size(),
                                                                 reinterpret_cast<const unsigned char *>(encoded.constData()),
                                                                 encodedLen,
                                                                 rsaSize,
                                                                 nullptr,
                                                                 0,
                                                                 oaepMd,
                                                                 mgf1Md);
        if (decodedLen < 0) {
            qCDebug(RUQOLA_ENCRYPTION_LOG) << "Session key OAEP decode failed with" << label;
            return {};
        }
        qCDebug(RUQOLA_ENCRYPTION_LOG) << "Session key OAEP decode succeeded with" << label << "decodedLen=" << decodedLen;
        out.resize(decodedLen);
        return out;
    };

    // Rocket.Chat uses RSA-OAEP with SHA-256. Some environments encode MGF1
    // with SHA-1 while keeping OAEP hash at SHA-256, so try both first.
    QByteArray decryptedSessionKey = decryptWithHash(EVP_sha256(), EVP_sha256(), "oaep=sha256 mgf1=sha256");
    if (decryptedSessionKey.isEmpty()) {
        decryptedSessionKey = decryptWithHash(EVP_sha256(), EVP_sha1(), "oaep=sha256 mgf1=sha1");
    }
    if (decryptedSessionKey.isEmpty()) {
        // Backward compatibility for previously stored OAEP-SHA1 ciphertexts.
        decryptedSessionKey = decryptWithHash(EVP_sha1(), EVP_sha1(), "oaep=sha1 mgf1=sha1");
    }

    if (decryptedSessionKey.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Session key decryption failed!";
    }
    return decryptedSessionKey;
}

/**
 * @brief Encrypts a message with the room key, AES-CBC in the mode the key length dictates.
 * @param plainText The message to encrypt.
 * @param sessionKey The session key: 32 bytes (AES-256) or 16 bytes (AES-128).
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

    // The mode follows the key, as everywhere else: passing a 32-byte key to the AES-128 helper
    // silently threw away half of it and produced something no Rocket.Chat client could read.
    if (sessionKey.size() != 32 && sessionKey.size() != 16) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "QByteArray EncryptionUtils::encryptMessage, unexpected session key size" << sessionKey.size();
        return {};
    }

    QByteArray iv = generateRandomIV(16);
    QByteArray cipherText = sessionKey.size() == 32 ? encryptAES_CBC_256(plainText, sessionKey, iv) : encryptAES_CBC_128(plainText, sessionKey, iv);

    if (cipherText.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "QByteArray EncryptionUtils::encryptMessage, message encryption failed, cipher text is empty!";
        return {};
    }

    QByteArray result;
    result.append(std::move(iv));
    result.append(std::move(cipherText));
    return result;
}

/**
 * @brief Decrypts a message with the room key, AES-CBC in the mode the key length dictates.
 * @param encrypted The message to decrypt.
 * @param sessionKey The session key: 32 bytes (AES-256) or 16 bytes (AES-128).
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

    if (sessionKey.size() != 32 && sessionKey.size() != 16) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "QByteArray EncryptionUtils::decryptMessage, unexpected session key size" << sessionKey.size();
        return {};
    }

    const QByteArray iv = encrypted.left(16);
    const QByteArray cipherText = encrypted.mid(16);

    QByteArray plainText = sessionKey.size() == 32 ? decryptAES_CBC_256(cipherText, sessionKey, iv) : decryptAES_CBC_128(cipherText, sessionKey, iv);

    if (plainText.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "QByteArray EncryptionUtils::decryptMessage, message decryption failed, plain text is empty";
        return {};
    }

    return plainText;
}

QByteArray EncryptionUtils::encryptAES_GCM_256(const QByteArray &plainText, const QByteArray &key, const QByteArray &iv)
{
    if (plainText.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "encryptAES_GCM_256: plaintext is empty";
        return {};
    }

    if (key.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "encryptAES_GCM_256: key is empty";
        return {};
    }

    if (iv.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "encryptAES_GCM_256: iv is empty";
        return {};
    }

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        return {};
    }

    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr)) {
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }
    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, iv.size(), nullptr)) {
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }
    if (1
        != EVP_EncryptInit_ex(ctx, nullptr, nullptr, reinterpret_cast<const unsigned char *>(key.data()), reinterpret_cast<const unsigned char *>(iv.data()))) {
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }

    QByteArray ciphertext(plainText.size(), 0);
    int len = 0;
    if (1
        != EVP_EncryptUpdate(ctx,
                             reinterpret_cast<unsigned char *>(ciphertext.data()),
                             &len,
                             reinterpret_cast<const unsigned char *>(plainText.constData()),
                             plainText.size())) {
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }
    int ciphertextLen = len;

    if (1 != EVP_EncryptFinal_ex(ctx, reinterpret_cast<unsigned char *>(ciphertext.data()) + ciphertextLen, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }
    ciphertextLen += len;
    ciphertext.resize(ciphertextLen);

    constexpr int tagLen = 16;
    QByteArray tag(tagLen, 0);
    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, tagLen, tag.data())) {
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }

    EVP_CIPHER_CTX_free(ctx);
    return ciphertext + tag;
}

QByteArray EncryptionUtils::decryptAES_GCM_256(const QByteArray &ciphertext, const QByteArray &key, const QByteArray &iv)
{
    // AES-GCM: Web Crypto appends the 16-byte authentication tag after the ciphertext.
    constexpr int tagLen = 16;
    if (ciphertext.size() <= tagLen) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "decryptAES_GCM_256: ciphertext too short";
        return {};
    }

    const QByteArray data = ciphertext.left(ciphertext.size() - tagLen);
    const QByteArray tag = ciphertext.right(tagLen);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        return {};
    }

    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr)) {
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }
    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, iv.size(), nullptr)) {
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }
    if (1
        != EVP_DecryptInit_ex(ctx, nullptr, nullptr, reinterpret_cast<const unsigned char *>(key.data()), reinterpret_cast<const unsigned char *>(iv.data()))) {
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }

    QByteArray plaintext(data.size(), 0);
    int len = 0;
    if (1
        != EVP_DecryptUpdate(ctx,
                             reinterpret_cast<unsigned char *>(plaintext.data()),
                             &len,
                             reinterpret_cast<const unsigned char *>(data.data()),
                             data.size())) {
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }
    int plaintextLen = len;

    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, tagLen, const_cast<char *>(tag.data()))) {
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }

    if (EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char *>(plaintext.data()) + plaintextLen, &len) <= 0) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "decryptAES_GCM_256: authentication tag verification failed";
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }
    plaintextLen += len;
    plaintext.resize(plaintextLen);

    EVP_CIPHER_CTX_free(ctx);
    return plaintext;
}

/**
 * @brief Converts a JWK RSA private key JSON to PEM format.
 *
 * Rocket.Chat encrypts the private key as JWK JSON (not PEM). This function
 * reconstructs the OpenSSL RSA key from the JWK components and serialises it
 * as a PKCS#1 PEM string so that the rest of the code can use it uniformly.
 *
 * @param jwkJson UTF-8 encoded JSON containing at minimum the keys:
 *        kty, n, e, d, p, q, dp, dq, qi (all base64url-encoded BIGNUMs).
 * @return PEM-encoded private key, or empty on error.
 */
QByteArray EncryptionUtils::privateKeyJWKToPEM(const QByteArray &jwkJson)
{
    const QJsonDocument doc = QJsonDocument::fromJson(jwkJson);
    if (doc.isNull() || !doc.isObject()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "privateKeyJWKToPEM: invalid JSON";
        return {};
    }
    const QJsonObject obj = doc.object();
    if (obj.value(QStringLiteral("kty")).toString() != QLatin1String("RSA")) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "privateKeyJWKToPEM: not an RSA key";
        return {};
    }

    // Helper: base64url → BIGNUM
    const auto b64urlToBN = [](const QString &b64url) -> BIGNUM * {
        // Normalise: base64url → standard base64 with padding
        QString b64 = b64url;
        b64.replace(QLatin1Char('-'), QLatin1Char('+')).replace(QLatin1Char('_'), QLatin1Char('/'));
        while (b64.size() % 4 != 0) {
            b64.append(QLatin1Char('='));
        }
        const QByteArray bytes = QByteArray::fromBase64(b64.toLatin1());
        if (bytes.isEmpty()) {
            return nullptr;
        }
        return BN_bin2bn(reinterpret_cast<const unsigned char *>(bytes.constData()), bytes.size(), nullptr);
    };

    BIGNUM *n = b64urlToBN(obj.value(QStringLiteral("n")).toString());
    BIGNUM *e = b64urlToBN(obj.value(QStringLiteral("e")).toString());
    BIGNUM *d = b64urlToBN(obj.value(QStringLiteral("d")).toString());
    BIGNUM *p = b64urlToBN(obj.value(QStringLiteral("p")).toString());
    BIGNUM *q = b64urlToBN(obj.value(QStringLiteral("q")).toString());
    BIGNUM *dp = b64urlToBN(obj.value(QStringLiteral("dp")).toString());
    BIGNUM *dq = b64urlToBN(obj.value(QStringLiteral("dq")).toString());
    BIGNUM *qi = b64urlToBN(obj.value(QStringLiteral("qi")).toString());

    if (!n || !e || !d) {
        BN_free(n);
        BN_free(e);
        BN_free(d);
        BN_free(p);
        BN_free(q);
        BN_free(dp);
        BN_free(dq);
        BN_free(qi);
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "privateKeyJWKToPEM: missing required key components";
        return {};
    }

    RSA *rsa = RSA_new();
    if (!rsa) {
        BN_free(n);
        BN_free(e);
        BN_free(d);
        BN_free(p);
        BN_free(q);
        BN_free(dp);
        BN_free(dq);
        BN_free(qi);
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "privateKeyJWKToPEM: unable to allocate the key";
        return {};
    }
    // RSA_set0_* transfers ownership of the BIGNUMs to rsa. The optional groups are all-or-nothing,
    // so whatever is left over on an incomplete one has to be released here instead of leaking.
    RSA_set0_key(rsa, n, e, d);
    if (p && q) {
        RSA_set0_factors(rsa, p, q);
    } else {
        BN_free(p);
        BN_free(q);
    }
    if (dp && dq && qi) {
        RSA_set0_crt_params(rsa, dp, dq, qi);
    } else {
        BN_free(dp);
        BN_free(dq);
        BN_free(qi);
    }

    BIO *bio = BIO_new(BIO_s_mem());
    if (!bio) {
        RSA_free(rsa);
        return {};
    }
    PEM_write_bio_RSAPrivateKey(bio, rsa, nullptr, nullptr, 0, nullptr, nullptr);

    BUF_MEM *buf = nullptr;
    BIO_get_mem_ptr(bio, &buf);
    const QByteArray pem(buf->data, static_cast<qsizetype>(buf->length));

    BIO_free(bio);
    RSA_free(rsa);
    return pem;
}

/**
 * @brief Converts a JWK RSA public key JSON to PEM format.
 *
 * Rocket.Chat stores public keys as JWK JSON (kty=RSA, with base64url-encoded
 * n and e fields). This function reconstructs the OpenSSL RSA public key and
 * serialises it as a SubjectPublicKeyInfo PEM so that publicKeyFromPEM() can
 * consume it uniformly.
 *
 * @param jwkJson UTF-8 encoded JSON containing at minimum: kty, n, e.
 * @return PEM-encoded public key, or empty on error.
 */
QByteArray EncryptionUtils::publicKeyJWKToPEM(const QByteArray &jwkJson)
{
    const QJsonDocument doc = QJsonDocument::fromJson(jwkJson);
    if (doc.isNull() || !doc.isObject()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "publicKeyJWKToPEM: invalid JSON";
        return {};
    }
    const QJsonObject obj = doc.object();
    if (obj.value(QStringLiteral("kty")).toString() != QLatin1String("RSA")) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "publicKeyJWKToPEM: not an RSA key";
        return {};
    }

    const auto b64urlToBN = [](const QString &b64url) -> BIGNUM * {
        QString b64 = b64url;
        b64.replace(QLatin1Char('-'), QLatin1Char('+')).replace(QLatin1Char('_'), QLatin1Char('/'));
        while (b64.size() % 4 != 0) {
            b64.append(QLatin1Char('='));
        }
        const QByteArray bytes = QByteArray::fromBase64(b64.toLatin1());
        if (bytes.isEmpty()) {
            return nullptr;
        }
        return BN_bin2bn(reinterpret_cast<const unsigned char *>(bytes.constData()), bytes.size(), nullptr);
    };

    BIGNUM *n = b64urlToBN(obj.value(QStringLiteral("n")).toString());
    BIGNUM *e = b64urlToBN(obj.value(QStringLiteral("e")).toString());
    if (!n || !e) {
        BN_free(n);
        BN_free(e);
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "publicKeyJWKToPEM: missing n or e components";
        return {};
    }

    RSA *rsa = RSA_new();
    if (!rsa) {
        BN_free(n);
        BN_free(e);
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "publicKeyJWKToPEM: unable to allocate the key";
        return {};
    }
    RSA_set0_key(rsa, n, e, nullptr); // transfers ownership

    // Wrap in EVP_PKEY and write as SubjectPublicKeyInfo PEM (BEGIN PUBLIC KEY)
    EVP_PKEY *pkey = EVP_PKEY_new();
    if (!pkey) {
        RSA_free(rsa);
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "publicKeyJWKToPEM: unable to allocate the key wrapper";
        return {};
    }
    EVP_PKEY_assign_RSA(pkey, rsa); // pkey owns rsa from here
    BIO *bio = BIO_new(BIO_s_mem());
    if (!bio) {
        EVP_PKEY_free(pkey);
        return {};
    }
    PEM_write_bio_PUBKEY(bio, pkey);

    BUF_MEM *buf = nullptr;
    BIO_get_mem_ptr(bio, &buf);
    const QByteArray pem(buf->data, static_cast<qsizetype>(buf->length));

    BIO_free(bio);
    EVP_PKEY_free(pkey);
    return pem;
}

QByteArray EncryptionUtils::decryptAES_CBC_256(const QByteArray &data, const QByteArray &key, const QByteArray &iv)
{
    EVP_CIPHER_CTX *ctx;
    int len;
    int plaintext_len;

    QByteArray plaintext(data.size(), 0);

    ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        return {};
    }

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

    const int max_out_len = data.size() + EVP_CIPHER_block_size(EVP_aes_256_cbc());
    QByteArray cipherText(max_out_len, 0);

    if (ctx = EVP_CIPHER_CTX_new(); !ctx) {
        return {};
    }

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

    const int max_out_len = data.size() + EVP_CIPHER_block_size(EVP_aes_128_cbc());
    QByteArray cipherText(max_out_len, 0);

    if (ctx = EVP_CIPHER_CTX_new(); !ctx) {
        return {};
    }

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

    if (ctx = EVP_CIPHER_CTX_new(); !ctx) {
        return {};
    }

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
        // system() is the cryptographically secure generator: global() is only securely seeded.
        const int index = QRandomGenerator::system()->bounded(charSize);
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

EncryptionUtils::EncryptionInfo EncryptionUtils::splitVectorAndEcryptedData(const QByteArray &cipherText)
{
    EncryptionUtils::EncryptionInfo info;
    if (cipherText.size() > 16) {
        info.vector = cipherText.left(16);
        info.encryptedData = cipherText.mid(16);
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
    const QByteArray charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()-_=+[]{}|;:,.<>?"_ba;
    const int charsetSize = charset.size();
    // A plain 'byte % charsetSize' would favour the first (256 % charsetSize) characters, so drop the
    // bytes of the incomplete last range instead of folding them back into the charset.
    const int rejectionLimit = 256 - (256 % charsetSize);

    QString randomStr;
    randomStr.reserve(numberChar);
    while (randomStr.size() < numberChar) {
        const QByteArray randomBytes = generateRandomIV(numberChar);
        if (randomBytes.isEmpty()) {
            return {};
        }
        for (const char randomByte : randomBytes) {
            const int value = static_cast<unsigned char>(randomByte);
            if (value >= rejectionLimit) {
                continue;
            }
            randomStr.append(QLatin1Char(charset.at(value % charsetSize)));
            if (randomStr.size() == numberChar) {
                break;
            }
        }
    }
    return randomStr;
}
