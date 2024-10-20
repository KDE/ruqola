/*
 * SPDX-FileCopyrightText: 2020 Olivier de Gaalon <olivier.jg@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#include "accountsoverviewwidget.h"

#include "accountmanager.h"
#include "ddpapi/ddpclient.h"
#include "model/rocketchataccountfilterproxymodel.h"
#include "model/rocketchataccountmodel.h"
#include "model/roommodel.h"
#include "rocketchataccount.h"
#include "ruqola.h"

#include <KLocalizedString>

#include <QHBoxLayout>
#include <QIcon>
#include <QTabBar>

namespace
{
struct UnreadAlert {
    int unread = 0;
    bool alert = false;
};

[[nodiscard]] QString currentLoginStatusText(RocketChatAccount *account)
{
    if (!account->ddp()->isConnected()) {
        return i18n("Not connected");
    }
    switch (account->loginStatus()) {
    case AuthenticationManager::Connecting:
        return i18n("Connecting");
    case AuthenticationManager::LoginOtpAuthOngoing:
        return i18n("Login OTP code required");
    case AuthenticationManager::LoginFailedInvalidUserOrPassword:
        return i18n("Login failed: invalid username or password");
    case AuthenticationManager::LoginOngoing:
        return i18n("Logging in");
    case AuthenticationManager::LoggedIn:
        return i18n("Logged in");
    case AuthenticationManager::LoggedOut:
        return i18n("Logged out");
    case AuthenticationManager::FailedToLoginPluginProblem:
        return i18n("Failed to login due to plugin problem");
    case AuthenticationManager::GenericError:
        return i18n("Login failed: generic error");
    case AuthenticationManager::LoginOtpRequired:
        return i18n("A one-time password is required to complete the login procedure.");
    case AuthenticationManager::LoginFailedInvalidOtp:
        return i18n("Login failed: Invalid OTP code.");
    case AuthenticationManager::LoginFailedUserNotActivated:
        return i18n("Login failed: User is not activated.");
    case AuthenticationManager::LoginFailedLoginBlockForIp:
        return i18n("Login has been temporarily blocked For IP.");
    case AuthenticationManager::LoginFailedLoginBlockedForUser:
        return i18n("Login has been temporarily blocked For User.");
    case AuthenticationManager::LoginFailedLoginAppNotAllowedToLogin:
        return i18n("App user is not allowed to login.");
    case AuthenticationManager::LogoutOngoing:
    case AuthenticationManager::LogoutCleanUpOngoing:
    case AuthenticationManager::LoggedOutAndCleanedUp:
        break;
    }
    return i18n("Unknown state");
}

[[nodiscard]] UnreadAlert currentUnreadAlert(RocketChatAccount *account)
{
    UnreadAlert ua;
    account->roomModel()->getUnreadAlertFromAccount(ua.alert, ua.unread);
    return ua;
}

[[nodiscard]] QString currentText(RocketChatAccount *account)
{
    QString text = account->displayName();
    if (text.isEmpty()) {
        text = i18n("(Unnamed)");
    }

    if (account->loginStatus() == AuthenticationManager::LoggedIn) {
        if (int unread = currentUnreadAlert(account).unread) {
            text += QStringLiteral(" (%1)").arg(unread);
        }
    }

    return text;
}
}

AccountsOverviewWidget::AccountsOverviewWidget(QWidget *parent)
    : QWidget(parent)
    , mTabBar(new QTabBar(this))
    , mAccountManager(Ruqola::self()->accountManager())
{
    mTabBar->setShape(QTabBar::RoundedSouth);
    mTabBar->setObjectName(QStringLiteral("mTabBar"));

    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});
    mainLayout->addWidget(mTabBar);

    const auto model = mAccountManager->rocketChatAccountProxyModel();
    connect(model, &QAbstractItemModel::modelReset, this, &AccountsOverviewWidget::updateButtons);
    connect(model, &QAbstractItemModel::rowsInserted, this, &AccountsOverviewWidget::updateButtons);
    connect(model, &QAbstractItemModel::rowsRemoved, this, &AccountsOverviewWidget::updateButtons);
    connect(model, &QAbstractItemModel::rowsMoved, this, &AccountsOverviewWidget::updateButtons);
    updateButtons();

    connect(mAccountManager, &AccountManager::currentAccountChanged, this, &AccountsOverviewWidget::updateCurrentTab);

    connect(mTabBar, &QTabBar::currentChanged, this, [this](int i) {
        auto account = mTabBar->tabData(i).value<RocketChatAccount *>();
        mAccountManager->setCurrentAccount(account ? account->accountName() : QString());
    });
}

AccountsOverviewWidget::~AccountsOverviewWidget() = default;

void AccountsOverviewWidget::updateButtons()
{
    const auto model = mAccountManager->rocketChatAccountProxyModel();
    const auto count = model->rowCount();

    for (int i = 0; i < count; ++i) {
        if (i == mTabBar->count()) {
            mTabBar->addTab({});
        }

        auto index = model->index(i, 0);
        auto account = index.data(RocketChatAccountModel::Account).value<RocketChatAccount *>();
        disconnect(account, nullptr, this, nullptr);
        disconnect(account->roomModel(), nullptr, this, nullptr);

        mTabBar->setTabData(i, QVariant::fromValue(account));
        mTabBar->setTabVisible(i, account->accountEnabled());

        auto updateTabText = [this, i, account]() {
            if (account->accountEnabled())
                mTabBar->setTabText(i, currentText(account));
        };
        auto updateTabToolTip = [this, i, account]() {
            if (account->accountEnabled())
                mTabBar->setTabToolTip(i, currentLoginStatusText(account));
        };
        auto updateTabIcon = [this, i, account]() {
            QIcon icon;
            if (account->accountEnabled() && account->loginStatus() == AuthenticationManager::LoggedIn) {
                if (currentUnreadAlert(account).alert) {
                    icon = QIcon::fromTheme(QStringLiteral("message-new"));
                } else {
                    icon = Utils::iconFromAccount(account);
                }
            } else {
                icon = QIcon::fromTheme(QStringLiteral("data-error"));
            }
            mTabBar->setTabIcon(i, icon);
        };
        connect(account, &RocketChatAccount::accountNameChanged, this, updateTabText);
        connect(account, &RocketChatAccount::loginStatusChanged, this, [updateTabText, updateTabToolTip]() {
            updateTabText();
            updateTabToolTip();
        });
        connect(account->roomModel(), &RoomModel::needToUpdateNotification, this, [updateTabText, updateTabIcon]() {
            updateTabText();
            updateTabIcon();
        });

        updateTabText();
        updateTabToolTip();
        updateTabIcon();
    }

    while (mTabBar->count() > count) {
        mTabBar->removeTab(count);
    }

    updateCurrentTab();
}

void AccountsOverviewWidget::updateCurrentTab()
{
    auto account = mAccountManager->account();
    auto tabIndex = [this, account]() {
        for (int i = 0, c = mTabBar->count(); i < c; ++i) {
            if (mTabBar->tabData(i).value<RocketChatAccount *>() == account) {
                return i;
            }
        }
        return -1;
    };
    mTabBar->setCurrentIndex(tabIndex());
}

void AccountsOverviewWidget::showNextView()
{
    goToView(mTabBar->currentIndex() + 1);
}

void AccountsOverviewWidget::showPreviousView()
{
    goToView(mTabBar->currentIndex() - 1);
}

void AccountsOverviewWidget::goToView(int index)
{
    if (index >= 0 && index < mTabBar->count()) {
        auto rocketChatAccount = mTabBar->tabData(index).value<RocketChatAccount *>();
        if (rocketChatAccount) {
            mAccountManager->setCurrentAccount(rocketChatAccount->accountName());
        }
    }
}

#include "moc_accountsoverviewwidget.cpp"
