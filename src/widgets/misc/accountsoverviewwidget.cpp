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
                mTabBar->setTabToolTip(i, account->loginStatusText());
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
