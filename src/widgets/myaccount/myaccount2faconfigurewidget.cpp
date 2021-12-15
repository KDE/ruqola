/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccount2faconfigurewidget.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QVBoxLayout>

MyAccount2FaConfigureWidget::MyAccount2FaConfigureWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mActivate2FAViaEmailCheckbox(new QCheckBox(i18n("Activate Two Authentication Factor via Email"), this))
    , mActivate2FAViaTOTPCheckbox(new QCheckBox(i18n("Activate Two Authentication Factor via TOTP"), this))
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mActivate2FAViaEmailCheckbox->setObjectName(QStringLiteral("mActivate2FAViaEmailCheckbox"));
    mainLayout->addWidget(mActivate2FAViaEmailCheckbox);

    mActivate2FAViaTOTPCheckbox->setObjectName(QStringLiteral("mActivate2FAViaTOTPCheckbox"));
    mainLayout->addWidget(mActivate2FAViaTOTPCheckbox);

    mainLayout->addStretch(1);
}

MyAccount2FaConfigureWidget::~MyAccount2FaConfigureWidget() = default;

void MyAccount2FaConfigureWidget::load()
{
    if (mRocketChatAccount) {
        if (mRocketChatAccount->twoFactorAuthenticationByEmailEnabled()) {
            mActivate2FAViaEmailCheckbox->setChecked(mRocketChatAccount->ownUser().servicePassword().email2faEnabled());
        }
        if (mRocketChatAccount->twoFactorAuthenticationByTOTPEnabled()) {
            // TODO
        }
    }
}

void MyAccount2FaConfigureWidget::save()
{
    if (mRocketChatAccount) {
        // TODO verify it
        if (mRocketChatAccount->twoFactorAuthenticationByEmailEnabled()) {
            mRocketChatAccount->enable2FaEmailJob(mActivate2FAViaEmailCheckbox->isChecked());
        }
        if (mRocketChatAccount->twoFactorAuthenticationByTOTPEnabled()) {
            // TODO
        }
    }
}

void MyAccount2FaConfigureWidget::initialize()
{
    if (mRocketChatAccount) {
        mActivate2FAViaEmailCheckbox->setVisible(mRocketChatAccount->twoFactorAuthenticationByEmailEnabled());
        mActivate2FAViaTOTPCheckbox->setVisible(mRocketChatAccount->twoFactorAuthenticationByTOTPEnabled());
    }
}
