/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccount2faconfigurewidget.h"
#include "ddpapi/ddpclient.h"
#include "myaccount2fadisabletotpwidget.h"
#include "myaccount2fatotpwidget.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QStackedWidget>
#include <QVBoxLayout>

MyAccount2FaConfigureWidget::MyAccount2FaConfigureWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mActivate2FAViaEmailCheckbox(new QCheckBox(i18n("Activate Two Authentication Factor via Email"), this))
    , mActivate2FAViaTOTPCheckbox(new QCheckBox(i18n("Activate Two Authentication Factor via TOTP"), this))
    , mRocketChatAccount(account)
    , mMyAccount2FaTotpWidget(new MyAccount2FaTotpWidget(account, this))
    , mMyAccountDisable2FaTotpWidget(new MyAccount2FaDisableTotpWidget(account, this))
    , mMyAccount2FaEmpty(new QWidget(this))
    , mStackedWidget(new QStackedWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mActivate2FAViaEmailCheckbox->setObjectName(QStringLiteral("mActivate2FAViaEmailCheckbox"));
    mainLayout->addWidget(mActivate2FAViaEmailCheckbox);

    mActivate2FAViaTOTPCheckbox->setObjectName(QStringLiteral("mActivate2FAViaTOTPCheckbox"));
    mainLayout->addWidget(mActivate2FAViaTOTPCheckbox);
    connect(mActivate2FAViaTOTPCheckbox, &QCheckBox::clicked, this, &MyAccount2FaConfigureWidget::slot2FAViaTOTPActivated);

    mStackedWidget->setObjectName(QStringLiteral("mStackedWidget"));
    mainLayout->addWidget(mStackedWidget);

    mMyAccountDisable2FaTotpWidget->setObjectName(QStringLiteral("mMyAccountDisable2FaTotpWidget"));
    mMyAccount2FaTotpWidget->setObjectName(QStringLiteral("mMyAccount2FaTotpWidget"));
    mMyAccount2FaEmpty->setObjectName(QStringLiteral("mMyAccount2FaEmpty"));

    connect(mMyAccount2FaTotpWidget, &MyAccount2FaTotpWidget::show2FaEnabledWidget, this, [this]() {
        mStackedWidget->setCurrentIndex(Enable2FaPage);
    });

    connect(mMyAccountDisable2FaTotpWidget, &MyAccount2FaDisableTotpWidget::hide2FaDisableTotpWidget, this, [this]() {
        mStackedWidget->setCurrentIndex(EmptyPage);
    });

    mStackedWidget->insertWidget(EmptyPage, mMyAccount2FaEmpty);
    mStackedWidget->insertWidget(Enable2FaPage, mMyAccount2FaTotpWidget);
    mStackedWidget->insertWidget(Disable2FaPage, mMyAccountDisable2FaTotpWidget);
    mStackedWidget->setCurrentIndex(EmptyPage);

    mainLayout->addStretch(1);
}

MyAccount2FaConfigureWidget::~MyAccount2FaConfigureWidget() = default;

void MyAccount2FaConfigureWidget::slot2FAViaTOTPActivated(bool checked)
{
    if (checked) {
        if (!mRocketChatAccount->ownUser().servicePassword().totp()) {
            mRocketChatAccount->ddp()->enable2fa();
        }
    } else {
        if (mRocketChatAccount->ownUser().servicePassword().totp()) {
            mStackedWidget->setCurrentIndex(Disable2FaPage);
        }
    }
}

void MyAccount2FaConfigureWidget::load()
{
    if (mRocketChatAccount) {
        if (mRocketChatAccount->twoFactorAuthenticationByEmailEnabled()) {
            mActivate2FAViaEmailCheckbox->setChecked(mRocketChatAccount->ownUser().servicePassword().email2faEnabled());
        }
        if (mRocketChatAccount->twoFactorAuthenticationByTOTPEnabled()) {
            mActivate2FAViaTOTPCheckbox->setChecked(mRocketChatAccount->ownUser().servicePassword().totp());
        }
    }
}

void MyAccount2FaConfigureWidget::save()
{
    if (mRocketChatAccount) {
        if (mRocketChatAccount->twoFactorAuthenticationByEmailEnabled()) {
            mRocketChatAccount->enable2FaEmailJob(mActivate2FAViaEmailCheckbox->isChecked());
        }
        // Not necessary
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

#include "moc_myaccount2faconfigurewidget.cpp"
