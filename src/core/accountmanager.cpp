/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "accountmanager.h"
#include "localdatabase/localdatabaseutils.h"
#include "managerdatapaths.h"
#include "notificationhistorymanager.h"
#include "notifications/notifierjob.h"
#include "parsemessageurlutils.h"
#include "rocketchataccount.h"
#include "ruqola_debug.h"
#include "ruqolaglobalconfig.h"
#include <KLocalizedString>
#include <QDir>
#include <QDirIterator>
#include <QSettings>

AccountManager::AccountManager(QObject *parent)
    : QObject(parent)
    , mRocketChatAccountModel(new RocketChatAccountModel(this))
    , mRocketChatAccountProxyModel(new RocketChatAccountFilterProxyModel(this))
{
    mRocketChatAccountProxyModel->setSourceModel(mRocketChatAccountModel);
    loadAccount();
}

AccountManager::~AccountManager() = default;

int AccountManager::accountNumber() const
{
    return mRocketChatAccountModel->accountNumber();
}

bool AccountManager::showMessage(const ParseMessageUrlUtils &parseUrl)
{
    auto account = mRocketChatAccountModel->accountFromServerUrl(parseUrl.serverHost());
    if (account) {
        // const QString path{parseUrl.path()};
        const QString messageId = parseUrl.messageId();
        qCDebug(RUQOLA_LOG) << " parseUrl " << parseUrl;
        // https://<server url>/channel/python?msg=sn3gEQom7NcLxTg5h
        setCurrentAccount(account->accountName());
        // qDebug() << " account->accountName() : " << account->accountName();
        Q_EMIT mCurrentAccount->raiseWindow();
        Q_EMIT mCurrentAccount->selectChannelAndMessage(messageId, parseUrl.roomId(), parseUrl.roomIdType(), parseUrl.channelType());
        return true;
    }
    return false;
}

void AccountManager::openMessageUrl(const QString &messageUrl)
{
    ParseMessageUrlUtils parseUrl;
    if (parseUrl.parseUrl(messageUrl)) {
        if (showMessage(parseUrl)) {
            return;
        }
    }

    Q_EMIT messageUrlNotFound(i18n("Server not found: %1", messageUrl));
    // TODO report error
}

void AccountManager::connectToAccount(RocketChatAccount *account)
{
    connect(account, &RocketChatAccount::notification, this, [this, account](const NotificationInfo &info) {
        NotificationHistoryManager::self()->addNotification(info);

        switch (info.notificationType()) {
        case NotificationInfo::StandardMessage: {
            auto job = new NotifierJob;
            job->setInfo(info);
            connect(job, &NotifierJob::switchToAccountAndRoomName, this, &AccountManager::slotSwitchToAccountAndRoomName);
            connect(job, &NotifierJob::sendReply, this, [account](const QString &str, const QString &roomId, const QString &tmId) {
                if (tmId.isEmpty()) {
                    account->sendMessage(roomId, str);
                } else {
                    account->replyOnThread(roomId, tmId, str);
                }
                // qDebug() << " str" << str << " Room Name " << roomName;
            });
            job->start();
            break;
        }
        case NotificationInfo::ConferenceCall: {
            break;
        }
        }
    });
    connect(account, &RocketChatAccount::updateNotification, this, &AccountManager::updateNotification);
    connect(account, &RocketChatAccount::roomNeedAttention, this, &AccountManager::roomNeedAttention);
    connect(account, &RocketChatAccount::logoutDone, this, &AccountManager::logoutAccountDone);
}

void AccountManager::slotSwitchToAccountAndRoomName(const QString &accountName, const QString &roomName, const QString &channelType)
{
    setCurrentAccount(accountName);
    QString linkRoom;
    if (channelType == QLatin1Char('c')) {
        linkRoom = QStringLiteral("ruqola:/room/%1").arg(roomName);
    } else {
        linkRoom = QStringLiteral("ruqola:/user/%1").arg(roomName);
    }
    Q_EMIT mCurrentAccount->raiseWindow();
    Q_EMIT mCurrentAccount->openLinkRequested(linkRoom);
}

void AccountManager::loadAccount()
{
    qCDebug(RUQOLA_LOG) << " void AccountManager::loadAccount()" << ManagerDataPaths::self()->path(ManagerDataPaths::Config, QString());
    QDirIterator it(ManagerDataPaths::self()->path(ManagerDataPaths::Config, QString()),
                    QStringList() << QStringLiteral("ruqola.conf"),
                    QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot,
                    QDirIterator::Subdirectories);
    QVector<RocketChatAccount *> lstAccounts;
    while (it.hasNext()) {
        const QString val = it.next();
        qCDebug(RUQOLA_LOG) << "Account found list.at(i)" << val;
        auto account = new RocketChatAccount(val);
        if (account->settings()->isValid()) {
            connectToAccount(account);
            lstAccounts.append(account);
        } else {
            account->deleteLater();
        }
    }

    // New account => empty list.
    if (lstAccounts.isEmpty()) {
        qCDebug(RUQOLA_LOG) << "Empty list. Create a default rocketchataccount";
        auto account = new RocketChatAccount();
        if (account->accountEnabled()) {
            connectToAccount(account);
        }
        lstAccounts.append(account);
    }

    mRocketChatAccountModel->setAccounts(lstAccounts);

    QSettings settings;
    const QString previousAccount = settings.value(QStringLiteral("currentAccount"), QString()).toString();
    if (previousAccount.isEmpty()) {
        // Use first one
        mCurrentAccount = mRocketChatAccountModel->account(0);
    } else {
        selectAccount(previousAccount);
        if (!mCurrentAccount) {
            // Use first one
            mCurrentAccount = mRocketChatAccountModel->account(0);
        }
    }
    mRocketChatAccountProxyModel->setAccountOrder(RuqolaGlobalConfig::self()->accountOrder());
}

RocketChatAccountFilterProxyModel *AccountManager::rocketChatAccountProxyModel() const
{
    return mRocketChatAccountProxyModel;
}

RocketChatAccount *AccountManager::account() const
{
    return mCurrentAccount;
}

void AccountManager::addAccount(const AccountManagerInfo &info)
{
    // TODO verify if account exist or not ?
    auto account = new RocketChatAccount();
    account->setAccountName(info.accountName);
    account->setUserName(info.userName);
    account->setServerUrl(info.serverUrl);
    account->setAccountEnabled(info.enabled);
    account->setPassword(info.password);
    if (info.enabled) {
        connectToAccount(account);
    }
    addAccount(account);
}

void AccountManager::modifyAccount(const AccountManagerInfo &info)
{
    RocketChatAccount *account = mRocketChatAccountModel->account(info.accountName);
    if (account) {
        account->setDisplayName(info.displayName);
        account->setUserName(info.userName);
        account->setServerUrl(info.serverUrl);
        account->setAccountEnabled(info.enabled);
        if (!info.enabled) {
            // TODO fixme
            // disconnect(account, &RocketChatAccount::notification, this, &AccountManager::notification);
            disconnect(account, &RocketChatAccount::updateNotification, this, &AccountManager::updateNotification);
            disconnect(account, &RocketChatAccount::logoutDone, this, &AccountManager::logoutAccountDone);
        }
    }
}

RocketChatAccount *AccountManager::accountFromName(const QString &accountName)
{
    return mRocketChatAccountModel->account(accountName);
}

QStringList AccountManager::accountsName() const
{
    return mRocketChatAccountModel->accountsName();
}

void AccountManager::addAccount(RocketChatAccount *account)
{
    mRocketChatAccountModel->insertAccount(account);
}

void AccountManager::selectAccount(const QString &accountName)
{
    RocketChatAccount *account = mRocketChatAccountModel->account(accountName);
    if (account) {
        mCurrentAccount = account;
    } else {
        qCWarning(RUQOLA_LOG) << "AccountName " << accountName << " is not found on system.";
    }
}

void AccountManager::setCurrentAccount(const QString &accountName)
{
    RocketChatAccount *account = mRocketChatAccountModel->account(accountName);
    if (account) {
        if (mCurrentAccount != account) {
            QSettings settings;
            settings.setValue(QStringLiteral("currentAccount"), accountName);
            settings.sync();
            mCurrentAccount = account;
            Q_EMIT currentAccountChanged();
        }
    } else {
        qCWarning(RUQOLA_LOG) << "AccountName " << accountName << " is not found on system. Fallback to default one.";
    }
}

QString AccountManager::currentAccount() const
{
    return mCurrentAccount ? mCurrentAccount->accountName() : QString();
}

void AccountManager::removeLogs(const QString &accountName)
{
    const QString directory = LocalDatabaseUtils::localMessageLoggerPath() + accountName;
    removeDirectory(directory);
}

void AccountManager::removeDatabaseAccount(const QString &accountName)
{
    const QString directory = LocalDatabaseUtils::localAccountDatabasePath() + accountName;
    removeDirectory(directory);
}

void AccountManager::removeDirectory(const QString &directory)
{
    QDir dir(directory);
    if (dir.exists()) {
        if (!dir.removeRecursively()) {
            qCWarning(RUQOLA_LOG) << " Impossible to remove directory : " << directory;
        }
    }
}

void AccountManager::removeAccount(const QString &accountName, bool removeLogFiles)
{
    auto account = mRocketChatAccountModel->removeAccount(accountName);
    if (mRocketChatAccountModel->accountNumber() > 0) {
        mCurrentAccount = mRocketChatAccountModel->account(0);
        removeDatabaseAccount(accountName);
        // TODO remove others files
        if (removeLogFiles) {
            removeLogs(accountName);
        }
    } else {
        // TODO create new dummy account !
    }
    Q_EMIT currentAccountChanged();
    if (account) {
        account->deleteLater();
    }
}

RocketChatAccountModel *AccountManager::rocketChatAccountModel() const
{
    return mRocketChatAccountModel;
}

QStringList AccountManager::accountNamesSorted() const
{
    QStringList lst;
    auto model = rocketChatAccountProxyModel();
    for (int i = 0; i < model->rowCount(); ++i) {
        const auto index = model->index(i, 0);
        auto account = index.data(RocketChatAccountModel::Account).value<RocketChatAccount *>();
        lst << account->displayName();
    }
    return lst;
}

#include "moc_accountmanager.cpp"
