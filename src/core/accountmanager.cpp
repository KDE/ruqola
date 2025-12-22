/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "accountmanager.h"
#include "job/validateinviteserverjob.h"
#include "localdatabase/localdatabaseutils.h"
#include "managerdatapaths.h"
#include "model/rocketchataccountfilterproxymodel.h"
#include "model/rocketchataccountmodel.h"
#include "notificationhistorymanager.h"
#include "notifications/notifierjob.h"
#include "rocketchataccount.h"
#include "rocketchataccountsettings.h"
#include "ruqola_debug.h"
#if HAVE_TEXT_TO_SPEECH
#include "texttospeech/texttospeechenqueuemanager.h"
#include <TextEditTextToSpeech/TextToSpeech>
#endif

#include "ruqola.h"
#include "ruqola_sound_debug.h"
#include "ruqolaglobalconfig.h"
#include "utils.h"

#if HAVE_ACTIVITY_SUPPORT
#include "activities/activitiesmanager.h"
#endif
#include <KLocalizedString>
#include <KNotification>
#include <QDir>
#include <QDirIterator>
#include <QSettings>
#include <TextEmoticonsCore/EmojiModelManager>
#include <TextEmoticonsCore/UnicodeEmoticonManager>

using namespace Qt::Literals::StringLiterals;
namespace
{
// constexpr int currentDataBaseVersion = 1;
// Version 2 we need to delete local database as before we never supported remove room => we have some invalid rooms
constexpr int currentDataBaseVersion = 2;
}

AccountManager::AccountManager(QObject *parent)
    : QObject(parent)
    , mRocketChatAccountModel(new RocketChatAccountModel(this))
    , mRocketChatAccountProxyModel(new RocketChatAccountFilterProxyModel(this))
#if HAVE_ACTIVITY_SUPPORT
    , mActivitiesManager(new ActivitiesManager(this))
#endif
    , mTextToSpeechEnqueueManager(new TextToSpeechEnqueueManager(this))
{
#if HAVE_ACTIVITY_SUPPORT
    mRocketChatAccountProxyModel->setActivitiesManager(mActivitiesManager);
    // TODO disable/enable account
#endif
    mRocketChatAccountProxyModel->setSourceModel(mRocketChatAccountModel);
    TextEmoticonsCore::UnicodeEmoticonManager::self(u":/emoji_ruqola.json"_s);
    loadAccount();
    connect(this, &AccountManager::activitiesChanged, mRocketChatAccountProxyModel, &RocketChatAccountFilterProxyModel::slotActivitiesChanged);
    connect(TextEditTextToSpeech::TextToSpeech::self(),
            &TextEditTextToSpeech::TextToSpeech::aboutToSynthesize,
            this,
            &AccountManager::slotAboutToSynthesizeChanged);
}

AccountManager::~AccountManager() = default;

int AccountManager::accountNumber() const
{
    return mRocketChatAccountModel->accountNumber();
}

bool AccountManager::showMessage(const ParseRocketChatUrlUtils::ParsingInfo &parseInfo)
{
    auto rocketChatAccount = mRocketChatAccountModel->accountFromServerUrl(parseInfo.serverHost);
    if (rocketChatAccount) {
        // const QString path{parseUrl.path()};
        const QByteArray messageId = parseInfo.messageId.toLatin1();
        qCDebug(RUQOLA_LOG) << " parseUrl " << parseInfo;
        // https://<server url>/channel/python?msg=sn3gEQom7NcLxTg5h
        setCurrentAccount(rocketChatAccount->accountName());
        // qDebug() << " account->accountName() : " << account->accountName();
        Q_EMIT mCurrentAccount->raiseWindow();
        Q_EMIT mCurrentAccount->selectChannelAndMessage(messageId, parseInfo.roomId, parseInfo.roomIdType, parseInfo.channelType);
        return true;
    }
    return false;
}

void AccountManager::disconnectAccount(RocketChatAccount *account)
{
    disconnect(account, &RocketChatAccount::updateNotification, this, &AccountManager::updateNotification);
    disconnect(account, &RocketChatAccount::roomNeedAttention, this, &AccountManager::roomNeedAttention);
    disconnect(account, &RocketChatAccount::logoutDone, this, &AccountManager::logoutAccountDone);
    disconnect(account, &RocketChatAccount::activitiesChanged, this, &AccountManager::activitiesChanged);
    // TODO connect(account, &RocketChatAccount::notification
}

#if HAVE_ACTIVITY_SUPPORT
ActivitiesManager *AccountManager::activitiesManager() const
{
    return mActivitiesManager;
}
#endif

void AccountManager::connectToAccount(RocketChatAccount *account)
{
    connect(account, &RocketChatAccount::notification, this, [this, account](const NotificationInfo &info) {
        NotificationHistoryManager::self()->addNotification(info);

        switch (info.notificationType()) {
        case NotificationInfo::NotificationType::StandardMessage: {
            if (const Room *room = account->room(info.roomId())) {
                const QByteArray audioNotificationId = room->notificationOptions().audioNotificationValue();
                account->playSound(audioNotificationId);
            } else {
                qCWarning(RUQOLA_SOUND_LOG) << "Room doesn't exist!" << info.roomId();
            }
            auto job = new NotifierJob;
            job->setInfo(info);
            connect(job, &NotifierJob::switchToAccountAndRoomName, this, &AccountManager::slotSwitchToAccountAndRoomName);
            connect(job, &NotifierJob::sendReply, this, [account](const QString &str, const QByteArray &roomId, const QByteArray &tmId) {
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
        case NotificationInfo::NotificationType::ConferenceCall: {
            break;
        }
        }
    });
    connect(account, &RocketChatAccount::updateNotification, this, &AccountManager::updateNotification);
    connect(account, &RocketChatAccount::roomNeedAttention, this, &AccountManager::roomNeedAttention);
    connect(account, &RocketChatAccount::logoutDone, this, &AccountManager::logoutAccountDone);
    connect(account, &RocketChatAccount::activitiesChanged, this, &AccountManager::activitiesChanged);
}

void AccountManager::slotSwitchToAccountAndRoomName(const QString &accountName, const QByteArray &roomId, const QString &channelType)
{
    setCurrentAccount(accountName);
    QString linkRoom;
    if (channelType == u'c') {
        linkRoom = u"ruqola:/room/%1"_s.arg(QString::fromLatin1(roomId));
    } else {
        linkRoom = u"ruqola:/user/%1"_s.arg(QString::fromLatin1(roomId));
    }
    Q_EMIT mCurrentAccount->raiseWindow();
    Q_EMIT mCurrentAccount->openLinkRequested(linkRoom);
}

AccountManager::MigrateDatabaseType AccountManager::needToHandleDataMigration() const
{
    if (RuqolaGlobalConfig::self()->databaseVersion() == 0 && currentDataBaseVersion == 1) {
        return MigrateDatabaseType::All;
    } else if (RuqolaGlobalConfig::self()->databaseVersion() == 1 && currentDataBaseVersion == 2) {
        return MigrateDatabaseType::DatabaseWithoutLogger;
    }
    return MigrateDatabaseType::None;
}

TextToSpeechEnqueueManager *AccountManager::textToSpeechEnqueueManager() const
{
    return mTextToSpeechEnqueueManager;
}

void AccountManager::slotAboutToSynthesizeChanged(qsizetype previousId, qsizetype currentId)
{
    // qDebug() << " previousId " << previousId << " currentId " << currentId;
    if (previousId != -1) {
        const TextToSpeechEnqueueInfo info = mTextToSpeechEnqueueManager->value(previousId);
        // qDebug() << " previous info " << info;
        if (info.isValid()) {
            RocketChatAccount *account = mRocketChatAccountModel->account(info.accountName());
            if (account) {
                account->updateTextToSpeech(info.roomId(), info.messageId(), false);
            }
        }
        // qDebug() << " enqueue list " << mTextAutoGenerateTextToSpeechEnqueueManager->enqueueList() << "previousId " << previousId;
    }
    if (currentId != -1) {
        const TextToSpeechEnqueueInfo info = mTextToSpeechEnqueueManager->value(currentId);
        // qDebug() << " current info " << info;
        if (info.isValid()) {
            RocketChatAccount *account = mRocketChatAccountModel->account(info.accountName());
            if (account) {
                account->updateTextToSpeech(info.roomId(), info.messageId(), true);
            }
            // qDebug() << " enqueue list " << mTextAutoGenerateTextToSpeechEnqueueManager->enqueueList() << "currentId " << currentId;
        }
    } else {
        mTextToSpeechEnqueueManager->clear();
        // qDebug() << mTextAutoGenerateTextToSpeechEnqueueManager->enqueueList();
    }
}

void AccountManager::loadAccount()
{
    const AccountManager::MigrateDatabaseType needDatabaseMigration = needToHandleDataMigration();
    if (needDatabaseMigration != AccountManager::MigrateDatabaseType::None) {
        RuqolaGlobalConfig::self()->setDatabaseVersion(currentDataBaseVersion);
        RuqolaGlobalConfig::self()->save();

        QStringList lst = {LocalDatabaseUtils::localDatabasePath()};
        if (needDatabaseMigration == AccountManager::MigrateDatabaseType::All) {
            lst += LocalDatabaseUtils::localMessageLoggerPath();
        }
        qDebug() << " Delete database : " << lst;
        for (const QString &path : lst) {
            QDir dir(path);
            if (dir.exists()) {
                if (!dir.removeRecursively()) {
                    qCWarning(RUQOLA_LOG) << "Impossible to remove database from " << dir.absolutePath();
                }
            }
        }
    }

    qCDebug(RUQOLA_LOG) << " void AccountManager::loadAccount()" << ManagerDataPaths::self()->path(ManagerDataPaths::Config, QString());
    QDirIterator it(ManagerDataPaths::self()->path(ManagerDataPaths::Config, QString()),
                    QStringList() << u"ruqola.conf"_s,
                    QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot,
                    QDirIterator::Subdirectories);
    QList<RocketChatAccount *> lstAccounts;
    while (it.hasNext()) {
        const QString val = it.next();
        qCDebug(RUQOLA_LOG) << "Account found list.at(i)" << val;
        auto account = new RocketChatAccount(val);
        if (account->settings()->isValid()) {
            if (account->accountEnabled()) {
                connectToAccount(account);
            }
            lstAccounts.append(account);
        } else {
            account->deleteLater();
        }
    }

    mRocketChatAccountModel->setAccounts(lstAccounts);

    const QSettings settings;
    const QString currentAccount = settings.value("currentAccount"_L1, QString()).toString();
    if (currentAccount.isEmpty()) {
        auto account = mRocketChatAccountModel->account(0);
        if (account && account->accountEnabled()) {
            // Use first one
            mCurrentAccount = account;
        }
    } else {
        selectAccount(currentAccount);
        if (!mCurrentAccount) {
            // Use first one
            auto account = mRocketChatAccountModel->account(0);
            if (account && account->accountEnabled()) {
                mCurrentAccount = account;
            }
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

void AccountManager::changeEnableState(RocketChatAccount *account, bool enabled)
{
    if (enabled) {
        connectToAccount(account);
    } else {
        disconnectAccount(account);
    }
}

void AccountManager::addInvitedAccount(const AccountManagerInfo &info)
{
    auto job = new ValidateInviteServerJob(this);
    job->setInfo(info);
    connect(job, &ValidateInviteServerJob::tokenIsInvalid, this, []() {
        auto notification = new KNotification(u"Invite-Account-Invalid"_s, KNotification::CloseOnTimeout);
        notification->setTitle(i18n("Invalid invite token"));
        notification->setText(i18n("Invite Token is invalid. Creating account canceled."));
        notification->sendEvent();
    });
    connect(job, &ValidateInviteServerJob::tokenIsValid, this, [](const AccountManager::AccountManagerInfo &info) {
        Q_EMIT Ruqola::self()->addInviteServer(info);
    });
    job->start();
}

void AccountManager::addAccount(AccountManagerInfo &&info)
{
    const QString newAccountName = Utils::createUniqueAccountName(accountsName(), info.accountName);
    auto account = new RocketChatAccount();
    account->setAccountName(newAccountName);
    account->setServerUrl(info.serverUrl);
    auto settings = account->settings();
    settings->setAccountEnabled(info.enabled);
    settings->setActivities(info.activitiesSettings.activities);
    settings->setActivityEnabled(info.activitiesSettings.enabled);
    settings->setInviteToken(info.inviteToken);
    if (info.authMethodType == AuthenticationManager::AuthMethodType::Password) {
        settings->setUserName(info.userName);
        settings->setPassword(info.password);
    } else if (info.authMethodType == AuthenticationManager::AuthMethodType::PersonalAccessToken) {
        settings->setAuthToken(info.token);
        settings->setUserId(info.userId);
    } else {
        // TODO for other authMethodType ?
        // google used ?
        // Fb ?
        // Gitlab ?
        // GitHub ?
    }
    settings->setAuthMethodType(info.authMethodType);
    if (info.enabled) {
        connectToAccount(account);
    }
    addAccount(account);
}

void AccountManager::modifyAccount(AccountManagerInfo &&info)
{
    RocketChatAccount *account = mRocketChatAccountModel->account(info.accountName);
    if (account) {
        auto settings = account->settings();
        settings->setDisplayName(info.displayName);
        account->setServerUrl(info.serverUrl);
        settings->setAccountEnabled(info.enabled);
        settings->setAuthMethodType(info.authMethodType);
        settings->setActivities(info.activitiesSettings.activities);
        settings->setActivityEnabled(info.activitiesSettings.enabled);
        if (info.authMethodType == AuthenticationManager::AuthMethodType::Password) {
            settings->setUserName(info.userName);
            // TODO add password ???
        } else if (info.authMethodType == AuthenticationManager::AuthMethodType::PersonalAccessToken) {
            settings->setAuthToken(info.token);
            settings->setUserId(info.userId);
        } else {
            // TODO ????
        }
        if (!info.enabled && account->accountEnabled()) {
            changeEnableState(account, false);
        } else if (info.enabled && !account->accountEnabled()) {
            changeEnableState(account, true);
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

bool AccountManager::isEmpty() const
{
    return mRocketChatAccountModel->isEmpty();
}

void AccountManager::addAccount(RocketChatAccount *account)
{
    const bool wasEmpty = isEmpty();
    mRocketChatAccountModel->insertAccount(account);
    const QString accountName = account->accountName();
    if (wasEmpty) {
        setCurrentAccount(accountName);
    }
}

void AccountManager::selectAccount(const QString &accountName)
{
    RocketChatAccount *account = mRocketChatAccountModel->account(accountName);
    if (account && account->accountEnabled()) {
        mCurrentAccount = account;
    } else {
        mCurrentAccount = nullptr;
        qCWarning(RUQOLA_LOG) << "AccountName " << accountName << " is not found on system.";
    }
}

void AccountManager::setCurrentAccount(const QString &accountName)
{
    RocketChatAccount *account = mRocketChatAccountModel->account(accountName);
    if (account) {
        if (mCurrentAccount != account) {
            QSettings settings;
            settings.setValue("currentAccount"_L1, accountName);
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
    const QString directory = LocalDatabaseUtils::localAccountsDatabasePath() + accountName;
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

QList<AccountManager::AccountDisplayInfo> AccountManager::accountDisplayInfoSorted() const
{
    QList<AccountManager::AccountDisplayInfo> lst;
    auto model = rocketChatAccountProxyModel();
    for (int i = 0; i < model->rowCount(); ++i) {
        const auto index = model->index(i, 0);
        auto account = index.data(RocketChatAccountModel::Account).value<RocketChatAccount *>();
        if (account->accountEnabled()) {
            AccountManager::AccountDisplayInfo info;
            info.name = account->settings()->displayName();
            info.icon = Utils::iconFromAccount(account);
            lst.append(std::move(info));
        }
    }
    return lst;
}

QDebug operator<<(QDebug d, const AccountManager::AccountManagerInfo &t)
{
    d.space() << "authenticationInfos" << t.authenticationInfos;
    d.space() << "displayName" << t.displayName;
    d.space() << "accountName" << t.accountName;
    d.space() << "userName" << t.userName;
    d.space() << "serverUrl" << t.serverUrl;
    d.space() << "token" << t.token;
    d.space() << "userId" << t.userId;
    d.space() << "authMethodType" << t.authMethodType;
    d.space() << "canResetPassword" << t.canResetPassword;
    d.space() << "enabled" << t.enabled;
    d.space() << "canRegisterAccount" << t.canRegisterAccount;
    d.space() << "activities" << t.activitiesSettings.activities;
    d.space() << "activities enabled" << t.activitiesSettings.enabled;
    d.space() << "invite token" << t.inviteToken;
    return d;
}

#include "moc_accountmanager.cpp"
