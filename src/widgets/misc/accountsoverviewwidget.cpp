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

#include <QAbstractButton>
#include <QHBoxLayout>
#include <QPainter>
#include <QPointer>
#include <QStyle>
#include <QStyleOption>

constexpr const double PAD = 0.2;

class AccountButton : public QAbstractButton
{
    struct UnreadAlert {
        int unread;
        bool alert;
    };

public:
    explicit AccountButton(QWidget *parent = nullptr)
        : QAbstractButton(parent)
        , mAccount(nullptr)
    {
        connect(Ruqola::self()->accountManager(), &AccountManager::currentAccountChanged, this, QOverload<>::of(&AccountButton::update));

        setMouseTracking(true);
        setFocusPolicy(Qt::NoFocus);
        setAttribute(Qt::WA_Hover);
    }

    void setAccount(RocketChatAccount *acct)
    {
        if (mAccount) {
            mAccount->disconnect(this);
            mAccount->roomModel()->disconnect(this);
            this->disconnect(acct);
        }

        mAccount = acct;

        if (mAccount) {
            auto updateFont = [this] {
                QFont f = font();
                f.setBold(currentUnreadAlert().alert);
                setFont(f);
                updateGeometry();
            };
            connect(acct, &RocketChatAccount::accountNameChanged, this, &AccountButton::updateGeometry);
            connect(acct, &RocketChatAccount::loginStatusChanged, this, &AccountButton::updateGeometry);
            connect(acct, &RocketChatAccount::loginStatusChanged, this, &AccountButton::updateTooltip);
            connect(acct->roomModel(), &RoomModel::needToUpdateNotification, this, updateFont);
            connect(this, &AccountButton::clicked, acct, [acct] {
                Ruqola::self()->accountManager()->setCurrentAccount(acct->accountName());
            });

            updateFont();
        }
        update();
    }

    QSize sizeHint() const override
    {
        const auto mngr = Ruqola::self()->accountManager();
        const bool singleAccount = mngr->rocketChatAccountModel()->rowCount() == 1;
        const double height = fontMetrics().height();
        const double padding = singleAccount ? 0 : height * PAD;
        const QSize textSize = fontMetrics().size(Qt::TextSingleLine, currentText());
        return {static_cast<int>(textSize.width() + padding * 2), static_cast<int>(height + padding * 2)};
    }

protected:
    void enterEvent(QEvent *event) override
    {
        if (isEnabled()) {
            update();
        }
        QAbstractButton::enterEvent(event);
    }

    void leaveEvent(QEvent *event) override
    {
        if (isEnabled()) {
            update();
        }
        QAbstractButton::leaveEvent(event);
    }

    void paintEvent(QPaintEvent *) override
    {
        if (!mAccount) {
            return;
        }

        QPainter p(this);

        const auto mngr = Ruqola::self()->accountManager();
        if (mngr->rocketChatAccountModel()->rowCount() > 1) {
            const bool isCurrent = mngr->currentAccount() == mAccount->accountName();

            QStyleOption opt;
            opt.init(this);
            if (isDown() || isCurrent) {
                opt.state |= QStyle::State_Sunken;
            }
            style()->drawPrimitive(QStyle::PE_PanelButtonTool, &opt, &p, this);
        }

        p.setPen(palette().color(QPalette::WindowText));
        p.setFont(font());
        p.drawText(rect(), Qt::AlignCenter, currentText());
    }

private:
    Q_REQUIRED_RESULT QString currentText() const
    {
        QString text = mAccount ? mAccount->displayName() : QString();
        if (text.isEmpty()) {
            text = i18n("(Unnamed)");
        }

        if (mAccount) {
            if (mAccount->loginStatus() != DDPAuthenticationManager::LoggedIn) {
                text += QStringLiteral(": %1").arg(currentLoginStatusText());
            } else if (int unread = currentUnreadAlert().unread) {
                text += QStringLiteral(" (%1)").arg(unread);
            }
        }

        return text;
    }

    Q_REQUIRED_RESULT UnreadAlert currentUnreadAlert() const
    {
        UnreadAlert ua = {0, false};
        mAccount->roomModel()->getUnreadAlertFromAccount(ua.alert, ua.unread);
        return ua;
    }

    Q_REQUIRED_RESULT QString currentLoginStatusText() const
    {
        if (mAccount) {
            if (!mAccount->ddp()->isConnected()) {
                return i18n("Not connected");
            }
            switch (mAccount->loginStatus()) {
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
        }
        return i18n("Unknown state");
    }

    void updateTooltip()
    {
        setToolTip(currentLoginStatusText());
    }

    QPointer<RocketChatAccount> mAccount;
};

AccountsOverviewWidget::AccountsOverviewWidget(QWidget *parent)
    : QWidget(parent)
{
    setLayout(new QHBoxLayout);
    const auto model = Ruqola::self()->accountManager()->rocketChatAccountModel();
    connect(model, &RocketChatAccountModel::accountNumberChanged, this, &AccountsOverviewWidget::updateButtons);
    updateButtons();
}

AccountsOverviewWidget::~AccountsOverviewWidget()
{
}

void AccountsOverviewWidget::updateButtons()
{
    const auto model = Ruqola::self()->accountManager()->rocketChatAccountModel();
    const auto count = model->rowCount();
    for (int i = 0; i < count; ++i) {
        RocketChatAccount *account = model->account(i);
        if (i >= mAccounts.size()) {
            auto button = new AccountButton(this);
            mAccounts.append(button);
            layout()->addWidget(mAccounts.constLast());
        }
        mAccounts[i]->setVisible(account->accountEnabled());
        mAccounts[i]->setAccount(model->account(i));
    }
    for (int i = count, total = mAccounts.size(); i < total; ++i) {
        mAccounts[i]->deleteLater();
    }
    mAccounts.resize(count);
}
