/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountconfigurewidget.h"
#include "myaccount2e2configurewidget.h"
#include "myaccount2faconfigurewidget.h"
#include "myaccountmanagedeviceconfigurewidget.h"
#include "myaccountpersonalaccesstokenconfigurewidget.h"
#include "myaccountpreferenceconfigurewidget.h"
#include "myaccountprofileconfigurewidget.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <QTabWidget>
#include <QVBoxLayout>

MyAccountConfigureWidget::MyAccountConfigureWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mMyAccount2FaConfigureWidget(new MyAccount2FaConfigureWidget(account, this))
    , mMyAccount2ProfileConfigureWidget(new MyAccountProfileConfigureWidget(account, this))
    , mMyAccountPreferenceConfigureWidget(new MyAccountPreferenceConfigureWidget(account, this))
    , mMyAccount2e2ConfigureWidget(new MyAccount2e2ConfigureWidget(account, this))
    , mMyAccountPersonalAccessTokenConfigureWidget(new MyAccountPersonalAccessTokenConfigureWidget(account, this))
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
    if (account && !account->ruqolaServerConfig()->allowProfileChange()) {
        tabWidget->setTabVisible(profileIndexPage, false);
    }

    mMyAccountPreferenceConfigureWidget->setObjectName(QStringLiteral("mMyAccountPreferenceConfigureWidget"));
    tabWidget->addTab(mMyAccountPreferenceConfigureWidget, i18n("Preference"));

    mMyAccount2FaConfigureWidget->setObjectName(QStringLiteral("mMyAccount2FaConfigureWidget"));
    const int index2faPage = tabWidget->addTab(mMyAccount2FaConfigureWidget, i18n("Two Authentication Factor"));
    if (account && !account->ruqolaServerConfig()->twoFactorAuthenticationEnabled()) {
        tabWidget->setTabVisible(index2faPage, false);
    }

    mMyAccount2e2ConfigureWidget->setObjectName(QStringLiteral("mMyAccount2e2ConfigureWidget"));
    const int index2e2Page = tabWidget->addTab(mMyAccount2e2ConfigureWidget, i18n("E2E Encryption"));
    if (account && !account->ruqolaServerConfig()->encryptionEnabled()) {
        tabWidget->setTabVisible(index2e2Page, false);
    }
    if (account && account->hasLicense(QStringLiteral("device-management"))) {
        auto manageDeviceWidget = new MyAccountManageDeviceConfigureWidget(mRocketChatAccount, this);
        tabWidget->addTab(manageDeviceWidget, i18n("Manage Device"));
        manageDeviceWidget->initialize();
    }
    mMyAccountPersonalAccessTokenConfigureWidget->setObjectName(QStringLiteral("mMyAccountPersonalAccessTokenConfigureWidget"));
    const int pageIndex = tabWidget->addTab(mMyAccountPersonalAccessTokenConfigureWidget, i18n("Personal Access Token"));
    if (account && !account->hasPermission(QStringLiteral("create-personal-access-tokens"))) {
        tabWidget->setTabVisible(pageIndex, false);
    }
}

MyAccountConfigureWidget::~MyAccountConfigureWidget() = default;

void MyAccountConfigureWidget::save()
{
    mMyAccountPreferenceConfigureWidget->save();
    if (mRocketChatAccount) {
        if (mRocketChatAccount->ruqolaServerConfig()->allowProfileChange()) {
            mMyAccount2ProfileConfigureWidget->save();
        }
        if (mRocketChatAccount->ruqolaServerConfig()->twoFactorAuthenticationEnabled()) {
            mMyAccount2FaConfigureWidget->save();
        }
    }
}

void MyAccountConfigureWidget::load()
{
    mMyAccountPreferenceConfigureWidget->load();
    if (mRocketChatAccount) {
        if (mRocketChatAccount->ruqolaServerConfig()->allowProfileChange()) {
            mMyAccount2ProfileConfigureWidget->load();
        }
        if (mRocketChatAccount->ruqolaServerConfig()->twoFactorAuthenticationEnabled()) {
            mMyAccount2FaConfigureWidget->load();
        }
    }
}

void MyAccountConfigureWidget::initialize()
{
    mMyAccount2ProfileConfigureWidget->initialize();
    mMyAccount2FaConfigureWidget->initialize();
    if (mRocketChatAccount && mRocketChatAccount->hasPermission(QStringLiteral("create-personal-access-tokens"))) {
        mMyAccountPersonalAccessTokenConfigureWidget->initialize();
    }
}

#include "moc_myaccountconfigurewidget.cpp"
