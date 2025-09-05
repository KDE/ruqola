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

EncryptionTestGui::EncryptionTestGui(QWidget *parent)
    : QWidget{parent}
    , mTextEdit(new QTextEdit(this))
    , mTextEditResult(new QTextEdit(this))

{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});
    auto labelInput = new QLabel(QStringLiteral("Input"), this);
    mainLayout->addWidget(labelInput);
    mainLayout->addWidget(mTextEdit);

    auto pushButtonDeriveMasterKey = new QPushButton(QStringLiteral("Derive Master Key"), this);
    mainLayout->addWidget(pushButtonDeriveMasterKey);
    connect(pushButtonDeriveMasterKey, &QPushButton::clicked, this, [this]() {
        password = QStringLiteral("admin");
        userId = QStringLiteral("root");
        mMasterKey = EncryptionUtils::deriveKey(userId.toUtf8(), password.toUtf8(), 1000, 32);
        qDebug() << "Derived Master Key:" << mMasterKey.toHex();
        mTextEditResult->setPlainText(QStringLiteral("Master Key derived successfully!"));
    });

    auto pushButtonGenerateRSAKey = new QPushButton(QStringLiteral("Generate RSA Pair"), this);
    mainLayout->addWidget(pushButtonGenerateRSAKey);
    connect(pushButtonGenerateRSAKey, &QPushButton::clicked, this, []() {
        // test
    });
    auto pushButtonGenerateSessionKey = new QPushButton(QStringLiteral("Generate Session Key"), this);
    mainLayout->addWidget(pushButtonGenerateSessionKey);
    connect(pushButtonGenerateSessionKey, &QPushButton::clicked, this, []() {
        // test
    });

    auto pushButtonEncode = new QPushButton(QStringLiteral("Encode"), this);
    mainLayout->addWidget(pushButtonEncode);
    connect(pushButtonEncode, &QPushButton::clicked, this, []() {
        // test
    });
    auto pushButtonDecode = new QPushButton(QStringLiteral("Decode"), this);
    mainLayout->addWidget(pushButtonDecode);
    connect(pushButtonDecode, &QPushButton::clicked, this, []() {
        // test
    });

    auto pushButtonReset = new QPushButton(QStringLiteral("Reset"), this);
    mainLayout->addWidget(pushButtonReset);
    connect(pushButtonReset, &QPushButton::clicked, this, []() {
        EncryptionUtils::generateRSAKey();
    });

    mTextEditResult->setReadOnly(true);
    auto labelOutput = new QLabel(QStringLiteral("Output"), this);
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
