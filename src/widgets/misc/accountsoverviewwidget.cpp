/*
 * SPDX-FileCopyrightText: 2020 Olivier de Gaalon <olivier.jg@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#include "accountsoverviewwidget.h"
using namespace Qt::Literals::StringLiterals;

#include "accountmanager.h"
#include "colorsandmessageviewstyle.h"
#include "ddpapi/ddpclient.h"
#include "model/rocketchataccountfilterproxymodel.h"
#include "model/rocketchataccountmodel.h"
#include "model/roommodel.h"
#include "rocketchataccount.h"
#include "rocketchataccountsettings.h"
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
    bool mentions = false;
};

[[nodiscard]] UnreadAlert currentUnreadAlert(RocketChatAccount *account)
{
    UnreadAlert ua;
    account->roomModel()->getUnreadAlertFromAccount(ua.alert, ua.unread, ua.mentions);
    return ua;
}

[[nodiscard]] QString currentText(RocketChatAccount *account)
{
    QString text = account->settings()->displayName();
    if (text.isEmpty()) {
        text = i18n("(Unnamed)");
    }

    if (account->loginStatus() == AuthenticationManager::LoggedIn) {
        auto unreadAlert = currentUnreadAlert(account);
        if (int unread = unreadAlert.unread) {
            text += u" (%1)"_s.arg(unread);
        }
        if (unreadAlert.mentions) {
            text += u" @"_s;
        }
    }

    return text;
}

[[nodiscard]] QColor currentTextColor(RocketChatAccount *account)
{
    auto colorScheme = ColorsAndMessageViewStyle::self().schemeView();
    if (account->loginStatus() != AuthenticationManager::LoggedIn) {
        return colorScheme.foreground(KColorScheme::InactiveText).color();
    }
    auto unreadAlertAndMentions = currentUnreadAlert(account);
    if (unreadAlertAndMentions.mentions) {
        return colorScheme.foreground(KColorScheme::NegativeText).color();
    }
    if (unreadAlertAndMentions.unread) {
        return colorScheme.foreground(KColorScheme::NeutralText).color();
    }
    if (unreadAlertAndMentions.alert) {
        return colorScheme.foreground(KColorScheme::ActiveText).color();
    }
    return colorScheme.foreground(KColorScheme::NormalText).color();
}
}

AccountsOverviewWidget::AccountsOverviewWidget(QWidget *parent)
    : QWidget(parent)
    , mTabBar(new QTabBar(this))
    , mAccountManager(Ruqola::self()->accountManager())
{
    mTabBar->setShape(QTabBar::RoundedSouth);
    mTabBar->setObjectName(u"mTabBar"_s);

    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
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
    connect(&ColorsAndMessageViewStyle::self(), &ColorsAndMessageViewStyle::needToUpdateColors, this, &AccountsOverviewWidget::updateButtons);
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
            if (account->accountEnabled()) {
                mTabBar->setTabText(i, currentText(account));
                mTabBar->setTabTextColor(i, currentTextColor(account));
            }
        };
        auto updateTabToolTip = [this, i, account]() {
            if (account->accountEnabled())
                mTabBar->setTabToolTip(i, account->loginStatusText());
        };
        auto updateTabIcon = [this, i, account]() {
            QIcon icon;
            if (account->accountEnabled() && account->loginStatus() == AuthenticationManager::LoggedIn) {
                if (currentUnreadAlert(account).alert) {
                    icon = QIcon::fromTheme(u"message-new"_s);
                } else {
                    icon = Utils::iconFromAccount(account);
                }
            } else {
                icon = QIcon::fromTheme(u"data-error"_s);
            }
            mTabBar->setTabIcon(i, icon);
        };
        connect(account, &RocketChatAccount::accountNameChanged, this, updateTabText);
        connect(account, &RocketChatAccount::loginStatusChanged, this, [updateTabText, updateTabToolTip]() {
            updateTabText();
            updateTabToolTip();
        });
        connect(account, &RocketChatAccount::ddpLoginStatusChanged, this, [updateTabText, updateTabToolTip]() {
            updateTabText();
            updateTabToolTip();
        });
        connect(account, &RocketChatAccount::ddpConnectedChanged, this, [updateTabToolTip]() {
            updateTabToolTip();
        });
        connect(account->roomModel(), &RoomModel::needToUpdateNotification, this, [updateTabText, updateTabIcon]() {
            updateTabText();
            updateTabIcon();
        });
        connect(account->roomModel(), &RoomModel::roomNeedAttention, this, [=]() {
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
