/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccount2fadisabletotpwidget.h"
#include "ddpapi/ddpclient.h"
#include "rocketchataccount.h"
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <KMessageBox>
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

    auto label = new QLabel(i18nc("@label:textbox", "Open your authentication app and enter the code.\nYou can also use one of your backup codes."), this);
    label->setObjectName(QStringLiteral("label"));
    mainLayout->addWidget(label);

    auto hboxLayout = new QHBoxLayout;
    hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
    hboxLayout->setContentsMargins({});
    mainLayout->addLayout(hboxLayout);

    mDisableCodeLineEdit->setObjectName(QStringLiteral("mDisableCodeLineEdit"));
    mDisableCodeLineEdit->setPlaceholderText(i18nc("@info:placeholder", "Enter authentication code"));
    KLineEditEventHandler::catchReturnKey(mDisableCodeLineEdit);
    hboxLayout->addWidget(mDisableCodeLineEdit);

    auto verifyButton = new QPushButton(i18nc("@action:button", "Verify"), this);
    verifyButton->setObjectName(QStringLiteral("verifyButton"));
    hboxLayout->addWidget(verifyButton);
    verifyButton->setEnabled(false);
    connect(verifyButton, &QPushButton::clicked, this, &MyAccount2FaDisableTotpWidget::slotVerify);
    connect(mDisableCodeLineEdit, &QLineEdit::textChanged, this, [verifyButton](const QString &str) {
        verifyButton->setEnabled(!str.trimmed().isEmpty());
    });

    auto regenerateCode = new QPushButton(i18nc("@action:button", "Regenerate Code"), this);
    regenerateCode->setObjectName(QStringLiteral("regenerateCode"));
    mainLayout->addWidget(regenerateCode);
    connect(regenerateCode, &QPushButton::clicked, this, &MyAccount2FaDisableTotpWidget::slotRegenerateCode);
    mainLayout->addStretch(1);
    if (mRocketChatAccount) {
        connect(mRocketChatAccount, &RocketChatAccount::disabledTotpValid, this, &MyAccount2FaDisableTotpWidget::slotTotpInvalid);
    }
}

MyAccount2FaDisableTotpWidget::~MyAccount2FaDisableTotpWidget() = default;

void MyAccount2FaDisableTotpWidget::slotTotpInvalid(bool check)
{
    if (check) {
        Q_EMIT hide2FaDisableTotpWidget();
    } else {
        KMessageBox::error(this, i18n("Invalid two factor code."), i18nc("@title:window", "Check Two Factor Code"));
    }
}

void MyAccount2FaDisableTotpWidget::slotVerify()
{
    mRocketChatAccount->ddp()->disable2fa(mDisableCodeLineEdit->text());
}

void MyAccount2FaDisableTotpWidget::slotRegenerateCode()
{
    mRocketChatAccount->ddp()->regenerateCodes2fa(mDisableCodeLineEdit->text());
}

#include "moc_myaccount2fadisabletotpwidget.cpp"
