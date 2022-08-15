/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountconfigurewidget.h"
#include "managedevice/managedevicewidget.h"
#include "myaccount2e2configurewidget.h"
#include "myaccount2faconfigurewidget.h"
#include "myaccountpreferenceconfigurewidget.h"
#include "myaccountprofileconfigurewidget.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include <KLocalizedString>
#include <QTabWidget>
#include <QVBoxLayout>

MyAccountConfigureWidget::MyAccountConfigureWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mMyAccount2FaConfigureWidget(new MyAccount2FaConfigureWidget(account, this))
    , mMyAccount2ProfileConfigureWidget(new MyAccountProfileConfigureWidget(account, this))
    , mMyAccountPreferenceConfigureWidget(new MyAccountPreferenceConfigureWidget(account, this))
    , mMyAccount2e2ConfigureWidget(new MyAccount2e2ConfigureWidget(account, this))
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    auto tabWidget = new QTabWidget(this);
    tabWidget->setObjectName(QStringLiteral("tabWidget"));
    mainLayout->addWidget(tabWidget);

    mMyAccount2ProfileConfigureWidget->setObjectName(QStringLiteral("mMyAccount2ProfileConfigureWidget"));
    const int profileIndexPage = tabWidget->addTab(mMyAccount2ProfileConfigureWidget, i18n("Profile"));
    if (account && !account->allowProfileChange()) {
        tabWidget->setTabVisible(profileIndexPage, false);
    }

    mMyAccountPreferenceConfigureWidget->setObjectName(QStringLiteral("mMyAccountPreferenceConfigureWidget"));
    tabWidget->addTab(mMyAccountPreferenceConfigureWidget, i18n("Preference"));

    mMyAccount2FaConfigureWidget->setObjectName(QStringLiteral("mMyAccount2FaConfigureWidget"));
    const int index2faPage = tabWidget->addTab(mMyAccount2FaConfigureWidget, i18n("Two Authentication Factor"));
    if (account && !account->twoFactorAuthenticationEnabled()) {
        tabWidget->setTabVisible(index2faPage, false);
    }

    mMyAccount2e2ConfigureWidget->setObjectName(QStringLiteral("mMyAccount2e2ConfigureWidget"));
    const int index2e2Page = tabWidget->addTab(mMyAccount2e2ConfigureWidget, i18n("E2E Encryption"));
    if (account && !account->encryptionEnabled()) {
        tabWidget->setTabVisible(index2e2Page, false);
    }
    if (account && account->hasLicense(QStringLiteral("device-management"))) {
        mManageDeviceWidget = new ManageDeviceWidget(mRocketChatAccount, this);
        tabWidget->addTab(mManageDeviceWidget, i18n("Manage Device"));
    }
}

MyAccountConfigureWidget::~MyAccountConfigureWidget() = default;

void MyAccountConfigureWidget::save()
{
    mMyAccountPreferenceConfigureWidget->save();
    if (mRocketChatAccount) {
        if (mRocketChatAccount->allowProfileChange()) {
            mMyAccount2ProfileConfigureWidget->save();
        }
        if (mRocketChatAccount->twoFactorAuthenticationEnabled()) {
            mMyAccount2FaConfigureWidget->save();
        }
        if (mRocketChatAccount->encryptionEnabled()) {
            mMyAccount2e2ConfigureWidget->save();
        }
    }
}

void MyAccountConfigureWidget::load()
{
    mMyAccountPreferenceConfigureWidget->load();
    if (mRocketChatAccount) {
        if (mRocketChatAccount->allowProfileChange()) {
            mMyAccount2ProfileConfigureWidget->load();
        }
        if (mRocketChatAccount->twoFactorAuthenticationEnabled()) {
            mMyAccount2FaConfigureWidget->load();
        }
        if (mRocketChatAccount->encryptionEnabled()) {
            mMyAccount2e2ConfigureWidget->load();
        }
    }
}

void MyAccountConfigureWidget::initialize()
{
    mMyAccount2ProfileConfigureWidget->initialize();
    mMyAccount2FaConfigureWidget->initialize();
}
