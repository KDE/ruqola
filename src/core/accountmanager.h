/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "authenticationinfo.h"
#include "config-ruqola.h"
#include "libruqolacore_export.h"
#include "parserocketchaturlutils.h"
#include "ruqolaserverconfig.h"

#include <QIcon>
#include <QObject>
class RocketChatAccount;
#if HAVE_ACTIVITY_SUPPORT
class ActivitiesManager;
#endif
class RocketChatAccountModel;
class RocketChatAccountFilterProxyModel;
#if HAVE_TEXT_TO_SPEECH
class TextToSpeechEnqueueManager;
#endif
class LIBRUQOLACORE_EXPORT AccountManager : public QObject
{
    Q_OBJECT
public:
    enum MigrateDatabaseType : qint8 {
        None = 0,
        All = 1,
        DatabaseWithoutLogger = 2,
    };
    Q_ENUM(MigrateDatabaseType)

    struct LIBRUQOLACORE_EXPORT ActivitySettings {
        QStringList activities;
        bool enabled = false;
        [[nodiscard]] bool contains(const QString &str) const
        {
            return activities.contains(str);
        }

        void changeActivities(bool added, const QString &currentActivity)
        {
            if (added) {
                if (!activities.contains(currentActivity)) {
                    activities.append(currentActivity);
                }
            } else {
                if (activities.contains(currentActivity)) {
                    activities.removeAll(currentActivity);
                }
            }
            enabled = true;
        }
    };

    struct LIBRUQOLACORE_EXPORT AccountManagerInfo {
        QList<AuthenticationInfo> authenticationInfos;
        ActivitySettings activitiesSettings;
        QString displayName;
        QString accountName;
        QString userName;
        QString serverUrl;
        QString password;
        QString token;
        QString inviteToken;

        QByteArray userId;
        AuthenticationManager::AuthMethodType authMethodType = AuthenticationManager::AuthMethodType::Unknown;
        RuqolaServerConfig::PasswordSettings passwordSettings;
        bool canResetPassword = false;
        bool enabled = true;
        bool canRegisterAccount = false;
        bool accountsManuallyApproveNewUsers = false;
    };

    struct LIBRUQOLACORE_EXPORT AccountDisplayInfo {
        QString name;
        QIcon icon;
    };

    explicit AccountManager(QObject *parent = nullptr);
    ~AccountManager() override;

    void removeAccount(const QString &accountName, bool removeLogFiles = false);
    void addAccount(AccountManagerInfo &&info);

    [[nodiscard]] RocketChatAccount *account() const;
    [[nodiscard]] RocketChatAccountModel *rocketChatAccountModel() const;

    [[nodiscard]] RocketChatAccountFilterProxyModel *rocketChatAccountProxyModel() const;

    void addAccount(RocketChatAccount *account);

    void setCurrentAccount(const QString &accountName);
    [[nodiscard]] QString currentAccount() const;

    void modifyAccount(AccountManagerInfo &&info);
    [[nodiscard]] QStringList accountsName() const;

    [[nodiscard]] bool isEmpty() const;

    [[nodiscard]] int accountNumber() const;
    [[nodiscard]] bool showMessage(const ParseRocketChatUrlUtils::ParsingInfo &parseInfo);
    [[nodiscard]] RocketChatAccount *accountFromName(const QString &accountName);

    [[nodiscard]] QList<AccountDisplayInfo> accountDisplayInfoSorted() const;
#if HAVE_ACTIVITY_SUPPORT
    ActivitiesManager *activitiesManager() const;
#endif

    void addInvitedAccount(const AccountManagerInfo &info);

    [[nodiscard]] TextToSpeechEnqueueManager *textToSpeechEnqueueManager() const;
Q_SIGNALS:
    void logoutAccountDone(const QString &accountName);
    void updateNotification(bool hasAlert, int nbUnread, const QString &accountName);
    void currentAccountChanged();
    void roomNeedAttention();
    void activitiesChanged();

private:
    LIBRUQOLACORE_NO_EXPORT void selectAccount(const QString &accountName);
    LIBRUQOLACORE_NO_EXPORT void removeDatabaseAccount(const QString &accountName);
    LIBRUQOLACORE_NO_EXPORT void removeDirectory(const QString &directory);
    LIBRUQOLACORE_NO_EXPORT void removeLogs(const QString &accountName);
    LIBRUQOLACORE_NO_EXPORT void loadAccount();
    LIBRUQOLACORE_NO_EXPORT void connectToAccount(RocketChatAccount *account);
    LIBRUQOLACORE_NO_EXPORT void slotSwitchToAccountAndRoomName(const QString &accountName, const QByteArray &roomId, const QString &channelType);
    LIBRUQOLACORE_NO_EXPORT void changeEnableState(RocketChatAccount *account, bool enabled);
    LIBRUQOLACORE_NO_EXPORT void disconnectAccount(RocketChatAccount *account);
    LIBRUQOLACORE_NO_EXPORT void slotAboutToSynthesizeChanged(qsizetype previousId, qsizetype currentId);
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT AccountManager::MigrateDatabaseType needToHandleDataMigration() const;
    RocketChatAccount *mCurrentAccount = nullptr;
    RocketChatAccountModel *const mRocketChatAccountModel;
    RocketChatAccountFilterProxyModel *const mRocketChatAccountProxyModel;
#if HAVE_ACTIVITY_SUPPORT
    ActivitiesManager *const mActivitiesManager;
#endif
    TextToSpeechEnqueueManager *const mTextToSpeechEnqueueManager;
};
Q_DECLARE_TYPEINFO(AccountManager::AccountManagerInfo, Q_RELOCATABLE_TYPE);
Q_DECLARE_TYPEINFO(AccountManager::AccountDisplayInfo, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const AccountManager::AccountManagerInfo &t);
