/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccount2faconfigurewidget.h"
#include "ddpapi/ddpclient.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QGuiApplication>
#include <QLabel>
#include <QScreen>
#include <QVBoxLayout>
#include <prison/Prison>

MyAccount2FaConfigureWidget::MyAccount2FaConfigureWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mActivate2FAViaEmailCheckbox(new QCheckBox(i18n("Activate Two Authentication Factor via Email"), this))
    , mActivate2FAViaTOTPCheckbox(new QCheckBox(i18n("Activate Two Authentication Factor via TOTP"), this))
    , mRocketChatAccount(account)
    , mQRCode(Prison::createBarcode(Prison::QRCode))
    , mTotpQrCode(new QLabel(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mActivate2FAViaEmailCheckbox->setObjectName(QStringLiteral("mActivate2FAViaEmailCheckbox"));
    mainLayout->addWidget(mActivate2FAViaEmailCheckbox);

    mActivate2FAViaTOTPCheckbox->setObjectName(QStringLiteral("mActivate2FAViaTOTPCheckbox"));
    mainLayout->addWidget(mActivate2FAViaTOTPCheckbox);
    connect(mActivate2FAViaTOTPCheckbox, &QCheckBox::clicked, this, &MyAccount2FaConfigureWidget::slot2FAViaTOTPActivated);

    mTotpQrCode->setObjectName(QStringLiteral("mTotpQrCode"));
    mainLayout->addWidget(mTotpQrCode);
    mTotpQrCode->setVisible(false);

    mainLayout->addStretch(1);
    if (mRocketChatAccount) {
        connect(mRocketChatAccount, &RocketChatAccount::totpResult, this, &MyAccount2FaConfigureWidget::slotTotpResult);
    }
}

MyAccount2FaConfigureWidget::~MyAccount2FaConfigureWidget()
{
    delete mQRCode;
}

void MyAccount2FaConfigureWidget::slot2FAViaTOTPActivated(bool checked)
{
    if (checked) {
        mRocketChatAccount->ddp()->enable2fa();
    }
}

void MyAccount2FaConfigureWidget::slotTotpResult(const QString &secret, const QString &url)
{
    mQRCode->setData(url);
    mTotpQrCode->setVisible(true);
    mTotpQrCode->setPixmap(QPixmap::fromImage(mQRCode->toImage(mQRCode->preferredSize(QGuiApplication::primaryScreen()->devicePixelRatio()).toSize())));
}

void MyAccount2FaConfigureWidget::load()
{
    if (mRocketChatAccount) {
        if (mRocketChatAccount->twoFactorAuthenticationByEmailEnabled()) {
            mActivate2FAViaEmailCheckbox->setChecked(mRocketChatAccount->ownUser().servicePassword().email2faEnabled());
        }
        if (mRocketChatAccount->twoFactorAuthenticationByTOTPEnabled()) {
            mActivate2FAViaEmailCheckbox->setChecked(mRocketChatAccount->ownUser().servicePassword().totp());
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
