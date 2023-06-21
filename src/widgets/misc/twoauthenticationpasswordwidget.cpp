/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "twoauthenticationpasswordwidget.h"
#include "rocketchataccount.h"
#include <KAuthorized>
#include <KLocalizedString>
#include <KPasswordLineEdit>

#include <QHBoxLayout>
#include <QPushButton>

TwoAuthenticationPasswordWidget::TwoAuthenticationPasswordWidget(QWidget *parent)
    : QWidget(parent)
    , mTwoFactorAuthenticationPasswordLineEdit(new KPasswordLineEdit(this))
{
    auto twoFactorLayout = new QHBoxLayout(this);
    twoFactorLayout->setObjectName(QStringLiteral("twoFactorLayout"));
    twoFactorLayout->setContentsMargins({});

    mTwoFactorAuthenticationPasswordLineEdit->setObjectName(QStringLiteral("mTwoFactorAuthenticationPasswordLineEdit"));
    mTwoFactorAuthenticationPasswordLineEdit->lineEdit()->setPlaceholderText(i18n("Enter code"));
    mTwoFactorAuthenticationPasswordLineEdit->setRevealPasswordAvailable(KAuthorized::authorize(QStringLiteral("lineedit_reveal_password")));
    twoFactorLayout->addWidget(mTwoFactorAuthenticationPasswordLineEdit);

    auto sendNewEmailCode = new QPushButton(i18n("Send new code"), this);
    sendNewEmailCode->setObjectName(QStringLiteral("sendNewEmailCode"));
    twoFactorLayout->addWidget(sendNewEmailCode);
    connect(sendNewEmailCode, &QPushButton::clicked, this, &TwoAuthenticationPasswordWidget::slotSendNewEmailCode);
    connect(mTwoFactorAuthenticationPasswordLineEdit, &KPasswordLineEdit::passwordChanged, this, [this](const QString &password) {
        Q_EMIT updateButtonOk(!password.isEmpty());
    });
}

TwoAuthenticationPasswordWidget::~TwoAuthenticationPasswordWidget() = default;

QString TwoAuthenticationPasswordWidget::code() const
{
    return mTwoFactorAuthenticationPasswordLineEdit->lineEdit()->text();
}

void TwoAuthenticationPasswordWidget::clear()
{
    mTwoFactorAuthenticationPasswordLineEdit->lineEdit()->clear();
}

void TwoAuthenticationPasswordWidget::slotSendNewEmailCode()
{
    mRocketChatAccount->sendUserEmailCode();
}

RocketChatAccount *TwoAuthenticationPasswordWidget::rocketChatAccount() const
{
    return mRocketChatAccount;
}

void TwoAuthenticationPasswordWidget::setRocketChatAccount(RocketChatAccount *newRocketChatAccount)
{
    mRocketChatAccount = newRocketChatAccount;
}

#include "moc_twoauthenticationpasswordwidget.cpp"
