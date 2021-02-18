/*
 * Copyright 2020  Olivier de Gaalon <olivier.jg@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
{
    mTabBar->setShape(QTabBar::RoundedSouth);
    mTabBar->setObjectName(QStringLiteral("mTabBar"));

    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});
    mainLayout->addWidget(mTabBar);

    auto accountManager = Ruqola::self()->accountManager();
    const auto model = accountManager->rocketChatAccountModel();
    connect(model, &RocketChatAccountModel::accountNumberChanged, this, &AccountsOverviewWidget::updateButtons);
    updateButtons();

    connect(accountManager, &AccountManager::currentAccountChanged, this, &AccountsOverviewWidget::updateCurrentTab);

    connect(mTabBar, &QTabBar::currentChanged, this, [this](int i) {
        auto account = mTabBar->tabData(i).value<RocketChatAccount *>();
        Ruqola::self()->accountManager()->setCurrentAccount(account ? account->accountName() : QString());
    });
}

AccountsOverviewWidget::~AccountsOverviewWidget()
{
}

void AccountsOverviewWidget::updateButtons()
{
    const auto model = Ruqola::self()->accountManager()->rocketChatAccountModel();
    const auto count = model->rowCount();

    for (int i = 0; i < count; ++i) {
        if (i == mTabBar->count()) {
            mTabBar->addTab({});
        }

        auto account = model->account(i);
        disconnect(account, nullptr, this, nullptr);

        mTabBar->setTabData(i, QVariant::fromValue(account));
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
        mTabBar->setTabVisible(i, account->accountEnabled());
#else
        if (!account->accountEnabled()) {
            mTabBar->removeTab(i);
        }
#endif

        auto updateTabText = [this, i, account]() {
            mTabBar->setTabText(i, currentText(account));
        };
        auto updateTabToolTip = [this, i, account]() {
            mTabBar->setTabToolTip(i, currentLoginStatusText(account));
        };
        auto updateTabIcon = [this, i, account]() {
            mTabBar->setTabIcon(i, currentUnreadAlert(account).alert ? QIcon::fromTheme(QStringLiteral("message-new")) : QIcon());
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
    auto account = Ruqola::self()->accountManager()->account();
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
