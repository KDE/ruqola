/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "asktwoauthenticationpasswordwidget.h"
using namespace Qt::Literals::StringLiterals;

#include "misc/twoauthenticationpasswordwidget.h"
#include <QVBoxLayout>

AskTwoAuthenticationPasswordWidget::AskTwoAuthenticationPasswordWidget(QWidget *parent)
    : QWidget(parent)
    , mTwoFactorAuthenticationPasswordLineEdit(new TwoAuthenticationPasswordWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
    mTwoFactorAuthenticationPasswordLineEdit->setObjectName(u"mTwoFactorAuthenticationPasswordLineEdit"_s);
    mainLayout->addWidget(mTwoFactorAuthenticationPasswordLineEdit);
    connect(mTwoFactorAuthenticationPasswordLineEdit,
            &TwoAuthenticationPasswordWidget::updateButtonOk,
            this,
            &AskTwoAuthenticationPasswordWidget::updateButtonOk);
}

AskTwoAuthenticationPasswordWidget::~AskTwoAuthenticationPasswordWidget() = default;

QString AskTwoAuthenticationPasswordWidget::code() const
{
    return mTwoFactorAuthenticationPasswordLineEdit->code();
}

RocketChatAccount *AskTwoAuthenticationPasswordWidget::rocketChatAccount() const
{
    return mTwoFactorAuthenticationPasswordLineEdit->rocketChatAccount();
}

void AskTwoAuthenticationPasswordWidget::setRocketChatAccount(RocketChatAccount *newRocketChatAccount)
{
    mTwoFactorAuthenticationPasswordLineEdit->setRocketChatAccount(newRocketChatAccount);
}

#include "moc_asktwoauthenticationpasswordwidget.cpp"
