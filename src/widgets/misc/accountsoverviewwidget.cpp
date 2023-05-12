/*
 * SPDX-FileCopyrightText: 2020 Olivier de Gaalon <olivier.jg@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#include "accountsoverviewwidget.h"

#include "accountmanager.h"
#include "model/rocketchataccountmodel.h"
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

Q_REQUIRED_RESULT QString currentLoginStatusText(RocketChatAccount *account)
{
    if (!account->ddp()->isConnected()) {
        return i18n("Not connected");
    }
    switch (account->loginStatus()) {
    case DDPAuthenticationManager::Connecting:
        return i18n("Connecting");
    case DDPAuthenticationManager::LoginOtpAuthOngoing:
        return i18n("Login OTP code required");
    case DDPAuthenticationManager::LoginFailedInvalidUserOrPassword:
        return i18n("Login failed: invalid username or password");
    case DDPAuthenticationManager::LoginOngoing:
        return i18n("Logging in");
    case DDPAuthenticationManager::LoggedIn:
        return i18n("Logged in");
    case DDPAuthenticationManager::LoggedOut:
        return i18n("Logged out");
    case DDPAuthenticationManager::FailedToLoginPluginProblem:
        return i18n("Failed to login due to plugin problem");
    case DDPAuthenticationManager::GenericError:
        return i18n("Login failed: generic error");
    case DDPAuthenticationManager::LoginOtpRequired:
        return i18n("A one-time password is required to complete the login procedure.");
    case DDPAuthenticationManager::LoginFailedInvalidOtp:
        return i18n("Login failed: Invalid OTP code.");
    case DDPAuthenticationManager::LoginFailedUserNotActivated:
        return i18n("Login failed: User is not activated.");
    case DDPAuthenticationManager::LogoutOngoing:
    case DDPAuthenticationManager::LogoutCleanUpOngoing:
    case DDPAuthenticationManager::LoggedOutAndCleanedUp:
        break;
    }
    return i18n("Unknown state");
}

Q_REQUIRED_RESULT UnreadAlert currentUnreadAlert(RocketChatAccount *account)
{
    UnreadAlert ua;
    account->roomModel()->getUnreadAlertFromAccount(ua.alert, ua.unread);
    return ua;
}

Q_REQUIRED_RESULT QString currentText(RocketChatAccount *account)
{
    QString text = account->displayName();
    if (text.isEmpty()) {
        text = i18n("(Unnamed)");
    }

    if (account->loginStatus() != DDPAuthenticationManager::LoggedIn) {
        text += QStringLiteral(": %1").arg(currentLoginStatusText(account));
    } else if (int unread = currentUnreadAlert(account).unread) {
        text += QStringLiteral(" (%1)").arg(unread);
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

        mTabBar->setTabData(i, QVariant::fromValue(account));
        mTabBar->setTabVisible(i, account->accountEnabled());

        auto updateTabText = [this, i, account]() {
            mTabBar->setTabText(i, currentText(account));
        };
        auto updateTabToolTip = [this, i, account]() {
            mTabBar->setTabToolTip(i, currentLoginStatusText(account));
        };
        auto updateTabIcon = [this, i, account]() {
            QIcon icon;
            if (currentUnreadAlert(account).alert) {
                icon = QIcon::fromTheme(QStringLiteral("message-new"));
            } else {
                // qDebug() << " FAVICON URL " << account->urlForLink(account->ruqolaServerConfig()->faviconUrl()) << " ACCOUNTNAME " << account->accountName();
                const QString iconFaviconUrl{account->attachmentUrlFromLocalCache(account->ruqolaServerConfig()->faviconUrl()).toLocalFile()};
                if (!iconFaviconUrl.isEmpty()) {
                    const QIcon iconFavicon{iconFaviconUrl};
                    if (!iconFavicon.isNull()) {
                        icon = std::move(iconFavicon);
                    }
                }
            }
            mTabBar->setTabIcon(i, icon);
        };
        connect(account, &RocketChatAccount::accountNameChanged, this, updateTabText);
        connect(account, &RocketChatAccount::loginStatusChanged, this, [=]() {
            updateTabText();
            updateTabToolTip();
        });
        connect(account->roomModel(), &RoomModel::needToUpdateNotification, this, [=]() {
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
