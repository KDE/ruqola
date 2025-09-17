/*
  SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>
  SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "encryptiontestgui.h"
#include "encryption/encryptionutils.h"
#include <QApplication>
#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
EncryptionTestGui::EncryptionTestGui(QWidget *parent)
    : QWidget{parent}
    , mTextEdit(new QTextEdit(this))
    , mTextEditResult(new QTextEdit(this))

{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});
    auto labelInput = new QLabel(u"Input"_s, this);
    mainLayout->addWidget(labelInput);
    mainLayout->addWidget(mTextEdit);

    auto pushButtonDeriveMasterKey = new QPushButton(u"Derive Master Key"_s, this);
    mainLayout->addWidget(pushButtonDeriveMasterKey);

    connect(pushButtonDeriveMasterKey, &QPushButton::clicked, this, [this]() {
        auto dialog = new QDialog(this);
        dialog->setWindowTitle(u"Credentials"_s);

        auto saltEdit = new QLineEdit(dialog);
        auto passwordEdit = new QLineEdit(dialog);
        passwordEdit->setEchoMode(QLineEdit::Password);

        auto layout = new QGridLayout(dialog);
        layout->addWidget(new QLabel(u"Salt:"_s, dialog), 0, 0);
        layout->addWidget(saltEdit, 0, 1);
        layout->addWidget(new QLabel(u"Password:"_s, dialog), 1, 0);
        layout->addWidget(passwordEdit, 1, 1);

        auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, dialog);
        layout->addWidget(buttonBox, 2, 0, 1, 2);

        connect(buttonBox, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
        connect(buttonBox, &QDialogButtonBox::rejected, dialog, &QDialog::reject);

        if (dialog->exec()) {
            mSalt = saltEdit->text();
            mPassword = passwordEdit->text();
            mMasterKey = EncryptionUtils::getMasterKey(mPassword, mSalt);

            if (mMasterKey.isEmpty()) {
                mTextEditResult->setPlainText(u"Master key is empty, generation failed!"_s);
            } else {
                qDebug() << "Derived Master Key:" << mMasterKey.toBase64();
                mTextEditResult->setPlainText(u"Master Key derivation succeeded!\n"_s + QString::fromUtf8(mMasterKey.toBase64()));
            }
        }
        delete dialog;
    });

    auto pushButtonGenerateRSAKey = new QPushButton(u"Generate RSA Pair"_s, this);
    mainLayout->addWidget(pushButtonGenerateRSAKey);
    connect(pushButtonGenerateRSAKey, &QPushButton::clicked, this, [this]() {
        mRsaKeyPair = EncryptionUtils::generateRSAKey();
        qDebug() << "Public Key:\n " << mRsaKeyPair.publicKey << "Private Key:\n " << mRsaKeyPair.privateKey;
        mTextEditResult->setPlainText(u"RSA pair generation succeeded!\n"_s + QString::fromUtf8(mRsaKeyPair.publicKey)
                                      + QString::fromUtf8(mRsaKeyPair.privateKey));
    });

    auto pushButtonEncryptPrivateKey = new QPushButton(u"Encrypt Private Key"_s, this);
    mainLayout->addWidget(pushButtonEncryptPrivateKey);
    connect(pushButtonEncryptPrivateKey, &QPushButton::clicked, this, [this]() {
        if (mMasterKey.isEmpty()) {
            mTextEditResult->setPlainText(u"Master key is empty, encryption of the private key failed!"_s);
        } else if (mRsaKeyPair.privateKey.isEmpty()) {
            mTextEditResult->setPlainText(u"Private key is empty, encryption of the private key failed!"_s);
        } else {
            mEncryptedPrivateKey = EncryptionUtils::encryptPrivateKey(mRsaKeyPair.privateKey, mMasterKey);
            qDebug() << mEncryptedPrivateKey.toBase64() << "encrypted and encoded to 'base64()' private key ";
            mTextEditResult->setPlainText(u"Private key encryption succeeded!\n"_s + QString::fromUtf8(mEncryptedPrivateKey.toBase64()));
        }
    });

    auto pushButtonDecryptPrivateKey = new QPushButton(u"Decrypt Private Key"_s, this);
    mainLayout->addWidget(pushButtonDecryptPrivateKey);
    connect(pushButtonDecryptPrivateKey, &QPushButton::clicked, this, [this]() {
        if (mMasterKey.isEmpty()) {
            mTextEditResult->setPlainText(u"Master key is empty, encryption of the private key failed!"_s);
        } else if (mEncryptedPrivateKey.isEmpty()) {
            mTextEditResult->setPlainText(u"Encrypted private key is empty, decryption of the private key failed!"_s);
        } else {
            mDecryptedPrivateKey = EncryptionUtils::decryptPrivateKey(mEncryptedPrivateKey, mMasterKey);
            mTextEditResult->setPlainText(u"Private key decryption succeeded!\n"_s + QString::fromUtf8(mDecryptedPrivateKey));
            qDebug() << mDecryptedPrivateKey << "decrypted private key '\n' ";
            qDebug() << mRsaKeyPair.privateKey << "init private key '\n' ";
        }
    });

    auto pushButtonExportPublicKey = new QPushButton(u"Export Public Key"_s, this);
    mainLayout->addWidget(pushButtonExportPublicKey);
    connect(pushButtonExportPublicKey, &QPushButton::clicked, this, [this]() {
        const auto expPublicKey = EncryptionUtils::exportJWKPublicKey(EncryptionUtils::publicKeyFromPEM(mRsaKeyPair.publicKey));
        qDebug() << "Public Key:\n " << mRsaKeyPair.publicKey << "Exported Public Key:\n " << expPublicKey;
        mTextEditResult->setPlainText(QStringLiteral("Public key export succeeded!\n") + QString::fromUtf8(expPublicKey));
    });

    auto pushButtonGenerateSessionKey = new QPushButton(u"Generate Session Key"_s, this);
    mainLayout->addWidget(pushButtonGenerateSessionKey);
    connect(pushButtonGenerateSessionKey, &QPushButton::clicked, this, [this]() {
        mSessionKey = EncryptionUtils::generateSessionKey();
        qDebug() << "Derived Session Key:" << mSessionKey.toBase64();
        mTextEditResult->setPlainText(u"Session key generation succeeded!\n"_s + QString::fromUtf8(mSessionKey.toBase64()));
    });

    auto pushButtonEncryptSessionKey = new QPushButton(u"Encrypt Session Key"_s, this);
    mainLayout->addWidget(pushButtonEncryptSessionKey);
    connect(pushButtonEncryptSessionKey, &QPushButton::clicked, this, [this]() {
        auto publicKey = mRsaKeyPair.publicKey;
        if (publicKey.isEmpty())
            mTextEditResult->setPlainText(u"Public key is empty, session key encryption failed!\n"_s);
        else {
            RSA *publicKeyfromPem = EncryptionUtils::publicKeyFromPEM(publicKey);
            mEncryptedSessionKey = EncryptionUtils::encryptSessionKey(mSessionKey, publicKeyfromPem);
            qDebug() << "Public Key from PEM:" << publicKeyfromPem;
            qDebug() << "Encrypted Session Key:" << mEncryptedSessionKey.toBase64();
            mTextEditResult->setPlainText(u"Session key encryption succeeded!\n"_s + QString::fromUtf8(mEncryptedSessionKey.toBase64()));
        }
    });

    auto pushButtonDecryptSessionKey = new QPushButton(u"Decrypt Session Key"_s, this);
    mainLayout->addWidget(pushButtonDecryptSessionKey);
    connect(pushButtonDecryptSessionKey, &QPushButton::clicked, this, [this]() {
        auto privateKey = mRsaKeyPair.privateKey;
        if (privateKey.isEmpty())
            mTextEditResult->setPlainText(u"Private key is empty, session key decryption failed!\n"_s);
        else {
            RSA *privateKeyfromPem = EncryptionUtils::privateKeyFromPEM(privateKey);
            mDecryptedSessionKey = EncryptionUtils::decryptSessionKey(mEncryptedSessionKey, privateKeyfromPem);
            qDebug() << "Private Key from PEM:" << privateKeyfromPem;
            qDebug() << "Decrypted Session Key:" << mDecryptedSessionKey.toBase64();
            mTextEditResult->setPlainText(u"Session key decryption succeeded!\n"_s + QString::fromUtf8(mDecryptedSessionKey.toBase64()));
        }
    });
    auto pushButtonEncryptMessage = new QPushButton(u"Encrypt message"_s, this);
    mainLayout->addWidget(pushButtonEncryptMessage);
    connect(pushButtonEncryptMessage, &QPushButton::clicked, this, [this]() {
        const auto text = mTextEdit->toPlainText();
        if (text.isEmpty()) {
            mTextEditResult->setPlainText(u"Text cannot be null, message encryption failed!\n"_s);
        } else {
            mEncryptedMessage = EncryptionUtils::encryptMessage(text.toUtf8(), mSessionKey);
            qDebug() << "Encrypted message:" << mEncryptedMessage.toBase64();
            mTextEditResult->setPlainText(u"Message encryption succeeded!\n"_s + QString::fromUtf8(mEncryptedMessage.toBase64()));
            mTextEdit->clear();
        }
    });
    auto pushButtonDecryptMessage = new QPushButton(u"Decrypt message"_s, this);
    mainLayout->addWidget(pushButtonDecryptMessage);
    connect(pushButtonDecryptMessage, &QPushButton::clicked, this, [this]() {
        qDebug() << "Session key:" << mSessionKey;
        if (QString::fromUtf8(mEncryptedMessage).isEmpty()) {
            mTextEditResult->setPlainText(u"Encrypted message is null, message decryption failed!\n"_s);
            return;
        }
        mDecryptedMessage = EncryptionUtils::decryptMessage(mEncryptedMessage, mSessionKey);
        qDebug() << "Decrypted message:" << mDecryptedMessage;
        mTextEditResult->setPlainText(u"Message decryption succeeded!\n"_s + QString::fromUtf8(mDecryptedMessage));
    });

    auto pushButtonReset = new QPushButton(u"Reset"_s, this);
    mainLayout->addWidget(pushButtonReset);
    connect(pushButtonReset, &QPushButton::clicked, this, [this]() {
        mTextEdit->clear();
        mTextEditResult->clear();
        mMasterKey.clear();
        mPassword.clear();
        mSalt.clear();
        mRsaKeyPair.privateKey.clear();
        mRsaKeyPair.publicKey.clear();
        mSessionKey.clear();
        mEncryptedSessionKey.clear();
        mDecryptedSessionKey.clear();
        mEncryptedMessage.clear();
        mDecryptedMessage.clear();
        qDebug() << "Master Key: " << mMasterKey << "\nsalt: " << mSalt << "\npassword: " << mPassword << "\nprivatekey: " << mRsaKeyPair.privateKey
                 << "\npublickey: " << mRsaKeyPair.publicKey << "\nencrypted session key: " << mEncryptedSessionKey
                 << "\ndecrypted session key: " << mDecryptedSessionKey << "\nencrypted message: " << mEncryptedMessage
                 << "\ndecrypted message: " << mDecryptedMessage;
        mTextEditResult->setPlainText(u"Reset succeeded!\n"_s);
    });

    mTextEditResult->setReadOnly(true);
    auto labelOutput = new QLabel(u"Output"_s, this);
    mainLayout->addWidget(labelOutput);
    mainLayout->addWidget(mTextEditResult);
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    EncryptionTestGui w;
    w.resize(800, 600);
    w.show();
    return app.exec();
}

#include "moc_encryptiontestgui.cpp"
