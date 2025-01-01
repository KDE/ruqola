/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "twoauthenticationpasswordwidget.h"
#include "2fa/user2fasendemailcodejob.h"
#include "connection.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"

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
    mTwoFactorAuthenticationPasswordLineEdit->lineEdit()->setPlaceholderText(i18nc("@info:placeholder", "Enter code"));
    mTwoFactorAuthenticationPasswordLineEdit->setRevealPasswordMode(
        KAuthorized::authorize(QStringLiteral("lineedit_reveal_password")) ? KPassword::RevealMode::OnlyNew : KPassword::RevealMode::Never);
    twoFactorLayout->addWidget(mTwoFactorAuthenticationPasswordLineEdit);

    auto sendNewEmailCode = new QPushButton(i18nc("@action:button", "Send new code"), this);
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
    auto job = new RocketChatRestApi::User2FASendEmailCodeJob(this);
    job->setUsernameOrEmail(mRocketChatAccount->userName());
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    if (!job->start()) {
        qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start User2FASendEmailCodeJob";
    }
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
