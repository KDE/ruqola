/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccount2fadisabletotpwidget.h"
#include "ddpapi/ddpclient.h"
#include "misc/lineeditcatchreturnkey.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

MyAccount2FaDisableTotpWidget::MyAccount2FaDisableTotpWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mRocketChatAccount(account)
    , mDisableCodeLineEdit(new QLineEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    auto label = new QLabel(i18n("Open your authentication app and enter the code.\nYou can also use one of your backup codes."), this);
    label->setObjectName(QStringLiteral("label"));
    mainLayout->addWidget(label);

    auto hboxLayout = new QHBoxLayout;
    hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
    hboxLayout->setContentsMargins({});
    mainLayout->addLayout(hboxLayout);

    mDisableCodeLineEdit->setObjectName(QStringLiteral("mDisableCodeLineEdit"));
    mDisableCodeLineEdit->setPlaceholderText(i18n("Enter Authentication Code"));
    new LineEditCatchReturnKey(mDisableCodeLineEdit, this);
    hboxLayout->addWidget(mDisableCodeLineEdit);

    auto verifyButton = new QPushButton(i18n("Verify"), this);
    verifyButton->setObjectName(QStringLiteral("verifyButton"));
    hboxLayout->addWidget(verifyButton);
    verifyButton->setEnabled(false);
    connect(verifyButton, &QPushButton::clicked, this, &MyAccount2FaDisableTotpWidget::slotVerify);
    connect(mDisableCodeLineEdit, &QLineEdit::textChanged, this, [verifyButton](const QString &str) {
        verifyButton->setEnabled(!str.trimmed().isEmpty());
    });

    auto regenerateCode = new QPushButton(i18n("Regenerate Code"), this);
    regenerateCode->setObjectName(QStringLiteral("regenerateCode"));
    mainLayout->addWidget(regenerateCode);
    connect(regenerateCode, &QPushButton::clicked, this, &MyAccount2FaDisableTotpWidget::slotRegenerateCode);
    mainLayout->addStretch(1);
}

MyAccount2FaDisableTotpWidget::~MyAccount2FaDisableTotpWidget()
{
}

void MyAccount2FaDisableTotpWidget::slotVerify()
{
    mRocketChatAccount->ddp()->disable2fa(mDisableCodeLineEdit->text());
}

void MyAccount2FaDisableTotpWidget::slotRegenerateCode()
{
    mRocketChatAccount->ddp()->regenerateCodes2fa(mDisableCodeLineEdit->text());
}
