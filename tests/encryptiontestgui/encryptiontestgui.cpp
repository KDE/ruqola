/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "encryptiontestgui.h"
#include "encryption/encryptionutils.h"
#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
EncryptionTestGui::EncryptionTestGui(QWidget *parent)
    : QWidget{parent}
    , mTextEdit(new QTextEdit(this))
    , mTextEditResult(new QTextEdit(this))
    , mTextEditResultCMark(new QTextEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->addWidget(mTextEdit);

    auto pushButton = new QPushButton(QStringLiteral("Decode"), this);
    mainLayout->addWidget(pushButton);
    connect(pushButton, &QPushButton::clicked, this, [this]() {
        EncryptionUtils::generateRSAKey();
    });
    mTextEditResult->setReadOnly(true);
    auto label = new QLabel(QStringLiteral("Decode"), this);
    mainLayout->addWidget(label);
    mainLayout->addWidget(mTextEditResult);
}

EncryptionTestGui::~EncryptionTestGui()
{
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
